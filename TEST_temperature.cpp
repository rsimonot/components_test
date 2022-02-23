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

    //int id = wiringPiI2CReadReg16(fd, id_register);
    std::bitset<16>id_bits(wiringPiI2CReadReg16(fd, id_register));
    int id = static_cast<int16_t>(id_bits.to_ullong());
    id = id>>8 | ((id<<8) & 0xffff);
    if (id < 0) {
        std::cout << R_B << "ERR : Unable to read from ADC Device" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+] Successfully read ID from ADC Device !" << N << std::endl;
    std::cout << P_B << "ID value -> " << P << id << "  |  bits=>" << id_bits << N << std::endl;

    unsigned int _temp = wiringPiI2CReadReg16(fd, temp_register);
    float temp = static_cast<int16_t>( _temp>>8|((_temp<<8)&0xffff)) * 0.0078125;
    /*std::bitset<16>temp_bits(wiringPiI2CReadReg16(fd, temp_register));
    float temp = static_cast<int16_t>(temp_bits.to_ulong() & 0xFFFF) * 0.0078125; */
    std::cout << G << "  [+] Successfully read Temperature from ADC Device !" << N << std::endl;
    std::cout << P_B << "TEMP value -> " << P << temp << "°C" << "  |  bits=>" << _temp << N << std::endl;
    return EXIT_SUCCESS;
}

// Compile with :
// g++ -o test/temp TEST_temperature.cpp -lwiringPi