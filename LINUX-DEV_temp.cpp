#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <iostream>
#include <string>

#include "colorfullcpp.hpp"

int main (void)
{
    int i2c_file, length;
    unsigned char buffer[60] = {0};

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
		std::cout << R_B << "ERR : Failed to open the i2c bus" << N << std::endl;;
		return EXIT_FAILURE;
	}
    std::cout << G << "  [+]  Done" << N << std::endl;
	
	int addr = 0x48;    // TMP116
	if (ioctl(i2c_file, I2C_SLAVE, addr) < 0)
	{
		std::cout << R_B << "ERR : Failed to acquire bus access and/or talk to slave" << N << std::endl;
		return EXIT_FAILURE;
	}
	
	//----- WRITE BYTES -----
    std::cout << P_B << "-> Writting bytes" << N << std::endl;
	buffer[0] = 0x00;   // Temperature register
	length = 1;			// Number of bytes to write
	if (write(i2c_file, buffer, length) != length)
		std::cout << R << "  [-]  Failed to write to the i2c bus" << N << std::endl;
	std::cout << G << "  [+]  Done" << N << std::endl;

	//----- READ BYTES -----
    std::cout << P_B << "-> Reading bytes" << N << std::endl;
	length = 2;			// Number of bytes to read
	if (read(i2c_file, buffer, length) != length)
		std::cout << R << "  [-]  Failed to read from the i2c bus" << N << std::endl;
	else
        std::cout << G << "  [+]  Done" << N << std::endl;

    int _temp = *(unsigned int *)buffer;
    float temp = static_cast<int16_t>( _temp>>8|((_temp<<8)&0xffff)) * 0.0078125;
    std::cout << P << "Data read : " << _temp << " which makes : " << temp << "°C" << N << std::endl;

    close(i2c_file);
    return EXIT_SUCCESS;
}

// Compile with :
// g++ -o test/ltemp LINUX-DEV_temp.cpp