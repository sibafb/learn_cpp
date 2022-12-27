#include <iostream>
#include <thread>

using namespace std;

class Threadclass{
	public:
		void setMember()
		{
			while(true)
			{  
				if(m_flag)
				{
					m_flag = false;
				}else{
					m_flag = true;
				}
				this_thread::sleep_for(chrono::seconds(1));
			}
		}
		bool getFlag()
		{
			return m_flag;
		}
	private:
		bool m_flag;
};

int main(int argc, char const *argv[]){

	Threadclass thClass;

	//thread th(&Threadclass::setMember, &thClass); 
	thread th2([&thClass]() { thClass.setMember(); });
	th2.detach();

	int counter=0;
	while(counter < 20)
	{ 
		cout<< thClass.getFlag() << endl;
		this_thread::sleep_for(chrono::milliseconds(500));
		counter++;
	}

	return 0;
}