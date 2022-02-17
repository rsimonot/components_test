#include "ADC.hpp"  // ADC functions, iostream, wiringPi.h, wiringPiI2C.h, bitset, colorfullcpp

int main (void)
{
    int i2c_address_doc = 0x51;    // I2C Address of TMP116N
    int ADC_address = detectADCDevice(i2c_address_doc);
    if (ADC_address < 0)
        return EXIT_FAILURE;

    int year_reg = 0x0a, month_reg = 0x09, days_reg = 0x07, hours_reg = 0x06, min_reg = 0x05, sec_reg = 0x04;
    int fd = wiringPiI2CSetup(ADC_address);

    std::cout << P_B << "-> Retrieving current time" << N << std::endl;
    wiringPiI2CWrite(fd, year_reg);
    int Y = wiringPiI2CRead(fd);
    if (Y < 0) {
        std::cout << R_B << "ERR : Unable to read from RTC Device" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+] Successfully read Year from RTC Device !" << N << std::endl;

    wiringPiI2CWrite(fd, month_reg);
    int M = wiringPiI2CRead(fd);
    std::cout << G << "  [+] Successfully read Month from RTC Device !" << N << std::endl;

    wiringPiI2CWrite(fd, days_reg);
    int D = wiringPiI2CRead(fd);
    std::cout << G << "  [+] Successfully read Day from RTC Device !" << N << std::endl;

    wiringPiI2CWrite(fd, hours_reg);
    int HRS = wiringPiI2CRead(fd);
    std::cout << G << "  [+] Successfully read Hours from RTC Device !" << N << std::endl;

    wiringPiI2CWrite(fd, min_reg);
    int MIN = wiringPiI2CRead(fd);
    std::cout << G << "  [+] Successfully read Minutes from RTC Device !" << N << std::endl;

    wiringPiI2CWrite(fd, sec_reg);
    int SEC = wiringPiI2CRead(fd);
    std::cout << G << "  [+] Successfully read Seconds from RTC Device !" << N << std::endl;

    std::cout << P_B << "Current time according to RTC =>  " << P
        << D << "/" << M << "/" << Y << "  -  "
        << HRS << ":" << MIN << ":" << SEC
        << N << std::endl;

    return EXIT_SUCCESS;
}

// Compile with :
// g++ -o test/rtc TEST_rtc.cpp -lwiringPi