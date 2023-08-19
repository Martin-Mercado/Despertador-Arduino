#include <virtuabotixRTC.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define pinBuzzer 5
#define pinMainButton 4
#define pinLeftButton 12
#define pinRightButton 13

// Initialize LCD display with I2C address 0x27, 18 columns, and 2 rows
LiquidCrystal_I2C lcd(0x27, 18, 2);

// Initialize RTC with pins 6, 7, and 8 for DS1302 module
virtuabotixRTC myRTC(6, 7, 8);

// Global Variables
bool alarmSet = true;
int alarmHour = 18;
int alarmMin = 0;

void setup() {
  // Configure button pins as inputs
  pinMode(pinMainButton, INPUT);
  pinMode(pinLeftButton, INPUT);
  pinMode(pinRightButton, INPUT);

  // Initialize LCD and turn on backlight
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {
  // Store current minute for comparison
  int NowMinute = myRTC.minutes;

  // Update RTC time
  myRTC.updateTime();

  // Reset LCD display when a new minute starts
  if (myRTC.minutes == NowMinute + 1) {
    lcd.clear();
  }

  // Toggle alarm set status when main button is pressed
  if (digitalRead(pinMainButton) && alarmSet == false) {
    alarmSet = true;
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Alarm Set");
    delay(2000);
    lcd.clear();
  }
  if (digitalRead(pinMainButton) && alarmSet == true) {
    alarmSet = false;
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Alarm Not Set");
    delay(2000);
    lcd.clear();
  }

  // Display current date
  lcd.setCursor(0, 0);
  lcd.print(myRTC.dayofmonth);
  lcd.print("/");
  lcd.print(myRTC.month);
  lcd.print("/");
  lcd.print(myRTC.year);

  // Display current time
  lcd.setCursor(1, 1);
  lcd.print(myRTC.hours);
  lcd.print(":");
  lcd.print(myRTC.minutes);
  lcd.print(":");
  lcd.print(myRTC.seconds);

  // Display alarm time if set, otherwise show "NOT SET"
  if (alarmSet) {
    lcd.setCursor(11, 0);
    lcd.print("SET");
    lcd.setCursor(10, 1);
    lcd.print(alarmHour);
    lcd.print(":");
    lcd.print(alarmMin);
  } else {
    lcd.setCursor(11, 0);
    lcd.print("NOT");
    lcd.setCursor(11, 1);
    lcd.print("SET");
  }

  // Change alarm time using left button
  if (digitalRead(pinLeftButton)) {
    SetAlarm();
  }

  // Turn off display temporarily using right button
  if (digitalRead(pinRightButton)) {
    lcd.noBacklight();
    delay(1000);

    // Calculate hour to turn on display
    int HourTurnedOff = myRTC.hours, HourToTurnOn;
    if (HourTurnedOff >= 0 && HourTurnedOff <= 15) {
      HourToTurnOn = HourTurnedOff + 8;
    } else if (HourTurnedOff > 15 && HourTurnedOff < 23) {
      HourToTurnOn = HourTurnedOff - 16;
    }

    // Wait until HourToTurnOn matches current hour or right button is pressed
    while (true) {
      myRTC.updateTime();
      delay(200);
      if (digitalRead(pinRightButton) || HourToTurnOn == myRTC.hours) {
        lcd.backlight();
        break;
      }
    }
  }

  // Check if alarm should go off
  CheckAlarm();

  // Add delay for stability
  delay(200);
}

// Generate a buzz sound using the buzzer
void buzz() {
  tone(pinBuzzer, 1000);
  delay(100);
  noTone(pinBuzzer);
  delay(150);
}

// Display the alarm time on the LCD
void printAlarmTime() {
  lcd.setCursor(5, 1);
  lcd.print(alarmHour);
  lcd.print(":");
  lcd.print(alarmMin);
  lcd.print("    ");
}

// Check if alarm should go off
void CheckAlarm() {
  if (alarmHour == myRTC.hours && alarmMin == myRTC.minutes && alarmSet && myRTC.seconds == 0) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print(myRTC.dayofmonth);
    lcd.print("/");
    lcd.print(myRTC.month);
    lcd.print("/");
    lcd.print(myRTC.year);

    lcd.setCursor(4, 1);
    lcd.print(myRTC.hours);
    lcd.print(":");
    lcd.print(myRTC.minutes);
    lcd.print(":");
    lcd.print(myRTC.seconds);

    // Sound the alarm for a specified duration
    int secondAlarmCalled = myRTC.seconds;
    for (int i = 1200; i >= 0; i--) {
      if (digitalRead(pinMainButton)) {
        alarmSet = false;
        lcd.clear();
        break;
      }
      buzz();
    }
  }
}

// Set the alarm time using buttons
void SetAlarm() {
  lcd.clear();
  bool HoursSet = true;

  // Set the alarm hour
  while (HoursSet) {
    delay(40);
    if (digitalRead(pinLeftButton) && alarmHour != 0) {
      alarmHour -= 1;
    } else if (digitalRead(pinRightButton) && alarmHour < 23) {
      alarmHour += 1;
    } else if (digitalRead(pinMainButton)) {
      HoursSet = false;
    }
    lcd.setCursor(0, 0);
    lcd.print("Set Alarm Hour: ");
    printAlarmTime();
  }

  // Set the alarm minutes
  bool MinutesSet = true;
  lcd.setCursor(0, 0);
  lcd.print("Set Alarm Minute: ");
  printAlarmTime();
  delay(2600);

  while (MinutesSet) {
    delay(40);
    if (digitalRead(pinLeftButton) && alarmMin != 0) {
      alarmMin -= 1;
    } else if (digitalRead(pinRightButton) && alarmMin < 59) {
      alarmMin += 1;
    } else if (digitalRead(pinMainButton)) {
      MinutesSet = false;
    }
    lcd.setCursor(0, 0);
    lcd.print("Set Alarm Minute: ");
    printAlarmTime();
  }

  // Display confirmation and set alarm time
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Alarm Set: ");
  lcd.setCursor(4, 1);
  lcd.print(alarmHour);
  lcd.print(":");
  lcd.print(alarmMin);
  delay(3000);
  lcd.clear();
}
