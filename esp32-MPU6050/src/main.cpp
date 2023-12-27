#include <Arduino.h>
#include "mpu6050_task.h" 

static MPU6050Task mpu6050_task(0);

void setup(void) {
  vTaskDelay(pdMS_TO_TICKS(10));
  Serial.begin(115200);
  Serial.println("sensoring glove begin...");

  mpu6050_task.begin();

 vTaskDelete(NULL);

}

void loop() {

}