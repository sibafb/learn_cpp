/** 増補改訂版　Java 言語で学ぶデザインパターン入門　【マルチスレッド編】p119 GuardedSuspection.cpp **/
#include <iostream>
#include <memory>
#include <deque>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <random>

class Request
{
public:
	Request(std::string name) 
	{
		m_name = name;
	}
	std::string getName()
	{
		return m_name;
	}
	std::string toString()
	{
		return "[ Request " + m_name + " ]";
	}
private:
	std::string m_name; 
};

class RequestQueue 
{
public:
	RequestQueue()
	{
	}
	Request getRequest()
	{
		while (m_dequeue.empty())
		{
			std::unique_lock<std::mutex> ul(m_mutex);
			m_cv.wait(ul);
		}
		Request ret = m_dequeue.front();
		m_dequeue.pop_front();
		return ret;
	}
	void putRequest(Request request)
	{
		std::unique_lock<std::mutex> ul(m_mutex);
		//サイズ制限をキュー側に設けるのが妥当だが、勉強用なので設けない
		m_dequeue.emplace_back(request);
		m_cv.notify_one();
	}
private:
	std::deque<Request> m_dequeue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
};

class ClientThread 
{
public:
	ClientThread(std::shared_ptr<RequestQueue> requestQueue, std::string name)
		:m_requestQueue(requestQueue),
		 m_clientThread(m_run),
		 m_name(name)
	{
	}
	~ClientThread()
	{
	}
	void join()
	{
		m_clientThread.join();
	}
	void detach(){
		m_clientThread.detach();
	}
private:
	std::function<void()> m_run = [this]
	{
		std::cout<<"ClientThread id :" << std::this_thread::get_id() <<std::endl;
		for(int i=0; i < 500; i++)
		{
			Request request("No." + std::to_string(i));
			std::cout << m_name << " requests " << request.toString()<< std::endl;
			m_requestQueue->putRequest(request);
			std::this_thread::sleep_for(std::chrono::milliseconds(dist1(engine)));
		}
	};
	std::shared_ptr<RequestQueue> m_requestQueue;

	std::random_device m_seed_gen;
  	std::default_random_engine engine{m_seed_gen()};
	std::uniform_int_distribution<> dist1{1,1000};

	std::string m_name;
	std::thread m_clientThread;
};

class ServerThread
{
public:
	ServerThread(std::shared_ptr<RequestQueue> requestQueue, std::string name)
		:m_requestQueue(requestQueue),
		 m_serverThread(m_run),
		 m_name(name)
	{
	}
	~ServerThread()
	{
	}
	void join()
	{
		m_serverThread.join();
	}
	void detach(){
		m_serverThread.detach();
	}
private:
	std::function<void()> m_run = [this]
	{
		std::cout<<"ServerThread id :" << std::this_thread::get_id() <<std::endl;
		for(int i = 0; i < 500; i++)
		{
			Request request = m_requestQueue->getRequest();
			std::cout<< m_name << "   handles  " << request.toString() << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(dist1(engine)));
		}
	};
	std::shared_ptr<RequestQueue> m_requestQueue;
	
	std::random_device m_seed_gen;
  	std::default_random_engine engine{m_seed_gen()};
	std::uniform_int_distribution<> dist1{1,1000};

	std::string m_name;
	std::thread m_serverThread;
};

int main()
{
	std::shared_ptr<RequestQueue> requestQueue = std::make_shared<RequestQueue>();
	ClientThread clientThread(requestQueue, "Alice");
	ServerThread serverThread(requestQueue, "Bob");

	clientThread.join();
	serverThread.join();
}