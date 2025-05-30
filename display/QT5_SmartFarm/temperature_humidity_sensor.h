#ifndef __TEMPERATURE_HUMIDITY_SENSOR_H__
#define __TEMPERATURE_HUMIDITY_SENSOR_H__

#include <string>

class TemperatureHumiditySensor {
    public:
        typedef struct {
            float temperature;
            float humidity;
        } CelsiusHumidityValue;

        TemperatureHumiditySensor() = default;

        ~TemperatureHumiditySensor() = default;

        int init(std::string devicePath, int deviceAddress);

        void deinit();

        CelsiusHumidityValue readCelsiusHumidityValue();

    private:
        int mDeviceFd;
        std::string mDevicePath;
        int mDeviceAddress;

        unsigned char computeCrc8(unsigned char *data, unsigned char len);
};


#endif // __TEMPERATURE_HUMIDITY_SENSOR_H__