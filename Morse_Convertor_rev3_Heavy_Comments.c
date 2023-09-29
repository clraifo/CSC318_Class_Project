/* This quick little Arduino circuit accepts user input via the ArdionIDE serial monitor and converts it 
into Morse Code, which is then blinked out by an external LED.

This version of the morse convertor has a few additions:

  - Added another LED for error indication
  - Added an LCD module for text display
  - Added more characters to the Morse code character set
  - Added exception handling for user text entry mistakes

Here are the complete wiring instructions for this version:

### 1. Arduino Uno:
The Arduino Uno will act as the heart of your project, controlling the LEDs and the LCD based on the input from the serial monitor.

### 2. LEDs:
1. **Morse LED (White or any other color you prefer)**:
   - Connect the longer leg (anode) to a 220Ω resistor. The other end of the resistor goes to Pin 9 on the Arduino.
   - Connect the shorter leg (cathode) directly to the GND (ground) pin on the Arduino.

2. **Error LED (Red)**:
   - Connect the longer leg (anode) to a 220Ω resistor. The other end of the resistor goes to Pin 10 on the Arduino.
   - Connect the shorter leg (cathode) directly to the GND (ground) pin on the Arduino.

### 3. LCD1602:
1. **VSS** (Pin 1 on the LCD) - Connect to GND on the Arduino.
2. **VDD** (Pin 2) - Connect to 5V on the Arduino.
3. **VO** (Pin 3) - Connect to the middle leg of a 10K potentiometer (this is for adjusting the contrast of the LCD).
4. **RS** (Pin 4) - Connect to Pin 12 on the Arduino.
5. **RW** (Pin 5) - Connect to GND on the Arduino.
6. **E** (Pin 6) - Connect to Pin 11 on the Arduino.
7. **Data Pins**:
   - **D0 to D3** (Pins 7-10) - We won't use these in 4-bit mode, so leave them unconnected.
   - **D4** (Pin 11) - Connect to Pin 5 on the Arduino.
   - **D5** (Pin 12) - Connect to Pin 4 on the Arduino.
   - **D6** (Pin 13) - Connect to Pin 3 on the Arduino.
   - **D7** (Pin 14) - Connect to Pin 2 on the Arduino.
8. **A** (Pin 15) - Connect to 5V through a 220Ω resistor. This powers the backlight.
9. **K** (Pin 16) - Connect to GND. This is the ground for the backlight.

### 4. 10K Potentiometer (for adjusting LCD contrast):
1. Connect one of the outer legs to GND.
2. Connect the other outer leg to 5V.
3. As mentioned earlier, the middle leg connects to the **VO** pin on the LCD.

After you've connected everything according to the above instructions, you should be good to go! Double-check your connections 
to ensure everything is secure and in the correct location. Once you're confident in your setup, you can power up the Arduino 
and upload the code.
*/

// Import the required library for the LCD
#include <LiquidCrystal.h>

// Initialize the LCD library with the interface pin numbers
// These pins connect the Arduino to the LCD via specific data and control lines
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Define pins for the LEDs
const int onboardLedPin = 13;       // The built-in LED on the Arduino board
const int morseLedPin = 9;          // External LED used for displaying Morse code
const int errorLedPin = 10;         // External LED used to indicate an error

// Variables to control the LED brightness and the delay between Morse code signals
int brightness = 100;  // LED brightness level (0-255)
int time_delay = 750;  // Time delay in milliseconds

// Function prototype for the Morse code blinking function
void blinkMorse(char* code);

// Define Morse code patterns for each character
// This array maps each character to its Morse code representation
char* morseCode[43] = {
  // Morse patterns for each letter of the alphabet
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--..",
  // Morse patterns for numbers 0-9
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.",
  // Morse patterns for some punctuation marks
  "--..--", ".-.-.-", "-.-.-", "---...", ".----.", ".-..-.", "-....-"
};

// This function is run once when the Arduino is powered on or reset
void setup() {
  // Configure LED pins as outputs
  pinMode(onboardLedPin, OUTPUT);
  pinMode(morseLedPin, OUTPUT);
  pinMode(errorLedPin, OUTPUT);
  
  // Turn off the on-board LED and the error LED at startup
  digitalWrite(onboardLedPin, LOW);
  digitalWrite(errorLedPin, LOW);
  
  // Start serial communication for sending/receiving data to/from the Arduino IDE's Serial Monitor
  Serial.begin(9600);
  // Display a message on the Serial Monitor to prompt the user
  Serial.println("Enter a word:");
  
  // Initialize the LCD with 16 columns and 2 rows
  lcd.begin(16, 2);
  // Display a message on the LCD to prompt the user
  lcd.print("Enter a word:");
}

// This function runs in a loop as long as the Arduino is powered
void loop() {
  // Check if there's incoming data on the serial port
  if (Serial.available()) {
    // Read the incoming byte (character) from the serial port
    char c = Serial.read();
    
    // Display the character on the LCD
    lcd.print(c);

    // If the character is not a newline or carriage return, process it
    if (c != '\n' && c != '\r') {
        bool validCharacter = false;  // Flag to track if the current character has a valid Morse representation

        // Check if the character is a lowercase letter and display its Morse code
        if (c >= 'a' && c <= 'z') {
          blinkMorse(morseCode[c - 'a']); // Convert the character to its Morse code index and blink it
          validCharacter = true;
        } 
        // Check if the character is an uppercase letter and display its Morse code
        else if (c >= 'A' && c <= 'Z') {
          blinkMorse(morseCode[c - 'A']);
          validCharacter = true;
        } 
        // Check if the character is a number and display its Morse code
        else if (c >= '0' && c <= '9') {
          blinkMorse(morseCode[c - '0' + 26]); // Numbers start at index 26 in the morseCode array
          validCharacter = true;
        } 
        // Check if the character is one of the supported punctuation marks and display its Morse code
        else {
          switch(c) {
            case ',': blinkMorse(morseCode[36]); validCharacter = true; break;
            case '.': blinkMorse(morseCode[37]); validCharacter = true; break;
            case ';': blinkMorse(morseCode[38]); validCharacter = true; break;
            case ':': blinkMorse(morseCode[39]); validCharacter = true; break;
            case '\'': blinkMorse(morseCode[40]); validCharacter = true; break;
            case '\"': blinkMorse(morseCode[41]); validCharacter = true; break;
            case '-': blinkMorse(morseCode[42]); validCharacter = true; break;
          }
        }

        // If the character doesn't have a Morse representation, indicate the error
        if (!validCharacter) {
            // Print an error message on the Serial Monitor
            Serial.println("Non-standard character detected, please try again");
            // Blink the error LED rapidly to indicate the error
            for (int i = 0; i < 5; i++) {
                analogWrite(errorLedPin, brightness);  // Turn the error LED on with the specified brightness
                delay(100);  // Keep it on for 100ms
                analogWrite(errorLedPin, 0);  // Turn the error LED off
                delay(25);  // Wait for 25ms
            }
        }

        // Wait between Morse code letters to make the output readable
        delay(time_delay);
    }
 
