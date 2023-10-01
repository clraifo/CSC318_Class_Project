#include <LiquidCrystal.h>

// Initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Define LED Pins
const int onboardLedPin = 13;
const int morseLedPin = 9;  // Morse code output LED
const int errorLedPin = 10;  // Error indication LED
const int buzzerPin = 8;  // Buzzer output pin

int currentCursorPos = 0;
int brightness = 100;  // Half brightness
int time_delay = 750;
String inputText = "";  // Store the user input

void blinkMorse(char* code);

// Define Morse code mappings
char* morseCode[43] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--..",
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.",
  "--..--", ".-.-.-", "-.-.-", "---...", ".----.", ".-..-.", "-....-"
};

void setup() {
  pinMode(buzzerPin, OUTPUT);
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

void handleError() {
    Serial.println("Non-standard character detected, please try again");
    lcd.clear();
    lcd.print("Err Invalid Char");
    for (int i = 0; i < 5; i++) {
        analogWrite(errorLedPin, brightness);  // Turn error LED on
        delay(100);  // 100ms on
        analogWrite(errorLedPin, 0);   // Turn error LED off
        delay(25);  // 25ms off
    }
    delay(time_delay);  // Gap after error message
    lcd.clear();
    currentCursorPos = 0;  // Reset cursor position
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    
    if (c == ' ') { // Check if the input character is a space
      inputText += ' ';
      lcd.setCursor(0, 1);
      if (inputText.length() <= 16) {
        lcd.print(inputText);
      } else {
        lcd.print(inputText.substring(inputText.length() - 16));
      }
      delay(time_delay); // Maintain a delay for spaces
      return;
    }

    // Append character to the inputText
    inputText += c;

    // Display the character on the LCD
    lcd.setCursor(0, 1);
    if (inputText.length() <= 16) {
      lcd.print(inputText);
    } else {
      lcd.print(inputText.substring(inputText.length() - 16));
    }

    // Check for newline or carriage return
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

        // If the character was not valid, handle the error
        if (!validCharacter) {
            handleError();
        } else {
            delay(time_delay);  // Gap between letters
        }
    } else { // If it's a newline or carriage return
        delay(2000);  // A 2-second delay to let the user read the scrolled message
        lcd.clear();  // Clear the LCD
        inputText = "";  // Clear the inputText
        lcd.print("Enter a word:");  // Display the prompt again
    }
  }
}


void blinkMorse(char* code) {
  for (int i = 0; i < strlen(code); i++) {
    if (code[i] == '.') {
      analogWrite(morseLedPin, brightness);  // Control brightness
      tone(buzzerPin, 1000, 300);  // Start sound at 1000Hz for 300ms
      delay(300);
      analogWrite(morseLedPin, 0);  // Turn off
      noTone(buzzerPin);  // Stop sound
      delay(200);
    } else if (code[i] == '-') {
      analogWrite(morseLedPin, brightness);  // Control brightness
      tone(buzzerPin, 1000, 1000);  // Start sound at 1000Hz for 1000ms
      delay(1000);
      analogWrite(morseLedPin, 0);  // Turn off
      noTone(buzzerPin);  // Stop sound
      delay(200);
    }
  }
}

