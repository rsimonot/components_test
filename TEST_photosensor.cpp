#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "colorfullcpp.hpp"

int main (void)
{
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/export" << N << std::endl;
        return -1;
    }
    if (write(fd, "6", 1) != 1 && access( "/sys/class/gpio/gpio6", F_OK ) == -1) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/export" << N << std::endl;
        return -2;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/export, or it's already done" << N << std::endl;
    close(fd);
    sleep(1);   // Stupid but it needs this time to acknowledge that /sys/class/gpio/gpio6 exists
                // Without any waiting time the actions bellow will fail

    // Set the pin to be an output by writing "out" to /sys/class/gpio/gpio6/direction
    fd = open("/sys/class/gpio/gpio6/direction", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/gpio6/direction" << N << std::endl;
        return -3;
    }
    if (write(fd, "in", 3) != 3) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/gpio6/direction" << N << std::endl;
        return -4;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/gpio6/direction" << N << std::endl;
    close(fd);

    // Reading from gpio6
    fd = open("/sys/class/gpio/gpio6/value", O_RDONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/gpio6/value" << N << std::endl;
        return -1;
    }
    char read_buffer[1];
    if (-1 == read(fd, read_buffer, 1)) {
        std::cout << R << "  [-]  Error reading from /sys/class/gpio/gpio6/value" << N << std::endl;
        return -2;
    }
    std::cout << G << "  [+]  Successfully read from /sys/class/gpio/gpio6/value" << N << std::endl;
    close(fd);
    std::string res = atoi(read_buffer) == 0 ? "Stuff detected" : "Nothing detected";
    std::cout << P << "> Read value : " << res << N << std::endl;

    // Unexport gpio6
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/unexport" << N << std::endl;
        return -1;
    }
    if (write(fd, "6", 2) != 2) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/unexport" << N << std::endl;
        return -2;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/unexport" << N << std::endl;
    close(fd);


    return EXIT_SUCCESS;
}

// Compile with :
// g++ -o test/photosensor TEST_photosensor.cpp