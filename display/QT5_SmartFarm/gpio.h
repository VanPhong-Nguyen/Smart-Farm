#ifndef __GPIO_H__
#define __GPIO_H__

#include <gpiod.h>

#include <string>

#define GPIO_LOW 0
#define GPIO_HIGH 1

class Gpio
{
    public:
        Gpio() = default;

        ~Gpio() = default;

        int init(std::string consumerName, std::string chipName, int line);

        void deinit();

        int setState(int state);

        int getState();

    private:
        struct gpiod_chip *mChip;
        struct gpiod_line *mLine;
};

#endif // __GPIO_H__
