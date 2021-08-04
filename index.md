# Custom Controller 

This project was my attempt to create a custom controller that can be connected to a computer in order to play basic computer games and potentially to offer an alternative option to a mouse and keyboard.

| **Engineer** | **School** | **Area of Interest** | **Grade** |
|:--:|:--:|:--:|:--:|
| Tanvir Islam | Brooklyn Technical High School | Electrical Engineering | Incoming Senior

# Third Milestone

<HTML>
  
  The next major step in my project was adding an accelerometer as a separate source of user input. By tilting the the accelerometer in any given direction, this will carry out the same function as the joysticks. Furthermore, there is an integrated mode switch with an LED to display what mode the accelerometer is on. Of note, this is also the first time I was required to solder. Namely, the pins must be soldered to the holes on the accelerometer.
  
  There are four pins that must be soldered to the accelerometer board: VCC, GND, SCL (Serial Clock Line), and SDA (Serial Data Line). Assuming the board in question does not have dedicated pins for SDA and SCL, SDA must correspond to pin 2 and SCL must correspond to pin 3 in order for the accelerometer to return the proper values. When setting up the accelerometer, run a test sketch using the MPU6050 library to see what values are reflected in the serial monitor. Make sure to calibrate the code such that when the accelerometer is in a resting position, all accelerometer values return 0 m/s<sup>2</sup>.
  
</HTML>

# Second Milestone

<HTML>

<img src="images_folder/IMG-2232.jpg" height=560 width=510 align=right style="float:right; padding-left:10px">
  
  My second milestone includes the addition of a control stick, the usage of internal pullup resistors, and a separate button for changing the mode of the joystick from mouse control to WASD control. After understanding the basics of Arduino's functions and how to build a circuit, I focused on adding another component to increase the complexity as well as improve any pre-existing components. Some of the male-male wires were replaced with shorter variants to leave more space on the breadboard.
   
  The physical implementation of the control stick was relatively simple. Each pin on the control stick has a label that corresponds with a label on the Arduino Micro, all of which can be connected directly from the board to the component. More specifically, VR<sub>x</sub> and VR<sub>y</sub> connected to the analog pins as they need to detect a range of motion on a control stick, while SW is the integral button of said control stick and thus only needs a digital pin.
  
  Starting with the circuitry, there is a pin that connects ground on the Arduino Micro to the negative rail on the breadboard. For each button, a short wire connects the negative rail to one end of the button. On the other end, a longer wire connects each button to a pin on the Arduino. Normally, this circuit would have no potential difference throughout. However, initializing each pin as an internal pullup resistor (diagram shown) allows the circuit to not require the usage of the positive rail.
  
  Although not mentioned in the video, I opted for a dual analog system rather than a button-switch mechanism. This way, I can make one stick dedicated to WASD (game movement) and the other stick for the cursor.

<img src="images_folder/IMG-2229.jpg" height=325 width=525 align=left style="float:left; padding-right:10px">
  
</HTML>

[![Tanvir I Milestone 2](https://res.cloudinary.com/marcomontalbano/image/upload/v1626876927/video_to_markdown/images/youtube--iPxXEkMKDf8-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://www.youtube.com/watch?v=iPxXEkMKDf8 "Tanvir I Milestone 2"){:target="_blank" rel="noopener"}

# First Milestone

<HTML>
  
<img src="images_folder/IMG-2207.jpg" height=475 width=475 align=left style="float:left; padding-right:10px">
  
  My first milestone was creating a circuit that uses: an Arduino Micro, pushbuttons, and resistors, in order to remotely trigger a keyboard response (the arrow keys in particular). This point marks the establishment of the basics for this project. At its core, the button press changes the state of the voltage between the sensor wire and the rest of the circuit. I initially used male-male wires to connect the breadboard, which is why it may appear very jumbled.
 
  To start, the power supply and ground are provided by the Arduino Micro from the 3V3 pin and the GND pin respectively. The value the input pin reads essentially acts as a Boolean value for an "if" statement. When the button is released, the current is no longer flowing through the sensor wire. As a result, this prompted the use of a resistor (more specifically, the pull-down resistor) whose sole purpose is to return to ground, effectively completing the circuit.
  
  It is noteworthy that I was unable to establish a Bluetooth connection with the ESP32, which ran into constantly inconsistent issues, one of which is shown below. Thankfully, the Arduino Micro was still more than sufficient for the desired outcome.
  <br>
  <br>
  <br>
  <img src="images_folder/IMG-2225.jpg" height=200 width=400 align=left style="float:left; padding-right:20px">
  <img src="images_folder/ESP32 BlueTooth Issue.png" width=400 style="float:left; padding-right:65px">
  <br>
  <br>
  (Top) Sketch of the first version of the circuit. Note that the bottom bars are meant to be the positive and negative column on a breadboard.
  <br>
  <br>
  (Bottom) Screenshot of a BlueTooth issue that prevented the connection of the ESP32.
  
</HTML>

  
[![Tanvir I Milestone 1](https://res.cloudinary.com/marcomontalbano/image/upload/v1626222369/video_to_markdown/images/youtube--pSd6XLdJq5s-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://www.youtube.com/watch?v=pSd6XLdJq5s "Tanvir I Milestone 1"){:target="_blank" rel="noopener"}

  
