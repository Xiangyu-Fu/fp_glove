#include "mpu6050_task.h"
#include "Adafruit_MPU6050.h"

// Adafruit_MPU6050 mpu;

MPU6050Task::MPU6050Task(const uint8_t task_core) 
    : Task<MPU6050Task>("MPU6050Task", 8192, task_core) {
    // Initialize the MPU6050 sensor
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!");
}

MPU6050Task::~MPU6050Task() {
    vQueueDelete(mpu_queue);
}

void MPU6050Task::addListener(QueueHandle_t queue) {
    mpu_queue = queue;
}

void MPU6050Task::setLogger(Logger* logger) {
    logger_ = logger;
}

void MPU6050Task::publish(sensors_event_t aEvent) {
    if (mpu_queue != nullptr) {
        // Send data to the queue
        xQueueSend(mpu_queue, &aEvent, portMAX_DELAY);
    }
}

void MPU6050Task::run() {
    // while (true) {
    //     sensors_event_t accel;
    //     mpu.getEvent(&accel);

    //     // Log the data if logger is set
    //     if (logger_) {
    //         logger_->log(accel);
    //     }

    //     // Publish the data
    //     publish(accel);

    //     // Delay to control the frequency of readings
    //     vTaskDelay(100 / portTICK_PERIOD_MS);
    // }
}
