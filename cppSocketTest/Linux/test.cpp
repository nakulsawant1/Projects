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
    std::thread firstThread(test1, std::ref(i));
    std::thread secondThread(test1, std::ref(i));
    firstThread.join();
    firstThread.join();
    // secondThread.join();
    return 0;
}