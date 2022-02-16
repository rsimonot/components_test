#include "ADC.hpp"  // ADC functions, iostream, wiringPi.h, wiringPiI2C.h, bitset, colorfullcpp
#include <math.h>

int main (void)
{
    int i2c_address_doc = 0x48;    // I2C Address of TMP116N
    int ADC_address = detectADCDevice(i2c_address_doc);
    if (ADC_address < 0)
        return EXIT_FAILURE;

    int temp_register = 0x00;
    int fd = wiringPiI2CSetup(ADC_address);
    wiringPiI2CWrite(fd, temp_register);
    int r_value = wiringPiI2CRead(fd);
    if (r_value < 0) {
        std::cout << R_B << "ERR : Unable to read from ADC Device" << N << std::endl;
        return EXIT_FAILURE;
    }
    float voltage = (float)r_value / 255.0 * 3.3;    // calculate voltage    
    float Rt = 10 * voltage / (3.3 - voltage);        //calculate resistance value of thermistor
    float tempK = 1/(1/(273.15 + 25) + log(Rt/10)/3950.0); //calculate temperature (Kelvin)
    float tempC = tempK -273.15;        //calculate temperature (Celsius)
    std::cout << G << "  [+] Successfully read from ADC Device !" << N << std::endl;
    std::cout << P_B << "TEMP value read -> " << P << r_value << N << std::endl;

    return EXIT_SUCCESS;
}

// Compile with :
// g++ -o test/temp TEST_temperature.cpp -lwiringPi