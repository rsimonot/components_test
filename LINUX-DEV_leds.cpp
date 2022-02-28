#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <iostream>
#include <string>
#include <unistd.h>

#include "colorfullcpp.hpp"

int setupGpio7()
{
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/export" << N << std::endl;
        return -1;
    }
    if (write(fd, "7", 1) != 1 && access( "/sys/class/gpio/gpio7", F_OK ) == -1) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/export" << N << std::endl;
        return -2;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/export, or it's already done" << N << std::endl;
    close(fd);
    sleep(1);   // Stupid but it needs this time to acknowledge that /sys/class/gpio/gpio7 exists
                // Without any waiting time the actions bellow will fail

    // Set the pin to be an output by writing "out" to /sys/class/gpio/gpio7/direction
    fd = open("/sys/class/gpio/gpio7/direction", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/gpio7/direction" << N << std::endl;
        return -3;
    }
    if (write(fd, "out", 3) != 3) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/gpio7/direction" << N << std::endl;
        return -4;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/gpio7/direction" << N << std::endl;
    close(fd);
    return 0;
}

int setGpio7Low()
{
    // Open gpio7 value
    int fd = open("/sys/class/gpio/gpio7/value", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/gpio7/value" << N << std::endl;
        return -1;
    }
    if (write(fd, "0", 1) != 1) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/gpio7/value" << N << std::endl;
        return -2;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/gpio7/value" << N << std::endl;
    close(fd);
    return 0;
}

int setGpio7High()
{
    // Set GPIO value to "high" ("1")
    int fd = open("/sys/class/gpio/gpio7/value", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/gpio7/value" << N << std::endl;
        return -1;
    }
    if (write(fd, "1", 1) != 1) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/gpio7/value" << N << std::endl;
        return -2;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/gpio7/value" << N << std::endl;
    close(fd);
    return 0;
}

int cleanupGpio7()
{
    // Unexport gpio7
    int fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/unexport" << N << std::endl;
        return -1;
    }
    if (write(fd, "7", 2) != 2) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/unexport" << N << std::endl;
        return -2;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/unexport" << N << std::endl;
    close(fd);
    return 0;
}

int initializeLedModule(int i2c_file, unsigned char buffer[], int length)
{
    buffer[0] = 0x00; buffer[1] = 0x00;     // MODE1 to 0x00
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl;

    buffer[0] = 0x01; buffer[1] = 0x00;     // MODE2 to 0x00
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl;

    buffer[0] = 0x0c; buffer[1] = 0xff;     // LEDOUT0 to 0xff
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl;

    buffer[0] = 0x0d; buffer[1] = 0xff;     // LEDOUT1 to 0xff
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl;

    return 0;
}

int controlLEDs(int i2c_file, unsigned char buffer[], int length, int val)
{
    buffer[0]  = 0x02; buffer[1]  = val;     // LED 1
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl; sleep(1.2);

    buffer[0] = 0x03; buffer[1] = val;     // LED 2
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl; sleep(1.2);

    buffer[0] = 0x04; buffer[1] = val;     // LED 3
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl; sleep(1.2);

    buffer[0] = 0x05; buffer[1] = val;     // LED 4
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl; sleep(1.2);

    buffer[0] = 0x06; buffer[1] = val;     // LED 5
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl; sleep(1.2);

    buffer[0] = 0x07; buffer[1] = val;     // LED 6
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl;

    return 0;
}

int GRPPWM_leds(int i2c_file, unsigned char buffer[], int length)
{
    std::cout << P << "Brightness MAX" << N << std::endl;
    buffer[0] = 0x0a; buffer[1] = 0xff;     // 255/255
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -1;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl;
    sleep(2);

    std::cout << P << "Brightness MED" << N << std::endl;
    buffer[0] = 0x0a; buffer[1] = 0x64;     // 180/255
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -2;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl;
    sleep(2);
    
    std::cout << P << "Brightness OFF" << N << std::endl;
    buffer[0] = 0x0a; buffer[1] = 0x00;     // 0/255
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -3;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl;

    return 0;
}

