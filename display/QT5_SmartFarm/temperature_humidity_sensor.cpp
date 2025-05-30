#include "temperature_humidity_sensor.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h> /* header chứa defines và hàm để tương tác với i2c chardev */
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

int  TemperatureHumiditySensor::init(std::string devicePath, int deviceAddress)
{
    /* Open device file /dev/i2c-1 tương ứng với bus I2C1 */
    mDeviceFd = open(devicePath.c_str(), O_RDWR);
    if (mDeviceFd < 0) {
        perror("Failed to open the I2C device");
        return -1;
    }

	/* Set địa chỉ 0x23 */
    if (ioctl(mDeviceFd, I2C_SLAVE, deviceAddress) < 0) {
        perror("Failed to set the I2C address");
        close(mDeviceFd);
        return -1;
    }

    mDevicePath = devicePath;
    mDeviceAddress = deviceAddress;
    return 0;
}

void TemperatureHumiditySensor::deinit()
{
    close(mDeviceFd);
}

TemperatureHumiditySensor::CelsiusHumidityValue TemperatureHumiditySensor::readCelsiusHumidityValue()
{
    /* Gửi lệnh đến cảm biến */
    static char oneShotCmd[2] = {0x24, 0x00}; 
    if (write(mDeviceFd, oneShotCmd, sizeof(oneShotCmd)) != sizeof(oneShotCmd)) {
        perror("Failed to write the command");
        return CelsiusHumidityValue{0, 0};
    }
    usleep(50000); /* Đợi cho cảm biến đo hoàn tất */

	/* Đọc 6 bytes dữ liệu  */
    unsigned char data[6];
    if (read(mDeviceFd, data, sizeof(data)) != sizeof(data)) {
        perror("Failed to read the data");
        return CelsiusHumidityValue{0, 0};
    }

    /* Kiểm tra CRC */
    unsigned char crc1 = data[2];
    unsigned char crc2 = data[5];
    unsigned char crc1Calculated = computeCrc8(data, 2);
    unsigned char crc2Calculated = computeCrc8(data + 3, 2);
    if (crc1 != crc1Calculated || crc2 != crc2Calculated) {
        fprintf(stderr, "CRC check failed\n");
        return CelsiusHumidityValue{0, 0};
    }

    /* Chuyển đổi dữ liệu thành nhiệt độ và độ ẩm */
    unsigned int rawTemp = (data[0] << 8) | data[1];
    unsigned int rawHum = (data[3] << 8) | data[4];
    float temperature = -45 + 175 * (float)rawTemp / 65535;
    float humidity = 100 * (float)rawHum / 65535;

    return CelsiusHumidityValue{temperature, humidity};
}


unsigned char TemperatureHumiditySensor::computeCrc8(unsigned char *data, unsigned char len)
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
