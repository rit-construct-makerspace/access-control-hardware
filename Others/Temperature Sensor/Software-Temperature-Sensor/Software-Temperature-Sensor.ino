/*

ACS Temperature Sensor

This add-on to the ACS bus will monitor a remote 10k thermistor, and report over- and under-temperature concerns via the interrupt pin. 

Onboard 3-key interface allows you to set information about the system. 

Settings:
  Enter settings by pressing and holding center button for 3 seconds, when Acces signal is not on.
  When in settings menu, center button flashes every 500mS
  Following settings available:
    Units (Indicated U) - U-CE or U-FH for Celsius/Farenheit
    High Temperature (Indicated H) - H095 for 95 degrees
    Low (Incidated L) - L010 for 10 degrees
    Brightness (indicated b) - bOFF for 0, b-01 to b-07 for 0-7. Should update in real-time to show, except 0 which is shown as 1. 
    Beta (indicated by bEtA) - screen shows the word, then flashes to the value as all 4 chars.
  Once you cycle through all settings, return to normal operation.

Display:
   Shows current temperature with unit when running
   When over- or under-temperature event, flash the accompanying button's light

*/

//Libraries
#include <TM16xx.h>
#include <TM1650.h>
#include <TM16xxDisplay.h>
#include <EEPROM.h>

//Pin Definitions
const byte THERM_PIN = PIN_PC5; 
const byte DAT_PIN = PIN_PD5;
const byte UP_LED = PIN_PD6;  //LED3
const byte UP_BUTTON = PIN_PB0; //Button3
const byte SET_LED = PIN_PB1; //LED1
const byte SET_BUTTON = PIN_PC1; //Button 1
const byte DOWN_LED = PIN_PB2; //LED2
const byte DOWN_BUTTON = PIN_PC2; //Button 2
const byte INTERRUPT_PIN = PIN_PC4;
const byte CLK_PIN = PIN_PE1;

//Global Variables - Settings
bool isCelsius = true;   // true = U-CE, false = U-FH
int highTemp = 95;       // H095
int lowTemp = 10;        // L010
int brightness = 7;      // 0 = bOFF, 1-7 = b-01 to b-07
int betaValue = 3950;    // Alternates bEtA / 3950

//Global Variables - Other
unsigned long ButtonTime = 0;
unsigned long lastTempReadTime = 0;
unsigned long lastAlarmBlinkTime = 0;
bool alarmBlinkState = false;
float currentTemp = 0.0;
bool isAlarming = false;
byte activeAlarmType = 0; // 0=None, 1=High, 2=Low
float smoothedTemp = -999.0; // -999.0 indicates uninitialized

//Objects
TM1650 chip(DAT_PIN, CLK_PIN);
TM16xxDisplay display(&chip, 4);

void setup() {
  // put your setup code here, to run once:

  //Step 1: Load Old Settings
  //First 4 bytes kept empty for future use
  EEPROM.get(4, highTemp);     // Bytes 4, 5
  EEPROM.get(6, lowTemp);      // Bytes 6, 7
  EEPROM.get(8, isCelsius);    // Byte 8
  EEPROM.get(9, brightness);   // Bytes 9, 10
  EEPROM.get(11, betaValue);   // Bytes 11, 12

if (betaValue < 1000 || betaValue > 5000) {
    // Memory is invalid, apply defaults
    isCelsius = true;
    highTemp = 85;
    lowTemp = 10;
    brightness = 7;
    betaValue = 3950;
  }

  //Set the screen brightness from memory
  display.setIntensity(brightness);

  //PinModes
  pinMode(SET_LED, OUTPUT);
  pinMode(UP_LED, OUTPUT);
  pinMode(DOWN_LED, OUTPUT);
  
  //Initial write of the interrupt to set open-drain mode. 
  digitalWrite(INTERRUPT_PIN, LOW);

}

