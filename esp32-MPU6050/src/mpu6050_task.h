#ifndef __MPU6050_TASK_H__
#define __MPU6050_TASK_H__

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "task.h"

class MPU6050Task : public Task<MPU6050Task> {
    friend class Task<MPU6050Task>;  // Allow base Task to invoke our run() function

public:
    MPU6050Task(const uint8_t task_core);
    ~MPU6050Task();

    void addListener(QueueHandle_t queue);
    void setLogger(Logger* logger);
    void publish();

protected:
    void run();

private:
    Adafruit_MPU6050 mpu;
    QueueHandle_t mpu_queue;
    Logger* logger_;
    sensors_event_t a, g, temp;
    uint32_t mpu_last_publish;
    uint32_t mpu_publish_interval;
};

#endif