#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h> /* header chứa defines và hàm để tương tác với i2c chardev */
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

#define SHT30_ADDR      0x44 /* I2C address of the SHT30 sensor */

/* Hàm tính CRC8 */
unsigned char crc8(const unsigned char *data, size_t len)
{
    unsigned char crc = 0xFF; 
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31; 
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

int main(int argc, char *argv[])
{
    char *devicePath = "/dev/i2c-1";
    int ret = 0;

	/* Open device file /dev/i2c-1 tương ứng với bus I2C1 */
    int fd = open(devicePath, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the I2C device");
        return -1;
    }

	/* Set địa chỉ 0x23 */
    if (ioctl(fd, I2C_SLAVE, SHT30_ADDR) < 0) {
        perror("Failed to set the I2C address");
        ret = -1;
        goto cleanup;
    }

	
    for (int i = 0; i < 10; ++i) 
    {
	/* Gửi lệnh đến cảm biến */
        static char oneShotCmd[2] = {0x24, 0x00}; 
        if (write(fd, oneShotCmd, sizeof(oneShotCmd)) != sizeof(oneShotCmd)) {
            perror("Failed to write the command");
            ret = -1;
            goto cleanup;
        }
        usleep(50000); /* Đợi cho cảm biến đo hoàn tất */

	/* Đọc 6 bytes dữ liệu  */
        char data[6];
        if (read(fd, data, sizeof(data)) != sizeof(data)) {
            perror("Failed to read the data");
            ret = -1;
            goto cleanup;
        }

        /* Kiểm tra CRC */
        unsigned char crc1 = data[2];
        unsigned char crc2 = data[5];
        unsigned char crc1Calculated = crc8(data, 2);
        unsigned char crc2Calculated = crc8(data + 3, 2);
        if (crc1 != crc1Calculated || crc2 != crc2Calculated) {
            fprintf(stderr, "CRC check failed\n");
            continue;
        }

	/* Chuyển đổi dữ liệu thành nhiệt độ và độ ẩm */
        unsigned int rawTemp = (data[0] << 8) | data[1];
        unsigned int rawHum = (data[3] << 8) | data[4];
        float temperature = -45 + 175 * (float)rawTemp / 65535;
        float humidity = 100 * (float)rawHum / 65535;
        
        printf("Temperature: %.2f °C\n", temperature);
        printf("Humidity: %.2f %%\n", humidity);
    }

cleanup:
    close(fd);
    return ret;
}

