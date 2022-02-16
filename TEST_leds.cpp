#include "ADC.hpp"  // ADC functions, iostream, wiringPi.h, wiringPiI2C.h, bitset, colorfullcpp

#define OEpin 50    // OE pin, all LEDs at the same time, is on GPIO17, pin#50

int flashLEDs(int ADC_address)
{
    int fd = wiringPiI2CSetup(ADC_address);
    // Structure of the sent data : 3 bits for auto-increment (always '000' -> no increment)
    // Then 5 bits for the register : LED0 to LED5
    int registers[6] = {00000010, 00000011, 00000100, 00000101, 00000110, 00000111};
    int i = 0;
    for (int reg : registers) {
        std::cout << P << "Flashing LED #" << ++i << "Reg value = " << reg << N << std::endl;
        if (wiringPiI2CWriteReg8(fd, reg, 11110000) < 0)
            return -1;
        delay(800);
        wiringPiI2CWriteReg8(fd, reg, 00000000);
        delay(1500);
    }

    return 0;
}

int main (void)
{
    //  First, use the OE pin to light up all the LEDs together one  time
    //  Or turn them all down if they're already up
    std::cout << P_B << "-> Lighting all the LEDs at the same time" << N << std::endl;
    wiringPiSetup();
    pinMode(OEpin, OUTPUT);
    digitalWrite(OEpin, LOW);
    delay(1000);
    digitalWrite(OEpin, HIGH);

    // Now controling all the LEDs at the same time
    int i2c_address_doc = 0x15;    // I2C Address of PCA9624PW 
    int ADC_address = detectADCDevice(i2c_address_doc);     // Just a confirmation for now, the function doesn't do that much job
    if (ADC_address < 0)
        return EXIT_FAILURE;

    std::cout << P_B << "-> Flashing LEDs in" << P << " 3 ... " << std::endl;
    delay(800);
    std::cout << "... 2 ..." << std::endl;
    delay(800);
    std::cout << "... 1 ..." << N << std::endl;
    delay(800);
    if (flashLEDs(ADC_address) != 0) {
        std::cout << R_B << "ERR : " << R << "Unable to flash LEDs" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+]  All LEDs flashed !" << std::endl;

    // Lastly, control all the LEDs' brightness at the same time as it will be done with the whole prototype


    std::cout << P_B << "-> All jobs done, exiting." << N << std::endl;

    return EXIT_SUCCESS;
}

// Compile with :
// g++ -o test/leds TEST_leds.cpp -lwiringPi