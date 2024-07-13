#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/* Tomato Counter Bluetooth
  By Ardan collabs with Muhajirin
*/

// limit dari sensor jika ada benda
int limitSensor = 800;




// pin definitions color sensor 1
#define PIN_S0 4
#define PIN_S1 5
#define PIN_S2 6
#define PIN_S3 7
#define PIN_OUT 8

// pin definitions color sensor 2 
#define PIN_S0_2 24
#define PIN_S1_2 25
#define PIN_S2_2 26
#define PIN_S3_2 27
#define PIN_OUT_2 28


// library
#include <Servo.h>
#include <SoftwareSerial.h>

// data
unsigned int bagus = 0;
unsigned int setengahMatang = 0;
unsigned int busuk = 0;


// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setupLCD() {
  lcd.begin(16,2);
  lcd.backlight();
  tulis("TOMATO", "Counter v1.0");
}

void tulis(String s1, String s2) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(s1);
  lcd.setCursor(0,1);
  lcd.print(s2);
}


// servo
Servo s11; // servo 1 sensor 1
Servo s21; // servo 2 sensor 1
Servo s12; // servo 1 sensor 2
Servo s22; // servo 2 sensor 2


// min and max position servo
unsigned int min = 0;
unsigned int max = 90;
unsigned int servoDelay = 1500; // milli seconds


// millis for servo
long m11 = 0;
long m12 = 0;
long m21 = 0;
long m22 = 0;

// servo aktif
bool a11 = false;
bool a12 = false;
bool a21 = false;
bool a22 = false;

// check sensor aktif
bool as1 = false;
bool as2 = false;
long mas1 = 0;
long mas2 = 0;


// check millis
void checkServoMillis() {

  if(!a11 && !a12 && !a21 && !a22) {
    return;
  }
  
  if(a11) {
    if(millis() > m11 + servoDelay) {
      resetServo(&s11, true);
      a11 = false;
    }
  }

  if(a12) {
    if(millis() > m12 + (servoDelay * 1.5)) {
      resetServo(&s12, false);
      a12 = false;
    }
  }


  if(a21) {
    if(millis() > m21 + servoDelay) {
      resetServo(&s21, true);
      a21 = false;
    }
  }

  if(a22) {
    if(millis() > m22 + (servoDelay * 1.5)) {
      resetServo(&s22, false);
      a22 = false;
    }
  }


}


// counter
void activateCounter(Servo *srv, bool noServo) {
  if(noServo) {
    busuk++;
  } else {
    if((srv == &s11) || (srv == &s12)) {
      bagus++;
    }

    if((srv == &s21) || (srv == &s22)) {
      setengahMatang++;
    }
  }
  printDataToLCD();
}

void printDataToLCD() {
  tulis("Bgs = " + String(bagus) + " 1/2 Mtg = ", String(setengahMatang) +  "   " + " Bsk = " + String(busuk));  
}

// activate servo
void activateServo(Servo *srv, bool reverse)  {




  if(reverse) {
    (*srv).write(min);
  } else {
    (*srv).write(max);
  }
  
  
  // trigger the millis counter
  if(srv == &s11 && !a11) {
    m11 = millis();
    a11 = true;
  } else if(srv == &s12 && !a12) {
    m12 = millis();
    a12 = true;
  } else if(srv == &s21 && !a21) {
    m21 = millis();
    a21 = true;
  } else if(srv == &s22 && !a22) {
    m22 = millis();
    a22 = true;
  }

  // proccess the counter
  activateCounter(srv, false);
}

// reset servo postition
void resetServo(Servo *srv, bool reverse) {

  if(reverse) {
    (*srv).write(max);
    return;
  }

  (*srv).write(min);
}


// reset all servo to its position
void resetAllServo() {
  resetServo(&s11, true);
  resetServo(&s12, false);
  resetServo(&s21, true);
  resetServo(&s22, false);
}


// setup all servo
void setupAllServo() {
  s11.attach(2);
  s12.attach(3);
  s21.attach(9);
  s22.attach(10);
}


// setup the color sensor
void setupColorSensors() {
  // sensor 1
  // set pin modes
  pinMode(PIN_S0, OUTPUT);
  pinMode(PIN_S1, OUTPUT);
  pinMode(PIN_S2, OUTPUT);
  pinMode(PIN_S3, OUTPUT);
  pinMode(PIN_OUT, INPUT);
  
  // set the frequency scaling to 20% (S==1; S1=0)
  digitalWrite(PIN_S0,HIGH);
  digitalWrite(PIN_S1,LOW);


  // color sensor 2
  // set pin modes
  pinMode(PIN_S0_2, OUTPUT);
  pinMode(PIN_S1_2, OUTPUT);
  pinMode(PIN_S2_2, OUTPUT);
  pinMode(PIN_S3_2, OUTPUT);
  pinMode(PIN_OUT_2, INPUT);
  
  // set the frequency scaling to 20% (S==1; S1=0)
  digitalWrite(PIN_S0_2,HIGH);
  digitalWrite(PIN_S1_2,LOW);
}



// global data for sensor
int frequency_red = 0;
int frequency_red2 = 0;
int frequency_green = 0;
int frequency_green2 = 0;
int frequency_blue = 0;
int frequency_blue2 = 0;

// monitoring the color sensor
void monitorSensor1() {
  digitalWrite(PIN_S2, LOW);
  digitalWrite(PIN_S3, LOW);
  frequency_red = pulseIn(PIN_OUT, LOW);
  digitalWrite(PIN_S2, HIGH);
  digitalWrite(PIN_S3, HIGH);
  frequency_green = pulseIn(PIN_OUT, LOW);
  digitalWrite(PIN_S2, LOW);
  digitalWrite(PIN_S3, HIGH);
  frequency_blue = pulseIn(PIN_OUT, LOW);
}

