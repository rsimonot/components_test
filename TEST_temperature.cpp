#include "ADC.hpp"  // ADC functions, iostream, wiringPi.h, wiringPiI2C.h, bitset, colorfullcpp

int main (void)
{
    int i2c_address_doc = 0x48;    // I2C Address of TMP116N
    int ADC_address = detectADCDevice(i2c_address_doc);
    if (ADC_address < 0)
        return EXIT_FAILURE;

    int id_register = 0x0f;
    int temp_register = 0x00;
    int fd = wiringPiI2CSetup(ADC_address);

    wiringPiI2CWrite(fd, id_register);
    int id = wiringPiI2CRead(fd);
    if (id < 0) {
        std::cout << R_B << "ERR : Unable to read from ADC Device" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+] Successfully read ID from ADC Device !" << N << std::endl;
    std::cout << P_B << "ID value -> " << P << id << N << std::endl;

    int r_value = wiringPiI2CReadReg8(fd, temp_register);
    if (r_value < 0) {
        std::cout << R_B << "ERR : Unable to read from ADC Device" << N << std::endl;
        return EXIT_FAILURE;
    }
    double temp = r_value * 0.0078125;
    std::cout << G << "  [+] Successfully read Temperature from ADC Device !" << N << std::endl;
    std::cout << P_B << "TEMP value -> " << P << r_value << "  |  " << temp << N << std::endl;

    return EXIT_SUCCESS;
}

// Compile with :
// g++ -o test/temp TEST_temperature.cpp -lwiringPi