#include "mpu6050_task.h"

Adafruit_MPU6050 mpu;
uint8_t mpu_num = 1;

MPU6050Task::MPU6050Task(const uint8_t task_core) 
    : Task<MPU6050Task>("MPU6050Task", 2500, 1, task_core) {
    mpu_queue = nullptr;  
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

void MPU6050Task::selectChannel(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}


void MPU6050Task::run() {
    Serial.println("MPU6050Task constructor");
    // Try to initialize!
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
        delay(10);
        }
    }
    Serial.println("MPU6050 Found!");

    // Initialize MPU6050s
    for (uint8_t i = 0; i < mpu_num ; i++)
    {
        selectChannel(i);
        setMPU6050();
    }
    
    Serial.println("");
    for (;;) {
        for (uint8_t i = 0; i < mpu_num ; i++)
        {
            printValues();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}


void MPU6050Task::setMPU6050()
{
        mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }
}


void MPU6050Task::printValues() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");

    Serial.print("Rotation X: ");
    Serial.print(g.gyro.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");

    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degC");

    Serial.println("");
}