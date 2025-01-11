#include <iostream>
#include <thread>
#include <chrono>
#include <functional>

using namespace std::chrono_literals;

void test1(int &i){
    while(i<10){
        if (i%2 == 0){
            std::cout << "Thread 1 " << i << std::endl;
            std::this_thread::sleep_for(2000ms);
            i++;
        }
    }
}

void test2(int &i){
    while(i<10){
        if (i%2 != 0) {
            std::cout << "Thread 2 " << i << std::endl;
            std::this_thread::sleep_for(2000ms);
            i++;
        }
    }
}

int main(){
    int i = 0;
    std::thread{test1, std::ref(i)}.detach();
    std::thread{test2, std::ref(i)}.detach();
    // firstThread.join();
    // firstThread.join();
    // secondThread.join();

    while (i<10){
        
    }

    return 0;
}