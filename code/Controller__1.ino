#include <Keyboard.h>
#include <Mouse.h>

int voltSense = (digitalRead(5) == LOW || digitalRead(6) == LOW || digitalRead(7) == LOW || digitalRead(8) == LOW); // determines if any keys should be pressed
int voltWrite = digitalRead(5);

// set pin numbers for switch, joystick axes, and LED:
// const int switchPin = 2;      // switch to turn on and off mouse control
const int mouseButton = 9;   // input pin for the mouse pushButton
const int stickButton = 8;
const int xAxis = A1;         // joystick X axis
const int yAxis = A0;         // joystick Y axis

// parameters for reading the joystick:
int range = 16;               // output range of X or Y movement
int responseDelay = 2;        // response delay of the mouse, in ms
int threshold = 2;            // resting threshold
int center = range / 2;       // resting position value

int mapX = 0;
int mapY = 0;

int mode = digitalRead(2);
int modeStatus = 0;

void setup() {

  Serial.begin(9600);

  Keyboard.begin();
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  Mouse.begin();
  pinMode(mouseButton, INPUT_PULLUP);
  pinMode(stickButton, INPUT_PULLUP);

}

void loop() {

  Serial.println(voltWrite);

  triggerKeyboard();
  triggerMouse();
  triggerJoystick();
  
}

void triggerKeyboard() {

  if (digitalRead(7) == LOW) {
    Keyboard.press(KEY_LEFT_ARROW);
  } else {
    Keyboard.release(KEY_LEFT_ARROW);
    delay(2);
  }
  if (digitalRead(6) == LOW) {
    Keyboard.press(KEY_DOWN_ARROW);
  } else {
    Keyboard.release(KEY_DOWN_ARROW);
    delay(2);
  }
  if (digitalRead(5) == LOW) {
    Keyboard.press(KEY_UP_ARROW);
  } else {
    Keyboard.release(KEY_UP_ARROW);
    delay(2);
  }
  if (digitalRead(4) == LOW) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.release(KEY_RIGHT_ARROW);
    delay(2);
  }

}

int MouseReadAxis(int thisAxis) {
  
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the rest position threshold, use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  if (abs(distance) > 2 && abs(distance) < 5) {
    distance == (distance / 4);
  }

  if (abs(distance) > 14) {
    distance == (distance * 5);
  }

  return distance;

}

void triggerMouse() {

    // read and scale the two axes:
    int xReading = MouseReadAxis(A3);
    int yReading = MouseReadAxis(A2);

    Mouse.move(xReading, yReading, 0);

    // read the mouse button and click or not click:
    if (digitalRead(mouseButton) == LOW) {
      if (!Mouse.isPressed(MOUSE_LEFT)) {
        Mouse.press(MOUSE_LEFT);
      }
    } else {
      if (Mouse.isPressed(MOUSE_LEFT)) {
        Mouse.release(MOUSE_LEFT);
      }
    }
    delay(2);

}

void triggerJoystick() {

  mapX = map(analogRead(A1), 0, 1023, -4, 4);
  mapY = map(analogRead(A0), 0, 1023, 4, -4);

    if (mapX <= -3) {
      Keyboard.press('a');
    } else {
      Keyboard.release('a');
      delay(2);
    }
    if (mapX >= 3) {
      Keyboard.press('d');
    } else {
      Keyboard.release('d');
      delay(2);
    }
    if (mapY <= -3) { 
      Keyboard.press('s');
    } else {
      Keyboard.release('s');
      delay(2);
    }
    if (mapY >= 3) {
      Keyboard.press('w');
    } else {
      Keyboard.release('w');
      delay(2);
    }

    if (digitalRead(stickButton) == LOW) {
      Keyboard.press((char) 32);
      delay(2);
    } else {
      Keyboard.release((char) 32);
      delay(2);
    }

}
