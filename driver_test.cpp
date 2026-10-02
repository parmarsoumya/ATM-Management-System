#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main()
{
    int fd = open("/dev/atm_device", O_RDWR);

    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    std::cout << "ATM device opened successfully." << std::endl;

    const char message[] = "ATM TEST";

    ssize_t written = write(fd, message, strlen(message));

    if (written < 0)
    {
        perror("write");
        close(fd);
        return 1;
    }

    std::cout << "Data sent to driver: "
              << written << " bytes" << std::endl;

    close(fd);

    std::cout << "ATM device closed." << std::endl;

    return 0;
}
