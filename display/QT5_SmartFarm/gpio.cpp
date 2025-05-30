#include "gpio.h"

int Gpio::init(std::string consumerName, std::string chipName, int line)
{
    int ret;

    mChip = gpiod_chip_open_by_name(chipName.c_str());
	if (mChip == nullptr) {
		perror("Failed to open chip failed\n");
		return -1;
	}

    mLine = gpiod_chip_get_line(mChip, line);
    if (mLine == nullptr) {
        perror("Failed to get line failed\n");
        gpiod_chip_close(mChip);
        return -1;
    }

    ret = gpiod_line_request_output(mLine, consumerName.c_str(), 1);
	if (ret < 0) {
		perror("Request line as output failed\n");
        gpiod_line_release(mLine);
        gpiod_chip_close(mChip);
        return -1;
	}

    return 0;
}

void Gpio::deinit()
{
    gpiod_line_release(mLine);
    gpiod_chip_close(mChip);
}

int Gpio::setState(int state)
{
    int ret;

    ret = gpiod_line_set_value(mLine, state);
    if (ret < 0) {
        perror("Failed to set line value\n");
        return -1;
    }

    return 0;
}

int Gpio::getState()
{
    int ret;

    ret = gpiod_line_get_value(mLine);
    if (ret < 0) {
        perror("Failed to get line value\n");
        return -1;
    }

    return ret;
}
