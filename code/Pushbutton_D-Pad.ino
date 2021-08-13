#include <Keyboard.h> // The main library for sending keystrokes.

void setup() 
{
 Keyboard.begin();  // Initialise the library.
}

// Loop around waiting for a button press on pin 2.
// When the button is pressed, go to the function triggerAutomation.

void loop() {
  if(digitalRead(2) == HIGH || digitalRead(3) == HIGH || digitalRead(7) == HIGH || digitalRead(8) == HIGH)
  {
    triggerAutomation();    
  }
}

void triggerAutomation() {
  if(digitalRead(2) == HIGH) {
    Keyboard.press(KEY_LEFT_ARROW);    // Press and hold the up arrow key.
    Keyboard.releaseAll();
    delay(100);                      // Wait for the computer to register the press.
  }
  if(digitalRead(3) == HIGH) {
    Keyboard.press(KEY_DOWN_ARROW);
    Keyboard.releaseAll();
    delay(100);
  }
  if(digitalRead(7) == HIGH) {
    Keyboard.press(KEY_UP_ARROW);    // Press and hold the up arrow key.
    Keyboard.releaseAll();
    delay(100);                      // Wait for the computer to register the press.
  }
  if(digitalRead(8) == HIGH) {
    Keyboard.press(KEY_RIGHT_ARROW);
    Keyboard.releaseAll();
    delay(100);
  }
}