void monitorSensor2() {
  digitalWrite(PIN_S2_2, LOW);
  digitalWrite(PIN_S3_2, LOW);
  frequency_red2 = pulseIn(PIN_OUT_2, LOW);
  digitalWrite(PIN_S2_2, HIGH);
  digitalWrite(PIN_S3_2, HIGH);
  frequency_green2 = pulseIn(PIN_OUT_2, LOW);
  digitalWrite(PIN_S2_2, LOW);
  digitalWrite(PIN_S3_2, HIGH);
  frequency_blue2 = pulseIn(PIN_OUT_2, LOW);
}

void printDataSensor(bool s1, bool s2) {
  if(s1) {
    Serial.print("R = ");
    Serial.print(frequency_red);
    Serial.print("\tG = ");
    Serial.print(frequency_green);
    Serial.print("\tB = ");
    Serial.println(frequency_blue);
  }

 

  if(s2) {
    Serial.print("R2 = ");
    Serial.print(frequency_red2);
    Serial.print("\tG2 = ");
    Serial.print(frequency_green2);
    Serial.print("\tB2 = ");
    Serial.println(frequency_blue2);
  }


  if(s1 || s2) {
    Serial.println();
    delay(100);
  }
  

}



// buat fungsi yang mengembalikan data int
// 0 - jika buah itu bagus
// 1 - jika buah itu setengah matang
// -1 - jika buah itu busuk
int threshold(int r, int g, int b) {
  int iret = -1;

  // kalibrasi data bagus dengan sensor
  if((r < 590) && (g > 700) && (b > 600)) {
    iret = 0;
  } else if((r > 590) && (g > 700) && (b > 600)) {
    iret = 1;
  } else if(r == 0 || g == 0 || b == 0) {
    iret = -2;
  }

  // kalibrasi data 1/2 matang dengan sensor

  return iret;
}


// analisa data sensor
void analyzeDataSensor() {
   if(frequency_red > limitSensor && frequency_blue > limitSensor && frequency_green > limitSensor)  {
    // kosong
  } else {
    // ada object, gerakkan servo berdasarkan klasifikasi objek
    if(!as1) {
      as1 = true;
      mas1 = millis();
      int iret = threshold(frequency_red, frequency_green, frequency_blue);

      if(iret == 0) {
        Serial.println("Bagus S1");
        activateServo(&s11, true); // servo bagus
      } else if(iret == 1) {
        Serial.println("1/2 Matang S1");
        activateServo(&s21, true); // servo 1/2 matang
      } else if(iret == -1) {
        Serial.println("Busuk S1");
        activateCounter(&s11, true); // busuk
      }
    }


  }

  if(frequency_red2 > limitSensor && frequency_blue2 > limitSensor && frequency_green2 > limitSensor)  {
    // kosong
  } else {
    // ada object, gerakkan servo berdasarkan klasifikasi objek

    if(!as2) {
      as2 = true;
      mas2 = millis();
      int iret = threshold(frequency_red2, frequency_green2, frequency_blue2);

      if(iret == 0) {
        Serial.println("Bagus S2");
        activateServo(&s12, false); // servo bagus
      } else if(iret == 1) {
        Serial.println("1/2 Matang S2");
        activateServo(&s22, false); // servo 1/2 matang
      } else if(iret == -1) {
        Serial.println("Busuk S2");
        activateCounter(&s22, true); // busuk
      }
    }

  }
}



// sending data counter
int sendingDelay = 3000;
long blueMillis = 0;
bool isSending = false;
void sendingDataCounter() {
  if(isSending || (bagus == 0 && setengahMatang == 0 && busuk == 0)) {
    return;
  }

  isSending = true;
  blueMillis = millis();
  String data = "*" + String(bagus) + "~" + String(setengahMatang) + "~" + String(busuk) + "#";
  //blueS.println(data);
  Serial.println(data);
}

// reset all data counter
void resetAllDataCounter() {
  busuk = 0;
  setengahMatang = 0;
  bagus = 0;
}

void setup() {

  // setup lcd
  setupLCD();
  delay(2000); // delay for 2 seconds

  // setup the servos
  tulis("Status", "Test Servo...");
  setupAllServo();
  delay(100); // delay 100 milli second
  // reset servo
  resetAllServo();
  activateServo(&s11, true);
  delay(500);
  activateServo(&s12, false);
  delay(500);
  activateServo(&s21, true);
  delay(500);
  activateServo(&s22, false);
  delay(500);
  tulis("Status", "Servo OK.");
  resetAllServo();
  resetAllDataCounter();
  delay(1000);


  // setup the color sensors
  tulis("Status", "Test Sensor...");
  setupColorSensors();
  delay(500);
  tulis("Sensor", "OK.");
  delay(1000);


  // inisialisasi bluetooth
  tulis("Status", "Test Bluetooth...");
  Serial.begin(9600); // initialize serial 
//  blueS.begin(9600);
  tulis("Bluetooth", "OK.");
  delay(1000);

  printDataToLCD();

}

void loop() {
  // monitoring the color sensors
  monitorSensor1();
  monitorSensor2();

  
  // print data sensor
  printDataSensor(false, false);


  // process data sensor
  analyzeDataSensor();

  // check servo millis
  checkServoMillis();

  // sending data to bluetooth
  sendingDataCounter();

  // cek aktif scanning data pada sensor dalam 3 detik
  if((millis() > mas1 + 3000) && as1) {
    as1 = false;
  }
  if((millis() > mas2 + 3000) && as2) {
    as2 = false;
  }

  // delay for data to sent
  if(millis() > blueMillis + sendingDelay) {
    isSending = false;
  }
  

}
