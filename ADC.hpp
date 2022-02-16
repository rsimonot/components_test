#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <bitset>

#include "colorfullcpp.hpp"

int detectADCDevice(int i2c_address_ADC)
{
    int address = i2c_address_ADC;
    std::cout << P_B << "-> Detecting ADC Device" << N << std::endl;
    std::cout << P << "> Testing with address : " << N << address << std::endl;
    if (wiringPiI2CSetup(address) >= 0 && wiringPiI2CWrite(wiringPiI2CSetup(address), 0) >= 0) {
        std::cout << G << "  [+]  Found !" << N << std::endl;
        return address;
    }
    std::cout << R << "  [-]  Nope." << N << std::endl;

    std::cout << R_B << "ERR : Unable to locate ADC Device" << N << std::endl;
    return -1;
}
