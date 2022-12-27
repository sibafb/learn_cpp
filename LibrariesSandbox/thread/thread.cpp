#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void work(){
	for(int i = 0; i < 10; i++){
		cout << std::this_thread::get_id() << " hello world" << endl;
	}
}

void work2(){
	for(int i = 0; i < 10; i++){
		cout << std::this_thread::get_id() << " good bye world" << endl;
	}
}

int main(int argc, char const *argv[]){
	thread th(work);
	thread th2(work2);

	th.join();
	th2.detach();
    sleep(5);
	return 0;
}