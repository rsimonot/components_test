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
        return EXIT_FAILURE;
    }
    if (write(fd, "18", 2) != 2 && access( "/sys/class/gpio/gpio18", F_OK ) == -1) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/export" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/export, or it's already done" << N << std::endl;
    close(fd);
    sleep(1);   // Stupid but it needs this time to acknowledge that /sys/class/gpio/gpio18 exists
                // Without any waiting time the actions bellow will fail

    // Set the pin to be an output by writing "out" to /sys/class/gpio/gpio18/direction
    fd = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/gpio18/direction" << N << std::endl;
        return EXIT_FAILURE;
    }
    if (write(fd, "out", 3) != 3) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/gpio18/direction" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/gpio18/direction" << N << std::endl;
    close(fd);

    // Writting to gpio18
    fd = open("/sys/class/gpio/gpio18/value", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/gpio18/value" << N << std::endl;
        return EXIT_FAILURE;
    }
    if (write(fd, "1", 1) != 1) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/gpio18/value" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/gpio18/value" << N << std::endl;

    sleep(2.5);

    fd = open("/sys/class/gpio/gpio18/value", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/gpio18/value" << N << std::endl;
        return EXIT_FAILURE;
    }
    if (write(fd, "0", 1) != 1) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/gpio18/value" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/gpio18/value" << N << std::endl;

    // Unexport gpio18
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
        std::cout << R << "  [-]  Unable to open /sys/class/gpio/unexport" << N << std::endl;
        return EXIT_FAILURE;
    }
    if (write(fd, "18", 2) != 2) {
        std::cout << R << "  [-]  Error writing to /sys/class/gpio/unexport" << N << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << G << "  [+]  Opened and wrote to /sys/class/gpio/unexport" << N << std::endl;
    close(fd);


    return EXIT_SUCCESS;
}

// Compile with :
// g++ -o test/fan TEST_fan.cpp