#include <Keyboard.h>

const int acsPin = 11;

int previousState = LOW; 
int currentState;

// --- New Timing Variables ---
unsigned long logoutTimerStart = 0;   // Stores the "stopwatch" start time
bool isLogoutPending = false;         // Keeps track of whether we are in the grace period
const unsigned long gracePeriod = 10000; // 10 seconds in milliseconds

void setup() {
  pinMode(acsPin, INPUT_PULLDOWN);
  Keyboard.begin();
  
  // Read the initial state of the switch right as the board boots
  previousState = digitalRead(acsPin);
}

void loginSequence() {
  Keyboard.press(KEY_RETURN);
  delay(50);
  Keyboard.release(KEY_RETURN);
  delay(200); 
  Keyboard.press(KEY_RETURN);
  delay(50);
  Keyboard.release(KEY_RETURN);
}

void logoutSequence() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  
  delay(400); 
  
  //UPDATE - call a script that lives on the computer now so it is easier to edit and change things going forward.
  String payload = "powershell -WindowStyle Hidden -ExecutionPolicy Bypass -File \"C:\\Scripts\\logout.ps1\"";
  Keyboard.print(payload);
  delay(100);
  
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();
  
  delay(1500); 
  
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('l');
  delay(100); 
  Keyboard.releaseAll();
}

void loop() {
  currentState = digitalRead(acsPin);

  // --- Card Inserted (Transition from LOW to HIGH) ---
  if (currentState == HIGH && previousState == LOW) {
    
    // Check if we were in the middle of a logout countdown
    if (isLogoutPending) {
      // Abort the logout! We don't need to log in again, just clear the pending flag.
      isLogoutPending = false;
    } 
    else {
      // Normal login sequence
      loginSequence();
    }
  }
  
  // --- Card Removed (Transition from HIGH to LOW) ---
  else if (currentState == LOW && previousState == HIGH) {
    // Start the stopwatch instead of logging out immediately
    isLogoutPending = true;
    logoutTimerStart = millis(); 
  }

  // --- Check the Stopwatch ---
  // If a logout is pending AND 10 seconds have passed since the timer started
  if (isLogoutPending && (millis() - logoutTimerStart >= gracePeriod)) {
    logoutSequence();
    isLogoutPending = false; // Reset the flag so it doesn't keep logging out
  }

  previousState = currentState;
  delay(50); // Debounce
}