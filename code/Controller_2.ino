#define DECODE_NEC          // Includes Apple and Onkyo

#include <Arduino.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <IRremote.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <Wire.h>

// Receiver

Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp;

int caseCount = 0;
int stickPresses = 0;

unsigned long onTimer = 0;
unsigned long currentTime = 0;
const long interval = 3000;

int voltSense = (digitalRead(5) == LOW || digitalRead(6) == LOW || digitalRead(7) == LOW || digitalRead(8) == LOW); // determines if any keys should be pressed

// set pin numbers for switch, joystick axes, and LED:
const int mouseButton = 9;   // input pin for the mouse pushButton
const int stickButton = 8;
const int xAxis = A1;         // joystick X axis
const int yAxis = A0;         // joystick Y axis

// parameters for reading the joystick:
int range = 10;               // output range of X or Y movement
int responseDelay = 2;        // response delay of the mouse, in ms
int threshold = 1;            // resting threshold
int center = range / 2;       // resting position value

int mapX;
int mapY;

int mapRed = 0;
int mapGreen = 0;
int mapBlue = 0;

float sensMult;
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
int red_light_pin = 10;
int green_light_pin = 11;
int blue_light_pin = 12;

void setup() {

  Serial.begin(115200);

  Keyboard.begin();
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  Mouse.begin();
  pinMode(mouseButton, INPUT_PULLUP);
  pinMode(stickButton, INPUT_PULLUP);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

  while (!Serial)
    delay(10); // will pause Zero, Leonardo, atc until serial console opens

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

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  /*
     Start the receiver, enable feedback LED and take LED feedback pin from the internal boards definition
  */
  IrReceiver.begin(13, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);

  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(13);

}

void loop() {

  int currentCase = (caseCount % 2);
  int currentStick = (stickPresses % 3);

  unsigned long IRtimer = millis();

  if (IrReceiver.decode()) {

    // Print a short summary of received data
    // IrReceiver.printIRResultShort(&Serial);
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      // We have an unknown protocol here, print more info
      // IrReceiver.printIRResultRawFormatted(&Serial, true);
    }
    Serial.println();
    /*
       !!!Important!!! Enable receiving of the next value,
       since receiving has stopped after the end of the current received data packet.
    */
    IrReceiver.resume(); // Enable receiving of the next value

    /*
       Finally, check the received data and perform actions according to the received command
    */
    if (IrReceiver.decodedIRData.command == 0x34) {
      currentTime = IRtimer;
      if ((round(a.acceleration.z) - 10) > 3) {
        caseCount++;
      }
      // digitalWrite(10, LOW);
      serialData();
      if (currentCase == 0) {
        keyboardMotion();
      }
      if (currentCase == 1) {
        mouseMotion();
      }
    }

    if (IrReceiver.decodedIRData.command == 0x43) {
      currentTime = IRtimer;
      if (digitalRead(stickButton) == LOW) {
        stickPresses++;
        delay(200);
      }
      if (currentStick == 0) {
        mapRed = map(analogRead(A1), 0, 1023, 0, 255);
      }
      if (currentStick == 1) {
        mapGreen = map(analogRead(A1), 0, 1023, 0, 255);
      }
      if (currentStick == 2) {
        mapBlue = map(analogRead(A1), 0, 1023, 0, 255);
      }
    }
  }

  if (IrReceiver.decodedIRData.command != 0x43) {

    triggerButtons();
    triggerCursor();
    triggerJoystick();

  }

  if (!IrReceiver.decode() && IRtimer - currentTime >= interval) {
    Serial.println("No data.");
  }
  /*
    triggerButtons();
    triggerCursor();
    triggerJoystick();
  */

  RGB_color(mapRed, mapGreen, mapBlue);
  Serial.println(IrReceiver.decodedIRData.command, HEX);
  Serial.print("Red ");
  Serial.println(mapRed);
  Serial.print("Green ");
  Serial.println(mapGreen);
  Serial.print("Blue ");
  Serial.println(mapBlue);
  Serial.print("currentStick ");
  Serial.println(currentStick);
  Serial.print("stickPresses ");
  Serial.println(stickPresses);

}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value) {

  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);

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
  // delay(17);

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
  }

}

void triggerButtons() {

  if (digitalRead(5) == LOW) {
    Keyboard.press(KEY_LEFT_ARROW);
  } else {
    Keyboard.release(KEY_LEFT_ARROW);
  }
  if (digitalRead(6) == LOW) {
    Keyboard.press(KEY_DOWN_ARROW);
  } else {
    Keyboard.release(KEY_DOWN_ARROW);
  }
  if (digitalRead(7) == LOW) {
    Keyboard.press(KEY_UP_ARROW);
  } else {
    Keyboard.release(KEY_UP_ARROW);
  }
  if (digitalRead(4) == LOW) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.release(KEY_RIGHT_ARROW);
  }

}

float MouseReadAxis(int thisAxis) {

  // read the analog input:
  float reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = mapfloat(reading, 0, 1023, 0, range);
  sensMult = mapfloat(analogRead(A4), 0, 1023, 1, 5);

  // if the output reading is outside from the rest position threshold, use it:
  float distance = reading - center;
  float newDistance;

  if (abs(distance) < threshold) {
    newDistance = 0;
  } else {
    if (distance < 0) {
      newDistance = (-0.35 * sensMult) * pow(distance, 2);
    } else {
      newDistance = (0.35 * sensMult) * pow(distance, 2);
    }
  }

  // Serial.println(newDistance);
  return newDistance;

}

void triggerCursor() {

  // read and scale the two axes:
  float xReading = MouseReadAxis(A3);
  float yReading = MouseReadAxis(A2);

  Mouse.move(xReading, yReading, 0);

  if (digitalRead(mouseButton) == LOW) {
    Keyboard.press((char) 32);
    delay(2);
  } else {
    Keyboard.release((char) 32);
  }

}

void triggerJoystick() {

  mapX = mapfloat(analogRead(A1), 0, 1023, -4, 4);
  mapY = mapfloat(analogRead(A0), 0, 1023, 4, -4);

  if (mapX <= -3) {
    Keyboard.press('a');
  } else {
    Keyboard.release('a');
  }
  if (mapX >= 3) {
    Keyboard.press('d');
  } else {
    Keyboard.release('d');
  }
  if (mapY <= -3) {
    Keyboard.press('s');
  } else {
    Keyboard.release('s');
  }
  if (mapY >= 3) {
    Keyboard.press('w');
  } else {
    Keyboard.release('w');
  }

  // read the mouse button and click or not click:
  if (digitalRead(stickButton) == LOW) {
    if (!Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.press(MOUSE_LEFT);
    }
  } else {
    if (Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.release(MOUSE_LEFT);
    }
  }

}
