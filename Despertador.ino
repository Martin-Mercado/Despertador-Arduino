#include <virtuabotixRTC.h>
#include <LiquidCrystal_I2C.h>
#define pinBuzzer 5
#define pinMainButton 4
#define pinLeftButton 12
#define pinRightButton 13

LiquidCrystal_I2C lcd(0x27, 18, 2);
virtuabotixRTC myRTC(6, 7, 8);


bool alarmSet = true;
int alarmHour = 18;
int alarmMin = 0;

void setup() {

  pinMode(pinMainButton, INPUT);
  pinMode(pinLeftButton, INPUT);
  pinMode(pinRightButton, INPUT);
  // segundos, minutos, horas, día de la semana, día del mes, mes, año

  //myRTC.setDS1302Time(0, 40, 19, 6,17,2,2023);

  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {

  int NowMinute = myRTC.minutes;

  myRTC.updateTime();


  //resetear lcd asi no queda el digito que sobra del ultimo minuto
  if (myRTC.minutes == NowMinute + 1) {

    lcd.clear();
  }

  //boton para poner y apagar la alarma
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


  //Fecha

  lcd.setCursor(0, 0);
  lcd.print(myRTC.dayofmonth);
  lcd.print("/");
  lcd.print(myRTC.month);
  lcd.print("/");
  lcd.print(myRTC.year);

  //hora
  lcd.setCursor(1, 1);

  lcd.print(myRTC.hours);
  lcd.print(":");
  lcd.print(myRTC.minutes);
  lcd.print(":");
  lcd.print(myRTC.seconds);

  //mostrar hora de alarma si esta seteada, sinon mostrar que no esta seteada
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

  //cambiar la hora de la alaema con boton izquierdo
  if (digitalRead(pinLeftButton)) {
    SetAlarm();
  }

  // apagar pantalla porque molesta
  if (digitalRead(pinRightButton)) {
    lcd.noBacklight();
    delay(1000);

    //calculate hour to turn on display
    int HourTurnedOff = myRTC.hours, HourToTurnOn;

    if (HourTurnedOff >= 0 && HourTurnedOff <= 15) {
      HourToTurnOn = HourTurnedOff + 8;
    } else if (HourTurnedOff > 15 && HourTurnedOff < 23) {
      HourToTurnOn = HourTurnedOff - 16;
    }
    while (true) {
      myRTC.updateTime();
      delay(200);

      if (digitalRead(pinRightButton) || HourToTurnOn == myRTC.hours) {
        lcd.backlight();
        break;
      }
    }
  }



  CheckAlarm();


  delay(200);
}

// Hacer ruidito de alarma
void buzz() {
  tone(pinBuzzer, 1000);
  delay(100);
  noTone(pinBuzzer);
  delay(150);
}


void printAlarmTime() {

  lcd.setCursor(5, 1);
  lcd.print(alarmHour);
  lcd.print(":");
  lcd.print(alarmMin);
  lcd.print("    ");
}


//check if alarm should go off
void CheckAlarm() {
  // prender alarma
  if (alarmHour == myRTC.hours && alarmMin == myRTC.minutes && alarmSet && myRTC.seconds == 0) {
    //fecha
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print(myRTC.dayofmonth);
    lcd.print("/");
    lcd.print(myRTC.month);
    lcd.print("/");
    lcd.print(myRTC.year);

    //hora
    lcd.setCursor(4, 1);

    lcd.print(myRTC.hours);
    lcd.print(":");
    lcd.print(myRTC.minutes);
    lcd.print(":");
    lcd.print(myRTC.seconds);

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


//funcion para setear a que hora suena la alarma
void SetAlarm() {
  //set Hours
  lcd.clear();
  bool HoursSet = true;
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


  //set Minutes
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