/* Adresses detected : 15, 36, 48, 51, 70 */
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <vector>
#include <string>

#include "colorfullcpp.hpp"

int main(void)
{
    std::vector<int> addresses{0x15, 0x36, 0x48, 0x51, 0x70, 0x71};     // 0x71 is an intentionnal error for a test purpose
    wiringPiSetup();
    std::cout << P_B << "-> wiringPiSetup OK, testing the I2C devices" << std::endl;
    std::cout << "-> Last test is supposed to fail !" <<  N << std::endl;
    
    for (int i = 0; i < addresses.size() ; i++)
    {
        std::cout << P << " -> Testing for device registered at adress : " << addresses[i] << N << std::endl;
        int test = wiringPiI2CSetup(addresses[i]);
        if (test < 0)
            std::cout << R << " [-]  Error for adress <" << addresses[i] << ">" << N << std::endl;
        if ( wiringPiI2CWrite(test, 0) < 0)
            std::cout << R << " [-]  No device found at address <" << addresses[i] << ">" << N << std::endl;
        else
            std::cout << G << " [+]  Devide found at address <" << addresses[i] << ">" << N << std::endl;
    }
    
    return EXIT_SUCCESS;
}

// Compile with :
//  g++ -o test/i2c-detect TEST_i2c_detect.cpp -lwiringPi