void loop() {
  unsigned long currentMillis = millis();

  // --- Static timing variables for the loop ---
  static unsigned long lastTempReadTime = 0;
  static unsigned long lastAlarmBlinkTime = 0;
  static unsigned long lastDisplayUpdate = 0;
  
  static bool alarmBlinkState = false;
  static bool showAlarmText = false;
  
  static float currentTemp = 0.0;
  static bool isAlarming = false;
  static byte activeAlarmType = 0; // 0=None, 1=High, 2=Low, 3=Hardware Error
  static bool ntcError = false;

  // ---------------------------------------------------------
  // Step 1: Read and Smooth the Temperature (10Hz)
  // ---------------------------------------------------------
  if (currentMillis - lastTempReadTime >= 100) {
    lastTempReadTime = currentMillis;

    int rawADC = analogRead(THERM_PIN);
    
    // Check for Open Circuit (~1023) or Short Circuit (~0)
    if (rawADC > 1015 || rawADC < 5) {
      ntcError = true;
    } else {
      ntcError = false;
      
      // Calculate Resistance (thermistor to Ground, 10k to 5V)
      float resistance = 10000.0 / ((1023.0 / (float)rawADC) - 1.0);

      // Steinhart-Hart Equation
      float steinhart = resistance / 10000.0;       
      steinhart = log(steinhart);                   
      steinhart /= (float)betaValue;                
      steinhart += 1.0 / (25.0 + 273.15);           
      steinhart = 1.0 / steinhart;                  
      steinhart -= 273.15;                          

      float newTemp = isCelsius ? steinhart : (steinhart * 9.0 / 5.0) + 32.0;

      // Apply Exponential Moving Average (EMA) Filter
      if (smoothedTemp == -999.0) {
        smoothedTemp = newTemp; // Snap to value on first boot
      } else {
        smoothedTemp = (smoothedTemp * 0.8) + (newTemp * 0.2); 
      }
      currentTemp = smoothedTemp;
    }
  }

  // ---------------------------------------------------------
  // Step 2: Check Alarms
  // ---------------------------------------------------------
  if (ntcError) {
    isAlarming = true;
    activeAlarmType = 3; // NTC Hardware Error
  } else if (currentTemp >= highTemp) {
    isAlarming = true;
    activeAlarmType = 1; // High Alarm
  } else if (currentTemp <= lowTemp) {
    isAlarming = true;
    activeAlarmType = 2; // Low Alarm
  } else {
    isAlarming = false;
    activeAlarmType = 0;
    digitalWrite(UP_LED, LOW);
    digitalWrite(DOWN_LED, LOW);
  }

  if(isAlarming){
    pinMode(INTERRUPT_PIN, OUTPUT);
  } else{
    pinMode(INTERRUPT_PIN, INPUT);
  }

  // ---------------------------------------------------------
  // Step 3: Fast Alarm LED Flashing (4Hz)
  // ---------------------------------------------------------
  if (isAlarming) {
    if (currentMillis - lastAlarmBlinkTime >= 250) { 
      lastAlarmBlinkTime = currentMillis;
      alarmBlinkState = !alarmBlinkState;

      if (activeAlarmType == 1) { 
        digitalWrite(UP_LED, alarmBlinkState ? HIGH : LOW);
        digitalWrite(DOWN_LED, LOW);
      } else if (activeAlarmType == 2) { 
        digitalWrite(DOWN_LED, alarmBlinkState ? HIGH : LOW);
        digitalWrite(UP_LED, LOW);
      } else if (activeAlarmType == 3) {
        // Flash BOTH LEDs for a hardware error
        digitalWrite(UP_LED, alarmBlinkState ? HIGH : LOW);
        digitalWrite(DOWN_LED, alarmBlinkState ? HIGH : LOW);
      }
    }
  }

  // ---------------------------------------------------------
  // Step 4: Display Formatting & Updating (2Hz)
  // ---------------------------------------------------------
  if (currentMillis - lastDisplayUpdate >= 500) {
    lastDisplayUpdate = currentMillis;

    // --- User's Brightness Override ---
    // Update the screen intensity here so we don't flood the data bus
    int activeBrt = brightness;
    if (isAlarming && brightness == 0) {
      activeBrt = 1;
    }
    display.setIntensity(activeBrt);

    // Build the Temperature String
    char tempStr[6];
    char unit = isCelsius ? 'C' : 'F';

    // Format the temperature text
    if (currentTemp <= -10.0 || currentTemp >= 100.0) {
      int tempInt = (int)(currentTemp > 0 ? currentTemp + 0.5 : currentTemp - 0.5); 
      snprintf(tempStr, sizeof(tempStr), "%3d%c", tempInt, unit);
    } else {
      char floatBuf[6];
      dtostrf(currentTemp, 1, 1, floatBuf); 
      if (currentTemp >= 0.0 && currentTemp < 10.0) {
        snprintf(tempStr, sizeof(tempStr), " %s%c", floatBuf, unit);
      } else {
        snprintf(tempStr, sizeof(tempStr), "%s%c", floatBuf, unit);
      }
    }

    // Push to Screen
    if (isAlarming) {
      showAlarmText = !showAlarmText; 
      if (showAlarmText) {
        if (activeAlarmType == 1) display.println(" HI ");
        else if (activeAlarmType == 2) display.println(" LO ");
        else if (activeAlarmType == 3) display.println("NTC ");
      } else {
        if (activeAlarmType == 3) display.println(" ERR");
        else display.println(tempStr);
      }
    } else {
      showAlarmText = false; 
      display.println(tempStr);
    }
  }

  // ---------------------------------------------------------
  // Step 5: Settings Menu Entry & Standby Breathing Logic
  // ---------------------------------------------------------
  if (digitalRead(SET_BUTTON) == HIGH) { // Button is NOT pressed
    ButtonTime = currentMillis + 3000;
    
    // Breathing LED (Only if user set brightness to 0 AND no alarms are active)
    if (brightness == 0 && !isAlarming) {
      // Create a continuous 4000 millisecond (4 second) cycle
      unsigned long breathCycle = currentMillis % 4000; 
      int fadeValue;
      
      if (breathCycle < 2000) {
        // First 2 seconds: Fade In (Map 0-1999ms to PWM 0-255)
        fadeValue = map(breathCycle, 0, 1999, 0, 255);
      } else {
        // Next 2 seconds: Fade Out (Map 2000-3999ms to PWM 255-0)
        fadeValue = map(breathCycle, 2000, 3999, 255, 0);
      }
      
      analogWrite(SET_LED, fadeValue);
    } else {
      // Screen is on (normal operation or alarm mode), turn LED off
      digitalWrite(SET_LED, LOW);
    }
    
  } else { // Button IS pressed
    digitalWrite(SET_LED, HIGH); // Force solid ON so user knows they are pressing it
    
    if (currentMillis >= ButtonTime) {
      display.println("CONF");
      delay(1000);
      runSettingsMenu();
      
      // Reset timers so it reads immediately upon exit
      lastTempReadTime = 0; 
      lastDisplayUpdate = 0;
      ButtonTime = millis() + 3000; 
    }
  }
}

