/** 増補改訂版　Java 言語で学ぶデザインパターン入門　【マルチスレッド編】p164 Producer-Consumer.cpp **/
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>
#include <random>
#include <condition_variable>
#include <deque>

using std::string;
using namespace std::literals::chrono_literals;

class Table{
public:
	Table(int buffer_size)
	{
		m_bufferMaxSize = buffer_size;
	}
	void put(std::string cake)
	{
		std::unique_lock<std::mutex> ul(m_mutex);
		while (m_bufferMaxSize <= m_buffer.size())
		{
			m_cv.wait(ul);
		}
		std::cout<< std::this_thread::get_id() <<"put."<<std::endl;
		m_buffer.emplace_back(cake);
		m_cv.notify_all();
	}
	std::string take()
	{
		std::unique_lock<std::mutex> ul(m_mutex);
		while (m_buffer.empty())
		{
			m_cv.wait(ul);
		}
		std::cout<< std::this_thread::get_id() <<"take."<<std::endl;
		std::string front = m_buffer.front();
		m_buffer.pop_front();
		m_cv.notify_all();
		return front;
	}
private:
	int m_bufferMaxSize;
	std::mutex m_mutex;
	std::deque<std::string> m_buffer;
	std::condition_variable m_cv;
};

class MakerThread 
{
public:
	MakerThread(std::string name, std::shared_ptr<Table> table)
		:m_name(name),
		m_table(table),
		m_thread(m_run)
	{
	}

	static int id;
	int nextId()
	{
		std::lock_guard<std::mutex> lock(id_lock);
		id++;
		return id;
	}
	void join()
	{
		m_thread.join();
	}

	void detach(){
		m_thread.detach();
	}

	void requestTernimation()
	{
		m_isTerminationRequested = true;
	}

private:
	bool m_isTerminationRequested = false;
	
	std::function<void()> m_run = [this]
	{
		std::cout<< std::this_thread::get_id() <<"Maker m_run start."<<std::endl;
		while(!m_isTerminationRequested)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(dist1(engine)));
			std::string cake = "[ Cake No." + std::to_string(nextId()) + " by " + m_name + " ]";
			m_table->put(cake);
		}
	};
	std::shared_ptr<Table> m_table;
	
	std::mutex id_lock;

	std::random_device m_seed_gen;
  	std::default_random_engine engine{m_seed_gen()};
	std::uniform_int_distribution<> dist1{1,2000};

	std::string m_name;
	std::thread m_thread;
};

class EaterThread
{
public:
	EaterThread(std::string name, std::shared_ptr<Table> table)
		:m_name(name),
		m_table(table),
		m_thread(m_run)
	{
	}

	void join()
	{
		m_thread.join();
	}

	void detach(){
		m_thread.detach();
	}

	void requestTernimation()
	{
		m_isTerminationRequested = true;
	}
private:
	bool m_isTerminationRequested = false;
	std::function<void()> m_run = [this]
	{
		std::cout<< std::this_thread::get_id()<<"Eater m_run start."<<std::endl;
		while(!m_isTerminationRequested)
		{
			std::string cake = m_table->take();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	};
	std::shared_ptr<Table> m_table;

	std::random_device m_seed_gen;
  	std::default_random_engine engine{m_seed_gen()};
	std::uniform_int_distribution<> dist1{1,1000};

	std::string m_name;
	std::thread m_thread;
};

int MakerThread::id = 0;

int main()
{
	std::cout<<"Producer and Consumer"<<std::endl;
	std::shared_ptr table = std::make_shared<Table>(3);

	MakerThread maker1("maker 1.",table);
	MakerThread maker2("maker 2.",table);
	MakerThread maker3("maker 3.",table);
	EaterThread eater1("eater 1", table);
	EaterThread eater2("eater 2", table);
	EaterThread eater3("eater 3", table);

	maker1.join();
	maker2.join();
	maker3.join();
	eater1.join();
	eater2.join();
	eater3.join();
}
