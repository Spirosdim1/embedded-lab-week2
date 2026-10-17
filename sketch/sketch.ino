
// Ορισμός PWM Pins για το RGB LED (~)
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

// Ορισμός Pins για τα κουμπιά
const int btnRedPin = 2;
const int btnGreenPin = 4;
const int btnBluePin = 7;

// Μεταβλητές τιμών έντασης χρωμάτων (0, 85, 170, 255)
int redVal = 0;
int greenVal = 0;
int blueVal = 0;

// Χρονικές παράμετροι Debounce (40ms χωρίς delay)
const unsigned long debounceDelay = 40;

// Μεταβλητές αποπαρασίτωσης για το κόκκινο κουμπί
int lastRedBtnReading = HIGH;
int redButtonState = HIGH;
unsigned long lastRedDebounceTime = 0;

// Μεταβλητές αποπαρασίτωσης για το πράσινο κουμπί
int lastGreenBtnReading = HIGH;
int greenButtonState = HIGH;
unsigned long lastGreenDebounceTime = 0;

// Μεταβλητές αποπαρασίτωσης για το μπλε κουμπί
int lastBlueBtnReading = HIGH;
int blueButtonState = HIGH;
unsigned long lastBlueDebounceTime = 0;

// Συνάρτηση αύξησης τιμής: 0 -> 85 -> 170 -> 255 -> 0
int getNextColorValue(int currentValue) {
  int nextVal = currentValue + 85;
  if (nextVal > 255) {
    nextVal = 0;
  }
  return nextVal;
}

// Εκτύπωση τρέχουσας κατάστασης στο Serial Monitor
void printSerialStatus() {
  Serial.print("R=");
  Serial.print(redVal);
  Serial.print(" G=");
  Serial.print(greenVal);
  Serial.print(" B=");
  Serial.println(blueVal);
}

void setup() {
  Serial.begin(9600);

  // Ρύθμιση LED pins ως εξόδους
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Ρύθμιση κουμπιών ως εισόδους με εσωτερική αντίσταση pull-up
  pinMode(btnRedPin, INPUT_PULLUP);
  pinMode(btnGreenPin, INPUT_PULLUP);
  pinMode(btnBluePin, INPUT_PULLUP);

  // Αρχικοποίηση εξόδων (σβηστό LED στην αρχή)
  analogWrite(redPin, redVal);
  analogWrite(greenPin, greenVal);
  analogWrite(bluePin, blueVal);
}

bool updateButtonState(int buttonPin, int &lastReading, unsigned long &lastDebounceTime, int &buttonState, int &colorValue, int ledPin) {
  int reading = digitalRead(buttonPin);
  unsigned long currentTime = millis();

  if (reading != lastReading) {
    lastDebounceTime = currentTime;
  }

  if ((currentTime - lastDebounceTime) >= debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        colorValue = getNextColorValue(colorValue);
        analogWrite(ledPin, colorValue);
        lastReading = reading;
        return true;
      }
    }
  }

  lastReading = reading;
  return false;
}

void loop() {
  bool stateChanged = false;

  stateChanged |= updateButtonState(btnRedPin, lastRedBtnReading, lastRedDebounceTime, redButtonState, redVal, redPin);
  stateChanged |= updateButtonState(btnGreenPin, lastGreenBtnReading, lastGreenDebounceTime, greenButtonState, greenVal, greenPin);
  stateChanged |= updateButtonState(btnBluePin, lastBlueBtnReading, lastBlueDebounceTime, blueButtonState, blueVal, bluePin);

  if (stateChanged) {
    printSerialStatus();
  }
}