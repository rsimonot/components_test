/* Adresses detected : 15, 36, 48, 51, 70 */
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <vector>
#include <string>

std::string PURPLE = "\033[1;35m";
std::string BLUE = "\033[1;36m";
std::string GREEN = "\033[1;32m";
std::string RED = "\033[1;31m";
std::string NORM = "\033[0m";

int main(void)
{
    std::vector<int> addresses{0x15, 0x36, 0x48, 0x51, 0x70};
    int test;
    wiringPiSetup();
    
    for (int i = 0; i < addresses.size() ; i++)
    {
        std::cout << PURPLE << "-> Testing for device registered at adress : " << addresses[i] << NORM << std::endl;
        test = wiringPiI2CSetup(addresses[i]);
        if (test < 0)
            std::cout << RED << " [-]  Error for adress <" << addresses[i] << ">" << std::endl;
        if ( wiringPiI2CWrite(test, 0) < 0)
            std::cout << RED << " [-]  No device found at address <" << addresses[i] << ">" << std::endl;
        else
            std::cout << GREEN << " [+]  Devide found at address <" << addresses[i] << ">" << std::endl;
    }
    
    return EXIT_SUCCESS;
}

// Compile with :
//  g++ -o test/i2c-detect TEST_i2c_detect.cpp -lwiringPi