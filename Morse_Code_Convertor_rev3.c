#include <LiquidCrystal.h>

// Initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Define LED Pins
const int onboardLedPin = 13;
const int morseLedPin = 9;  // Morse code output LED
const int errorLedPin = 10;  // Error indication LED

int brightness = 100;  // Half brightness
int time_delay = 750;

void blinkMorse(char* code);  // Function declaration

// Define Morse code mappings
char* morseCode[43] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--..",
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.",
  "--..--", ".-.-.-", "-.-.-", "---...", ".----.", ".-..-.", "-....-"
};

void setup() {
  pinMode(onboardLedPin, OUTPUT);
  pinMode(morseLedPin, OUTPUT);
  pinMode(errorLedPin, OUTPUT);
  
  digitalWrite(onboardLedPin, LOW);
  digitalWrite(errorLedPin, LOW);
  
  Serial.begin(9600);
  Serial.println("Enter a word:");
  
  // Set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Enter a word:");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    
    // Display the character on the LCD
    lcd.print(c);

    // Check for newline or carriage return and skip them
    if (c != '\n' && c != '\r') {
        bool validCharacter = false;  // Flag to check if the character is valid

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

        // If the character was not valid, print error and blink rapidly
        if (!validCharacter) {
            Serial.println("Non-standard character detected, please try again");
            for (int i = 0; i < 5; i++) {
                analogWrite(errorLedPin, brightness);  // Turn error LED on
                delay(100);  // 100ms on
                analogWrite(errorLedPin, 0);   // Turn error LED off
                delay(25);  // 25ms off
            }
        }

        delay(time_delay);  // Gap between letters
    }
  }
}

void blinkMorse(char* code) {
  for (int i = 0; i < strlen(code); i++) {
    if (code[i] == '.') {
      analogWrite(morseLedPin, brightness);  // Control brightness
      delay(300);
      analogWrite(morseLedPin, 0);  // Turn off
      delay(200);
    } else if (code[i] == '-') {
      analogWrite(morseLedPin, brightness);  // Control brightness
      delay(1000);
      analogWrite(morseLedPin, 0);  // Turn off
      delay(200);
    }
  }
}
