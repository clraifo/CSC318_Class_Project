/*
This Arduino sketch drives an LED using an ARDUINO Uno R3. 
Users can input alphanumeric text in the serial monitor of the Arduino IDE. 
Upon pressing enter, the text is represented in Morse code and blinked on an LED.
*/

// Define pin numbers
const int onboardLedPin = 13;       // Onboard LED pin number
const int externalLedPin = 9;       // External LED pin number (PWM-capable for brightness control)

int brightness = 128;               // LED brightness (0 to 255, with 128 being half brightness)
int time_delay = 750;               // Time delay between letters in Morse code

// Morse code mappings for letters a-z and numbers 0-9
char* morseCode[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--..",
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."
};

void setup() {
  pinMode(onboardLedPin, OUTPUT);   // Set onboard LED pin as output
  digitalWrite(onboardLedPin, LOW); // Turn off onboard LED
  
  pinMode(externalLedPin, OUTPUT);  // Set external LED pin as output
  Serial.begin(9600);               // Initialize serial communication at 9600 baud rate
  Serial.println("Enter a word: "); // Prompt user to enter a word in the serial monitor
}

void loop() {
  // Check if there's data available to read from the serial monitor
  if (Serial.available()) {
    char c = Serial.read();         // Read the incoming character

    // Check if the character is a lowercase letter
    if (c >= 'a' && c <= 'z') {
      blinkMorse(morseCode[c - 'a']);
    } 
    // Check if the character is an uppercase letter
    else if (c >= 'A' && c <= 'Z') {
      blinkMorse(morseCode[c - 'A']);
    } 
    // Check if the character is a number
    else if (c >= '0' && c <= '9') {
      blinkMorse(morseCode[c - '0' + 26]);  // Offset by 26 to account for a-z in the array
    }
    
    delay(time_delay);  // Delay between individual letters to distinguish them
  }
}

// Function to blink Morse code on the LED for a given character's Morse representation
void blinkMorse(char* code) {
  // Iterate through each symbol (dot or dash) in the Morse code representation
  for (int i = 0; i < strlen(code); i++) {
    // If the symbol is a dot
    if (code[i] == '.') {
      analogWrite(externalLedPin, brightness);  // Turn on LED with the specified brightness
      delay(300);                               // Duration LED stays on for a dot
      analogWrite(externalLedPin, 0);           // Turn off LED
      delay(200);                               // Duration LED stays off between Morse code symbols within a letter
    } 
    // If the symbol is a dash
    else if (code[i] == '-') {
      analogWrite(externalLedPin, brightness);  // Turn on LED with the specified brightness
      delay(1000);                              // Duration LED stays on for a dash
      analogWrite(externalLedPin, 0);           // Turn off LED
      delay(200);                               // Duration LED stays off between Morse code symbols within a letter
    }
  }
}
