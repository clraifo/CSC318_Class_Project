/*
This is a simple Arduino .ino sketch that drives a single LED via an ARDUINO Uno R3. 
The code allows the user to enter alphanumeric text in the serial monitor in the Arduino IDE, 
press enter, and then see it blinked out on the LED.
*/

// Define LED Pins
const int onboardLedPin = 13;
const int externalLedPin = 9; // Moved to a PWM-capable pin

int brightness = 128;  // Half brightness
int time_delay = 750;

// Define Morse code mappings
char* morseCode[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--..",
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."
};

void setup() {
  pinMode(onboardLedPin, OUTPUT);
  digitalWrite(onboardLedPin, LOW);  // Ensure the onboard LED is off
  
  pinMode(externalLedPin, OUTPUT);  // Set external LED pin as an output
  Serial.begin(9600);               // Initialize serial communication
  Serial.println("Enter a word: ");
}

void loop() {
  if (Serial.available()) {  // Check if data is available to read
    char c = Serial.read();  // Read the next character
    if (c >= 'a' && c <= 'z') {
      blinkMorse(morseCode[c - 'a']);
    } else if (c >= 'A' && c <= 'Z') {
      blinkMorse(morseCode[c - 'A']);
    } else if (c >= '0' && c <= '9') {
      blinkMorse(morseCode[c - '0' + 26]);  // +26 to offset past the letters
    }
    delay(time_delay);  // Gap between letters
  }
}

void blinkMorse(char* code) {
  for (int i = 0; i < strlen(code); i++) {
    if (code[i] == '.') {
      analogWrite(externalLedPin, brightness);  // Control brightness
      delay(300);
      analogWrite(externalLedPin, 0);  // Turn off
      delay(200);
    } else if (code[i] == '-') {
      analogWrite(externalLedPin, brightness);  // Control brightness
      delay(1000);
      analogWrite(externalLedPin, 0);  // Turn off
      delay(200);
    }
  }
}
