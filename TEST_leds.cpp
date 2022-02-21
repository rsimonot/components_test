#include "ADC.hpp"  // ADC functions, iostream, wiringPi.h, wiringPiI2C.h, bitset, colorfullcpp

#define OEpin 0    // OE pin, all LEDs at the same time, is on GPIO17, pin#50

int enableLEDs(int fd)
{
    // Structure of the sent data : 3 bits for auto-increment (always '000' -> no increment)
    // Then 5 bits for the register : LED0 to LED5
    int registers[6] = {0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    int i = 0;
    for (int reg : registers) {
        std::cout << P << "Enabling LED #" << ++i << "  Reg value = " << reg << N << std::endl;
        if (wiringPiI2CWriteReg8(fd, reg, 0xb4) < 0) {      // 180/255
            std::cout << R << "  [-] Problem for LED #" << i << N << std::endl;
            return -1;
        }
    }

    return 0;
}

int GRPPWM_leds(int fd, int GRPPWM_addr)
{
    std::cout << P << "Brightness MAX" << N << std::endl;
    if (wiringPiI2CWriteReg8(fd, GRPPWM_addr, 0xff) < 0)    // 255/255
        return -1;
    delay(1500);

    std::cout << P << "Brightness MED" << N << std::endl;
    if (wiringPiI2CWriteReg8(fd, GRPPWM_addr, 0x64) < 0)    // 100/255
        return -2;
    delay(1500);
    
    std::cout << P << "Brightness OFF" << N << std::endl;
    if (wiringPiI2CWriteReg8(fd, GRPPWM_addr, 0x00) < 0)    // 0/255
        return -3;
    delay(1500);

    return 0;
}


int main (void)
{
    wiringPiSetup();
    pinMode(OEpin, OUTPUT);
    digitalWrite(OEpin, LOW);      // enable LEDs by default
    int i2c_address_doc = 0x15;     // I2C Address of PCA9624PW
    int fd = wiringPiI2CSetup(i2c_address_doc);
    int ADC_address = detectADCDevice(i2c_address_doc);     // Just a confirmation for now, the function doesn't do that much job
    if (ADC_address < 0)
        return EXIT_FAILURE;

    //  First, set up MODE1 and LEDOUT0/LEDOUT1
    std::cout << P_B << "-> Initializing with the right parameters..." << N << std::endl;
    wiringPiI2CWriteReg8(fd, 0x00, 0x00);
    wiringPiI2CWriteReg8(fd, 0x0c, 0xff);
    wiringPiI2CWriteReg8(fd, 0x0d, 0xff);
    std::cout << P << "> Done" << N << std::endl;
    
    // Now controling the LEDs one by one
    std::cout << P_B << "-> Enabling LEDs ..." << std::endl;
    if (enableLEDs(fd) != 0) {
        std::cout << R_B << "ERR : Problem while enabling LEDs" << N << std::endl;
        return EXIT_FAILURE;
    }

    // Lastly, control all the LEDs' brightness at the same time as it will be done with the whole prototype
    std::cout << P_B  << "-> Controling global brightness" << N << std::endl;
    int GRPPWM_addr = 0x0a;
    if (GRPPWM_leds(fd, GRPPWM_addr) < 0) {
        std::cout << R_B << "ERR : Unable use GRPPWM" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+]  Global brightness OK !" << std::endl; delay(1000);

    // Finish

    std::cout << P_B << "-> All jobs done, exiting." << N << std::endl;
    digitalWrite(OEpin, HIGH);      // turn LEDs off
    return EXIT_SUCCESS;
}

// Compile with :
// g++ -o test/leds TEST_leds.cpp -lwiringPi