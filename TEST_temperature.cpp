#include "ADC.hpp"  // ADC functions, iostream, wiringPi.h, wiringPiI2C.h, bitset, colorfullcpp

int main (void)
{
    int i2c_address_doc = 0x36;    // I2C Address of MAX17261METD
    int ADC_address = detectADCDevice(i2c_address_doc);
    if (ADC_address < 0)
        return EXIT_FAILURE;

    int ADC_pin = 1/* WRONG ON PURPOSE */;
    int fd = wiringPiI2CSetup(ADC_address);
    wiringPiI2CWrite(fd, ADC_pin);
    int r_value = wiringPiI2CRead(fd);
    if (r_value < 0) {
        std::cout << R_B << "ERR : Unable to read from ADC Device" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+] Successfully read from ADC Device !" << N << std::endl;
    std::cout << P_B << "BATT value read -> " << P << r_value << N << std::endl;

    return EXIT_SUCCESS;
}


// Compile with :
// g++ -o test/temp TEST_temperature.cpp -lwiringPi