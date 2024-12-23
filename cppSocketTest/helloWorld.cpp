#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>

int main() {
    std::vector<std::string> x = {"hello", "bye", "hi", "okay", "no"};

    std::mt19937 gen(time(0));
    std::uniform_int_distribution<> distrib(0, 4);

    int randomNumber = distrib(gen);
    std::cout << x[randomNumber] << std::endl;
    randomNumber = distrib(gen);
    std::cout << x[randomNumber] << std::endl;
    randomNumber = distrib(gen);
    std::cout << x[randomNumber] << std::endl;
    randomNumber = distrib(gen);
    std::cout << x[randomNumber] << std::endl;
    randomNumber = distrib(gen);
    std::cout << x[randomNumber] << std::endl;


    return 0;
}