int manageLed7(int i2c_file, unsigned char buffer[], int length)
{
    std::cout << P << "LED7 OFF..." << N << std::endl; sleep(1);
    buffer[0] = 0x08; buffer[1] = 0x00;     // 0/255
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -3;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl; sleep(3);

    std::cout << P << "LED7 ON..." << N << std::endl; sleep(1);
    buffer[0] = 0x08; buffer[1] = 0xff;     // 255/255
    if (write(i2c_file, buffer, length) != length) {
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
        return -3;
    }
    std::cout << G << "  [+]  Successfully written to the i2c bus" << N << std::endl;

    return 0;
}

int main (void)
{
    // Basic initialization for GPIO pin
    std::cout << P_B << "-> Initializing GPIO7" << N << std::endl;
    if (setupGpio7() < 0) { 
        std::cout << R_B << "  ERR : Can't initialize GPIO pin" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << P << "> Done" << N << std::endl;
    
    // Setting gpio7 pin to HIGH
    std::cout << P_B << "-> Setting GPIO7 to LOW" << N << std::endl;
    if (setGpio7Low() < 0) {
        std::cout << R_B << "  ERR : Can't manage GPIO pin" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << P << "> Done" << N << std::endl;

    int i2c_file, fd;
    int length = 2;
    unsigned char buffer[2] = {0};

    //----- Output of i2cdetect -l
    // i2c-1   i2c             bcm2835 (i2c@7e804000)                  I2C adapter
    // i2c-21  i2c             Broadcom STB :                          I2C adapter
    // i2c-20  i2c             Broadcom STB :                          I2C adapter
    // >> I'll use i2c-1
    //----- OPEN THE I2C BUS -----
    std::cout << P_B << "-> Opening the I2C bus" << N << std::endl;
	char *filename = (char*)"/dev/i2c-1";
	if ((i2c_file = open(filename, O_RDWR)) < 0)
	{
		std::cout << R_B << "  ERR : Failed to open the i2c bus" << N << std::endl;;
		return EXIT_FAILURE;
	}
    std::cout << P << "> Done" << N << std::endl;
	
	int addr = 0x15;    // PCA9624PW
	if (ioctl(i2c_file, I2C_SLAVE, addr) < 0)
	{
		std::cout << R_B << "  ERR : Failed to acquire bus access and/or talk to slave" << N << std::endl;
		return EXIT_FAILURE;
	}
	
    //  First, set up MODE1 and LEDOUT0/LEDOUT1
    std::cout << P_B << "-> Initializing with the right parameters..." << N << std::endl;
    if (initializeLedModule(i2c_file, buffer, length) < 0) {
        std::cout << R_B << "  ERR : Problem while enabling LEDs" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << P << "> Done" << N << std::endl; sleep(3);
    
    // Now controling the LEDs one by one
    std::cout << P_B << "-> Controling LEDs ..." << N << std::endl;
    if (controlLEDs(i2c_file, buffer, length, 0x50) < 0) {
        std::cout << R_B << "  ERR : Problem while enabling LEDs" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << P << "> Done" << N << std::endl; sleep(3);

    std::cout << P_B  << "-> Controling LED7 only" << N << std::endl;
    if (manageLed7(i2c_file, buffer, length) < 0) {
        std::cout << R_B << "  ERR : Unable to manage LED7" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << P << "> Done" << N << std::endl; sleep(3);
    /*
    // Lastly, control all the LEDs' brightness at the same time as it will be done with the whole prototype
    std::cout << P_B  << "-> Controling global brightness" << N << std::endl;
    if (GRPPWM_leds(i2c_file, buffer, length) < 0) {
        std::cout << R_B << "  ERR : Unable use GRPPWM" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << P << "> Done" << N << std::endl;
    */

    std::cout << P_B << "-> Controling LEDs ..." << N << std::endl;
    if (controlLEDs(i2c_file, buffer, length, 0x00) < 0) {
        std::cout << R_B << "  ERR : Problem while enabling LEDs" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << P << "> Done" << N << std::endl; sleep(2);

    // Finish
    std::cout << P_B << "-> All jobs done, exiting." << N << std::endl;

    std::cout << P_B << "-> Setting gpio7 to HIGH" << N << std::endl;
    setGpio7High();
    std::cout << P << "> Done" << N << std::endl;
    std::cout << P_B << "-> Unsetting GPIO7 ..." << N << std::endl;
    cleanupGpio7();
    std::cout << P << "> Done" << N << std::endl;

    close(i2c_file);
    return EXIT_SUCCESS;
}

// Compile with :
// g++ -o test/lleds LINUX-DEV_leds.cpp