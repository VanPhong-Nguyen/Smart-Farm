#include "light_sensor.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h> /* header chứa defines và hàm để tương tác với i2c chardev */
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

int LightSensor::init(std::string devicePath, int deviceAddress)
{
    /* Open device file /dev/i2c-1 tương ứng với bus I2C1 */
    mDeviceFd = open(devicePath.c_str(), O_RDWR);
    if (mDeviceFd < 0)
    {
        fprintf(stderr, "Failed to open '%s'\n", devicePath.c_str());
        return -1;
    }

    /* Set địa chỉ 0x23 */
    if (ioctl(mDeviceFd, I2C_SLAVE, deviceAddress) < 0)
    {
        fprintf(stderr, "Failed to set address '%d'\n", deviceAddress);
        return -1;
    }
    
    mDevicePath = devicePath;
    mDeviceAddress = deviceAddress;
    return 0;
}

void LightSensor::deinit()
{
    close(mDeviceFd);
}

int LightSensor::readLuxValue()
{
    char cmd = 0b00100000; /* One Time H-Resolution Mode command */
    char buf[2];
    int len;

    /* Gửi lệnh đến cảm biến bh1750 */
    len = write(mDeviceFd, &cmd, sizeof(cmd));
    if (len < 0)
    {
        fprintf(stderr, "Failed to write command to bh1750\n");
        return -1;
    }

    usleep(120000); // /* Chờ 120 ms */

    /* Đọc dữ liệu từ cảm biến bh1750 */
    len = read(mDeviceFd, buf, 2);
    if (len < 0)
    {
        fprintf(stderr, "Failed to read data from bh1750\n");
        return -1;
    }

    /* Convert dữ liệu sáng lux */
    int lux = (buf[0] << 8 | buf[1] << 0) & 0xFFFF;
    return lux;
}
