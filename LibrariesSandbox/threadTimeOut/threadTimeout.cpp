#include <iostream>
#include <future>
#include <thread>

using namespace std;

class ThreadTimeout{
	public:
		bool waitOntime(int waitTime, promise<int>ret)
		{
			this_thread::sleep_for(chrono::seconds(waitTime - 1));
			ret.set_value(1);//値はなんでも
			return true;
		}
		bool waitTimeout(int waitTime, promise<int>ret)
		{
			this_thread::sleep_for(chrono::seconds(waitTime + 1));
			ret.set_value(1);
			return true;
		}
		void inClassTimeout(int waitTime)
		{
			promise<int> m_p;
			future<int> m_f = m_p.get_future();;
			thread m_th(&ThreadTimeout::waitOntime, this, waitTime, move(m_p));
			std::future_status m_result = m_f.wait_for(std::chrono::seconds(waitTime));
			
			if (m_result != std::future_status::timeout)
			{
				std::cout << "On time m_f:" << m_f.get() << std::endl;
			}else{
				std::cout << "Time out m_f!" << std::endl;
			}

			m_th.join();	
		}
	private:

};

int main(int argc, char const *argv[])
{
	promise<int> p1,p2;
  	future<int> f1 = p1.get_future();
	future<int> f2 = p2.get_future();

	ThreadTimeout thTimeout;

	thread th1(&ThreadTimeout::waitOntime, &thTimeout, 3, move(p1));
	std::future_status result1 = f1.wait_for(std::chrono::seconds(3));
	if (result1 != std::future_status::timeout)
	{
    	std::cout << "On time f1:" << f1.get() << std::endl;
  	}else{
		std::cout << "Time out f1!" << std::endl;
	}
	th1.join();

	thread th2(&ThreadTimeout::waitTimeout,&thTimeout, 3, move(p2));
	std::future_status result2 = f2.wait_for(std::chrono::seconds(3));
	if (result2 != std::future_status::timeout)
	{
    	std::cout << "On time f2:" << f2.get() << std::endl;
  	}else{
		std::cout << "Time out f2!" << std::endl;
	}
	th2.join();

	thTimeout.inClassTimeout(3);

	return 0;
}