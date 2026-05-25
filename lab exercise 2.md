I am using an Arduino Uno R3, a small breadboard, two push buttons, jumper wires,
and a 7-segment display. I placed the 7-segment display in the middle of the breadboard. 
The connections are as follows: pin A of the 7-segment display is connected to pin 2 of the 
Arduino Uno R3, pin B to pin 3, pin C to pin 4, pin D to pin 5, pin E to pin 6, pin F to pin 7, and pin G to pin 8.
I connected a 330Ω resistor to the common pin of the 7-segment display. Pin 10 of the Arduino is used for increment,
while pin 9 is used for decrement.

For the push buttons, I used red wires for the positive connections and black wires for the negative connections. 
I also used 10kΩ resistors for the push buttons. The positive rail of the breadboard is connected to the 5V pin of the Arduino, 
and the negative rail is connected to the GND pin. The left push button is used for incrementing the number, while the right push 
button is used for decrementing it. The programming language I used is C++.

This project demonstrates how a 7-segment display can be controlled using an Arduino Uno R3 and push buttons.
When the increment button is pressed, the displayed number increases from 0 to 9, while pressing the decrement button
decreases the number. The resistors are used to protect the components and ensure stable input signals. This setup is
useful for learning basic Arduino programming, digital input and output, and simple electronic circuit design.