void runSettingsMenu() {
  bool inMenu = true;
  int menuState = 0; // 0:Units, 1:High, 2:Low, 3:Brightness, 4:Beta
  
  unsigned long previousBlinkMillis = 0;
  bool blinkState = true; 
  unsigned long lastInteractionMillis = millis();
  
  bool lastSet = HIGH, lastUp = HIGH, lastDown = HIGH;

  // Press-and-Hold Timing Variables
  unsigned long upPressTime = 0, upRepeatTime = 0;
  unsigned long downPressTime = 0, downRepeatTime = 0;
  
  // --- NEW: Acceleration Variables ---
  const unsigned long HOLD_DELAY = 500;         // Wait 500ms before auto-scrolling
  const unsigned long START_REPEAT_RATE = 150;  // Start slow (150ms per tick)
  const unsigned long MIN_REPEAT_RATE = 15;     // Max speed floor (15ms per tick)
  const unsigned long RATE_ACCELERATION = 10;   // Speed up by 10ms every tick
  
  unsigned long currentUpRate = START_REPEAT_RATE;
  unsigned long currentDownRate = START_REPEAT_RATE;

  // Force screen on to start the menu
  int activeBrt = (brightness == 0) ? 1 : brightness;
  display.setIntensity(activeBrt);

  while (inMenu) {
    unsigned long currentMillis = millis();

    // 1. Check Idle Timeout (10 seconds)
    if (currentMillis - lastInteractionMillis >= 10000) {
      inMenu = false;
      break; 
    }

    // Read Buttons
    bool currentSet  = digitalRead(SET_BUTTON);
    bool currentUp   = digitalRead(UP_BUTTON);
    bool currentDown = digitalRead(DOWN_BUTTON);

    // Pause Blinking While Holding Any Button
    if (currentSet == LOW || currentUp == LOW || currentDown == LOW) {
      previousBlinkMillis = currentMillis;
      blinkState = true; 
    }

    // ---------------------------------------------------------
    // 2. SET BUTTON (Single Click Only)
    // ---------------------------------------------------------
    if (currentSet == LOW && lastSet == HIGH) {
      lastInteractionMillis = currentMillis; 
      menuState++;
      if (menuState > 4) {
        inMenu = false; 
        break;
      }
      delay(50); 
    }
    lastSet = currentSet;

    // ---------------------------------------------------------
    // 3. UP BUTTON (Single Click + Accelerating Hold-to-Scroll)
    // ---------------------------------------------------------
    bool triggerUp = false; 

    if (currentUp == LOW) {
      if (lastUp == HIGH) { 
        // Initial Press
        triggerUp = true;
        upPressTime = currentMillis;
        upRepeatTime = currentMillis;
        currentUpRate = START_REPEAT_RATE; // Reset speed on fresh press
        delay(50); 
      } else if (currentMillis - upPressTime >= HOLD_DELAY) { 
        // Holding
        if (currentMillis - upRepeatTime >= currentUpRate) { 
          triggerUp = true;
          upRepeatTime = currentMillis;
          
          // Accelerate! (Decrease the delay between ticks)
          if (currentUpRate > MIN_REPEAT_RATE) {
            currentUpRate -= RATE_ACCELERATION;
            if (currentUpRate < MIN_REPEAT_RATE) currentUpRate = MIN_REPEAT_RATE;
          }
        }
      }
    }
    lastUp = currentUp;

    if (triggerUp && inMenu) {
      lastInteractionMillis = currentMillis; 
      
      switch (menuState) {
        case 0: isCelsius = true; break; 
        case 1: highTemp = min(highTemp + 1, 999); break;
        case 2: lowTemp = min(lowTemp + 1, 999); break;
        case 3: brightness = min(brightness + 1, 7); break;
        case 4: 
          betaValue++; 
          if (betaValue > 5000) betaValue = 1500; 
          break; 
      }
    }

    // ---------------------------------------------------------
    // 4. DOWN BUTTON (Single Click + Accelerating Hold-to-Scroll)
    // ---------------------------------------------------------
    bool triggerDown = false; 

    if (currentDown == LOW) {
      if (lastDown == HIGH) { 
        // Initial Press
        triggerDown = true;
        downPressTime = currentMillis;
        downRepeatTime = currentMillis;
        currentDownRate = START_REPEAT_RATE; // Reset speed on fresh press
        delay(50); 
      } else if (currentMillis - downPressTime >= HOLD_DELAY) {
        // Holding
        if (currentMillis - downRepeatTime >= currentDownRate) {
          triggerDown = true;
          downRepeatTime = currentMillis;
          
          // Accelerate! (Decrease the delay between ticks)
          if (currentDownRate > MIN_REPEAT_RATE) {
            currentDownRate -= RATE_ACCELERATION;
            if (currentDownRate < MIN_REPEAT_RATE) currentDownRate = MIN_REPEAT_RATE;
          }
        }
      }
    }
    lastDown = currentDown;

    if (triggerDown && inMenu) {
      lastInteractionMillis = currentMillis; 
      
      switch (menuState) {
        case 0: isCelsius = false; break; 
        case 1: highTemp = max(highTemp - 1, -99); break;
        case 2: lowTemp = max(lowTemp - 1, -99); break;
        case 3: brightness = max(brightness - 1, 0); break;
        case 4: 
          betaValue--; 
          if (betaValue < 1500) betaValue = 5000; 
          break;
      }
    }

    // ---------------------------------------------------------
    // 5. Handle Blinking Logic & Update Display
    // ---------------------------------------------------------
    if (currentMillis - previousBlinkMillis >= 500) {
      previousBlinkMillis = currentMillis;
      blinkState = !blinkState; 
    }

    char dispBuf[5]; 
    activeBrt = (brightness == 0) ? 1 : brightness;

    if (menuState < 4) {
      if (blinkState) {
        display.setIntensity(activeBrt);
      } else {
        display.setIntensity(0);
      }
    } else {
      display.setIntensity(activeBrt);
    }

    switch (menuState) {
      case 0: snprintf(dispBuf, sizeof(dispBuf), "%s", isCelsius ? "U-CE" : "U-FH"); break;
      case 1: snprintf(dispBuf, sizeof(dispBuf), "H%03d", highTemp); break;
      case 2: snprintf(dispBuf, sizeof(dispBuf), "L%03d", lowTemp); break;
      case 3: 
        if (brightness == 0) snprintf(dispBuf, sizeof(dispBuf), "bOFF");
        else snprintf(dispBuf, sizeof(dispBuf), "b- %d", brightness); 
        break;
      case 4: 
        if (blinkState) snprintf(dispBuf, sizeof(dispBuf), "%04d", betaValue);
        else snprintf(dispBuf, sizeof(dispBuf), "bEtA");
        break;
    }

    display.println(dispBuf);

    // ---------------------------------------------------------
    // 6. LED UX Logic
    // ---------------------------------------------------------
    bool canGoUp = true;
    bool canGoDown = true;

    if (menuState == 1 && highTemp >= 999) canGoUp = false;
    if (menuState == 1 && highTemp <= -99) canGoDown = false;
    if (menuState == 2 && lowTemp >= 999) canGoUp = false;
    if (menuState == 2 && lowTemp <= -99) canGoDown = false;
    if (menuState == 3 && brightness >= 7) canGoUp = false;
    if (menuState == 3 && brightness <= 0) canGoDown = false;

    digitalWrite(UP_LED, (canGoUp && currentUp == HIGH) ? HIGH : LOW);
    digitalWrite(DOWN_LED, (canGoDown && currentDown == HIGH) ? HIGH : LOW);
    digitalWrite(SET_LED, (currentSet == HIGH) ? HIGH : LOW); 
  }

  // ---------------------------------------------------------
  // 7. Cleanup & Save Indication on Exit
  // ---------------------------------------------------------
  activeBrt = (brightness == 0) ? 1 : brightness;
  display.setIntensity(activeBrt);
  display.println("SAvE");
  delay(1000); 
  //Save the values to EEPROM
  EEPROM.put(4, highTemp);     // Bytes 4, 5
  EEPROM.put(6, lowTemp);      // Bytes 6, 7
  EEPROM.put(8, isCelsius);    // Byte 8
  EEPROM.put(9, brightness);   // Bytes 9, 10
  EEPROM.put(11, betaValue);   // Bytes 11, 12
  //Disable all LEDs
  digitalWrite(UP_LED, LOW);
  digitalWrite(DOWN_LED, LOW);
  digitalWrite(SET_LED, LOW);
  
  display.setIntensity(brightness);
}