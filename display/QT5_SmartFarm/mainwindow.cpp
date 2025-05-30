#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define LIGHT_SENSOR_DEVICE_PATH                "/dev/i2c-1"
#define LIGHT_SENSOR_DEVICE_ADDRESS             0x23

#define TEMPERATURE_HUMIDITY_DEVICE_PATH        "/dev/i2c-1"
#define TEMPERATURE_HUMIDITY_DEVICE_ADDRESS     0x44

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(&mReadSensorDataTimer, &QTimer::timeout, [&]() {
        TemperatureHumiditySensor::CelsiusHumidityValue celsiusHumidityValue;
        int luxValue;

        luxValue = mLightSensor.readLuxValue();
        celsiusHumidityValue = mTemperatureHumiditySensor.readCelsiusHumidityValue();

        ui->temperatureLabel->setText(QString("%1°C").arg((int)celsiusHumidityValue.temperature));
        ui->humidityLabel->setText(QString("%1%").arg((int)celsiusHumidityValue.humidity));
        ui->luxLabel->setText(QString("%1lx").arg(luxValue));
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::init()
{
    int ret;

    ret = mLightSensor.init(LIGHT_SENSOR_DEVICE_PATH, LIGHT_SENSOR_DEVICE_ADDRESS);
    if (ret < 0)
    {
        printf("Failed to init light sensor\n");
        return -1;
    }

    ret = mTemperatureHumiditySensor.init(TEMPERATURE_HUMIDITY_DEVICE_PATH, TEMPERATURE_HUMIDITY_DEVICE_ADDRESS);
    if (ret < 0)
    {
        printf("Failed to init temperature humidity sensor\n");
        return -1;
    }

    ret = mLightGpio.init("light", "gpiochip2", 2); /* P8_7 pin */
    if (ret < 0)
    {
        printf("Failed to init light GPIO\n");
        return -1;
    } 
    else 
    {
        int state = mLightGpio.getState();
        if (state == GPIO_HIGH)
        {
            ui->lightButton->setStyleSheet("background-color: green;");
        }
        else if (state == GPIO_LOW)
        {
            ui->lightButton->setStyleSheet("background-color: gray;");
        }
    }

    ret = mPumpGpio.init("pump", "gpiochip2", 3); /* P8_8 pin */
    if (ret < 0)
    {
        printf("Failed to init pump GPIO\n");
        return -1;
    }
    else 
    {
        int state = mPumpGpio.getState();
        if (state == GPIO_HIGH)
        {
            ui->pumpButton->setStyleSheet("background-color: green;");
        }
        else if (state == GPIO_LOW)
        {
            ui->pumpButton->setStyleSheet("background-color: gray;");
        }
    }

    mReadSensorDataTimer.start(3000); // every 3000 ms
    return 0;
}

void MainWindow::on_lightButton_clicked()
{
    int state = mLightGpio.getState();
    if (state == GPIO_HIGH)
    {
        mLightGpio.setState(GPIO_LOW);
        ui->lightButton->setStyleSheet("background-color: gray;");
    }
    else
    {
        mLightGpio.setState(GPIO_HIGH);
        ui->lightButton->setStyleSheet("background-color: green;");
    }
}

void MainWindow::on_pumpButton_clicked()
{
    int state = mPumpGpio.getState();
    if (state == GPIO_HIGH)
    {
        mPumpGpio.setState(GPIO_LOW);
        ui->pumpButton->setStyleSheet("background-color: gray;");
    }
    else
    {
        mPumpGpio.setState(GPIO_HIGH);
        ui->pumpButton->setStyleSheet("background-color: green;");
    }
}
