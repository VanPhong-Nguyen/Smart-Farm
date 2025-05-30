#ifndef __LIGHT_SENSOR_H__
#define __LIGHT_SENSOR_H__

#include <string>

class LightSensor {
    public:
        LightSensor() = default;

        ~LightSensor() = default;

        int init(std::string devicePath, int deviceAddress);

        void deinit();

        int readLuxValue();

    private:
        int mDeviceFd;
        std::string mDevicePath;
        int mDeviceAddress;
};

#endif // __LIGHT_SENSOR_H__
