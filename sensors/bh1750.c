#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h> /* header chứa defines và hàm để tương tác với i2c chardev */
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

#define BH1750_ADDR     0x23
#define I2C_DEVICE      "/dev/i2c-1"

int main(int argc, char *argv[])
{
    int fd;
    int len;

    /* Open device file /dev/i2c-1 tương ứng với bus I2C1 */
    fd = open(I2C_DEVICE, O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "Failed to open '%s'\n", I2C_DEVICE);
        return -1;
    }

    /* Set địa chỉ 0x23 */
    if (ioctl(fd, I2C_SLAVE, BH1750_ADDR) < 0)
    {
        fprintf(stderr, "Failed to set address '%d'\n", BH1750_ADDR);
        return -1;
    }

    for (int i = 0; i < 20; ++i)
    {
        char cmd = 0b00100000; /* One Time H-Resolution Mode command */
        char buf[2];

        /* Gửi lệnh đến cảm biến bh1750 */
        len = write(fd, &cmd, sizeof(cmd));
        if (len < 0)
        {
            fprintf(stderr, "Failed to write command to bh1750\n");
            return -1;
        }

	 usleep(120000); // /* Chờ 120 ms */

        /* Đọc dữ liệu từ cảm biến bh1750 */
        len = read(fd, buf, 2);
        if (len < 0)
        {
            fprintf(stderr, "Failed to read data from bh1750\n");
            return -1;
        }

        /* Convert dữ liệu sáng lux */
        int lux = (buf[0] << 8 | buf[1] << 0) & 0xFFFF;
        printf("lux: %d\n", lux);

        usleep(500000); // 500 ms
    }
    
    return 0;
}

