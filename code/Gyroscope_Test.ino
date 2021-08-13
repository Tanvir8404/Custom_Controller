// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Keyboard.h>
#include <Mouse.h>

Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp;

int caseCount = 0;
// int currentCase = (caseCount % 2);

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

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

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
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

  // int caseCount = 0;
  // int currentCase = caseCount % 2;

  pinMode(10, OUTPUT);

  Keyboard.begin();
  Mouse.begin();

  Serial.println("");
  delay(1000);
}

/* int accelerometerX = round(a.acceleration.x) + 2;
  int accelerometerY = round(a.acceleration.y);
  int accelerometerZ = round(a.acceleration.z);
  int gyroX = round(g.gyro.x * (180 / PI)) + 2;
  int gyroY = round(g.gyro.y * (180 / PI));
  int gyroZ = round(g.gyro.z * (180 / PI)); */

void loop() {

  int currentCase = (caseCount % 2);

  // TEST:

  serialData();
  if((round(a.acceleration.z) - 10) > 8) {
    caseCount++;
    delay(200);
  }
  if(currentCase == 0) {
    keyboardMotion();
    // digitalWrite(10, HIGH);
  }
  if(currentCase == 1) {
    mouseMotion();
    // digitalWrite(10, LOW);
  }

}

void serialData() {

  int currentCase = (caseCount % 2);

  /* Get new sensor events with the readings */
  // sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(round(a.acceleration.x) + 2);
  Serial.print(", Y: ");
  Serial.print(round(a.acceleration.y));
  Serial.print(", Z: ");
  Serial.print(round(a.acceleration.z) - 10);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(round(g.gyro.x * (180 / PI)) + 2);
  Serial.print(", Y: ");
  Serial.print(round(g.gyro.y * (180 / PI)));
  Serial.print(", Z: ");
  Serial.print(round(g.gyro.z * (180 / PI)) - 1);
  Serial.println(" deg/s");

  Serial.print("Temperature: ");
  Serial.print(round(temp.temperature * (9 / 5) + 32));
  Serial.println(" degF");

  Serial.print("currentCase = ");
  Serial.println(currentCase);

  Serial.print("caseCount = ");
  Serial.println(caseCount);
  delay(17);

}

void keyboardMotion() {

  if ((round(a.acceleration.x) + 2) < -4) {
    Keyboard.press('s');
  } else {
    Keyboard.release('s');
  }

  if ((round(a.acceleration.x) + 2) > 4) {
    Keyboard.press('w');
  } else {
    Keyboard.release('w');
  }

  if (round(a.acceleration.y) < -4) {
    Keyboard.press('d');
  } else {
    Keyboard.release('d');
  }

  if (round(a.acceleration.y) > 4) {
    Keyboard.press('a');
  } else {
    Keyboard.release('a');
  }

  /* if ((round(a.acceleration.z) - 10) > 6) {
    Keyboard.press((char) 32);
    } else {
    Keyboard.release((char) 32);
    } */

}

void mouseMotion() {

  if (round(a.acceleration.y) > 4) {
    Mouse.move(-2.5 * round(a.acceleration.y), 0, 0);
  }

  if (round(a.acceleration.y) < -4) {
    Mouse.move(-2.5 * round(a.acceleration.y), 0, 0);
  }

  if ((round(a.acceleration.x) + 2) > 4) {
    Mouse.move(0, -2.5 * round(a.acceleration.x), 0);
  }

  if ((round(a.acceleration.x) + 2) < -4) {
    Mouse.move(0, -2.5 * round(a.acceleration.x), 0);
  }w

}
