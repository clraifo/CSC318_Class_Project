/* 
   Morse Code Converter for Arduino with LCD and Audio Feedback

   Description:
   This program takes text input via the Arduino serial monitor, converts it to Morse code, and displays 
   the text on an LCD screen while also providing visual feedback via LEDs and auditory feedback via a buzzer.
*/

// Include the LiquidCrystal library to interface with the LCD
#include <LiquidCrystal.h>

// Initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Define pins for various components
const int onboardLedPin = 13;      // Onboard LED (mostly for debugging)
const int morseLedPin = 9;         // LED to blink Morse code
const int errorLedPin = 10;        // LED to indicate an error (like an unsupported character)
const int buzzerPin = 8;           // Buzzer to provide auditory Morse code

// Variables to keep track of text and LCD cursor position
int currentCursorPos = 0;          // Track current cursor position on LCD
String inputText = "";             // Store the user's input text

int brightness = 100;              // Control brightness of the LEDs (can be adjusted from 0 to 255)
int time_delay = 750;              // Standard delay time between Morse signals

// Morse code mappings for alphabets, numbers, and some punctuation
char* morseCode[43] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--..",
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.",
  "--..--", ".-.-.-", "-.-.-", "---...", ".----.", ".-..-.", "-....-"
};

void setup() {
  // Set up pins for output
  pinMode(buzzerPin, OUTPUT);
  pinMode(onboardLedPin, OUTPUT);
  pinMode(morseLedPin, OUTPUT);
  pinMode(errorLedPin, OUTPUT);
  
  // Initialize some outputs to LOW (turned off)
  digitalWrite(onboardLedPin, LOW);
  digitalWrite(errorLedPin, LOW);
  
  // Begin serial communication at 9600 baud
  Serial.begin(9600);
  Serial.println("Enter a word:");
  
  // Set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  lcd.print("Enter a word:");
}

// Function to handle invalid characters
void handleError() {
    // Send an error message to the serial monitor
    Serial.println("Non-standard character detected, please try again");
    
    // Display error message on the LCD
    lcd.clear();
    lcd.print("Err Invalid Char");
    
    // Blink the error LED 5 times quickly
    for (int i = 0; i < 5; i++) {
        analogWrite(errorLedPin, brightness);  // Turn error LED on
        delay(100);                            // 100ms on
        analogWrite(errorLedPin, 0);           // Turn error LED off
        delay(25);                             // 25ms off
    }
    delay(time_delay);  // Pause after error message
    lcd.clear();        // Clear the LCD for further input
    currentCursorPos = 0;  // Reset the cursor position
}

void loop() {
  // Check if there's any data available from the serial monitor
  if (Serial.available()) {
    char c = Serial.read();  // Read a character from the serial monitor
    
    // Handle spaces separately; display them on the LCD but don't convert to Morse
    if (c == ' ') {
      inputText += ' ';
      lcd.setCursor(0, 1);
      if (inputText.length() <= 16) {
        lcd.print(inputText);
      } else {
        lcd.print(inputText.substring(inputText.length() - 16));
      }
      delay(time_delay);  // Pause for spaces
      return;
    }

    // Append the character to the input text string
    inputText += c;

    // Display the text on the LCD, scrolling if necessary
    lcd.setCursor(0, 1);
    if (inputText.length() <= 16) {
      lcd.print(inputText);
    } else {
      lcd.print(inputText.substring(inputText.length() - 16));
    }

    // Check if the entered character is a newline or carriage return
    if (c != '\n' && c != '\r') {
        bool validCharacter = false;  // Flag to check if the character is valid

        // Convert valid characters to Morse and blink the Morse LED
        if (c >= 'a' && c <= 'z') {
          blinkMorse(morseCode[c - 'a']);
          validCharacter = true;
        } else if (c >= 'A' && c <= 'Z') {
          blinkMorse(morseCode[c - 'A']);
          validCharacter = true;
        } else if (c >= '0' && c <= '9') {
          blinkMorse(morseCode[c - '0' + 26]);
          validCharacter = true;
        } else {
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

        // If the character was not valid, handle the error
        if (!validCharacter) {
            handleError();
        } else {
            delay(time_delay);  // Gap between Morse signals
        }
    } else {
        delay(2000);  // Allow 2 seconds for the user to read the scrolled message on the LCD
        lcd.clear();  // Clear the LCD
        inputText = "";  // Reset the input text string
        lcd.print("Enter a word:");  // Display the prompt again
    }
  }
}

// Function to convert characters to Morse and blink the LED
void blinkMorse(char* code) {
  for (int i = 0; i < strlen(code); i++) {
    if (code[i] == '.') {
      analogWrite(morseLedPin, brightness);  // Blink the LED for a dot
      tone(buzzerPin, 1000, 300);  // Sound the buzzer for a dot
      delay(300);
      analogWrite(morseLedPin, 0);
      noTone(buzzerPin);
      delay(200);
    } else if (code[i] == '-') {
      analogWrite(morseLedPin, brightness);  // Blink the LED for a dash
      tone(buzzerPin, 1000, 1000);  // Sound the buzzer for a dash
      delay(1000);
      analogWrite(morseLedPin, 0);
      noTone(buzzerPin);
      delay(200);
    }
  }
}

/* 
   Wiring Instructions:

   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

   Morse code LED -> pin 9 (with appropriate resistor to ground)
   Error LED -> pin 10 (with appropriate resistor to ground)
   Buzzer -> pin 8 (other leg to ground)
*/
