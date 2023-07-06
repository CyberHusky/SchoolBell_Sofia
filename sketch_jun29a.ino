#include <SoftwareSerial.h>
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h> // библиотека для mp3 плеера
#include "DS3231.h" // RTC библиотека, для "real time clock" - часы реального времени(дословно)
#include <RTClib.h>

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//#include <Wire.h> // 5 и 6-е строки для OLED дисплеев, библиотека занимает очень много места, так что временно убрали идею с олед.
//#include <OLED_I2C.h>//
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiAvrI2c oled;

// ====== Objects ======
//OLED  myOLED(SDA, SCL, 8);
//extern uint8_t SmallFont[];
//extern uint8_t MediumNumbers[];
//extern uint8_t BigNumbers[];

SoftwareSerial speaker(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

//DS3231 clock;
//RTCDateTime dt;


// ===== Global Variables ======
char weekDay[][4] = {"Mon","Tue","Wed","Thu","Fri",};

String times_bell[] = {"8:0:0", "8:30:0", "9:5:0", "9:15:0", "10:0:0", "10:10:0", "10:55:0", "11:10:0", "11:55:0", "12:15:0", "13:0:0", "13:42:0", "14:30:0", "14:40:0", "15:25:0", "15:35:0", "16:20:0"};


// ====== SETUP ======
void setup() {

  Serial.begin(9600);


  #ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }


  speaker.begin(9600);
  speaker.listen();    

  if (!myDFPlayer.begin(speaker)) {  //Использование библиотеки softwareSerial для связи с mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection of RX/TX in speaker!"));
    Serial.println(F("2.Please insert or reinsert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  //myOLED.begin();
  //myOLED.setFont(SmallFont);

  delay(10);

  Serial.println("set time!");
  //clock.setDateTime(__DATE__, __TIME__);


  
  myDFPlayer.volume(10);      //Установка громкости. Значения от 0 до 30
  //myDFPlayer.volumeUp();    //Повышение Громкости
  //myDFPlayer.volumeDown();
  Serial.println("Let's play 1 music!");
  //myDFPlayer.play(1);       //Воспроизведение первого и единственного mp3 файла
  //delay(23000);
  //myDFPlayer.enableLoop();
  //myDFPlayer.loop(2);  
  //myDFPlayer.disableLoop();
  //myDFPlayer.pause();

  Serial.println("Let's begin!");

  #if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.

  oled.setFont(System5x7);
  oled.clear();
}



// ====== LOOP ======
void loop() {

  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  //Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  delay(1000);
  

/*
  dt = clock.getDateTime(); // все та же библиотека DS3231

  Serial.print("Raw data: ");
  Serial.print(dt.year);   Serial.print("-");
  Serial.print(dt.month);  Serial.print("-");
  Serial.print(dt.day);    Serial.print(" ");
  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");
  Serial.println(dt.dayOfWeek);

  delay(1000);
*/
  Serial.println("HEY!");
  //char str_hour[]   = dt.hour;
  String str_hour   = String(now.hour(),DEC);
  String str_minute = String(now.minute(),DEC);
  String str_second = String(now.second(),DEC);
  String str_time_now = str_hour+":"+str_minute+":"+str_second;
  Serial.println(str_time_now);
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
   

  for (int i = 0; i < (sizeof(times_bell) / sizeof(times_bell[0])); i++){
    //Serial.println(times_bell[i]);
    //Serial.println(str_time_now);
    
    if (strcmp(times_bell[i].c_str(), str_time_now.c_str()) == 0 && (strcmp(daysOfTheWeek[now.dayOfTheWeek()], "Monday") == 0|| strcmp(daysOfTheWeek[now.dayOfTheWeek()] , "Tuesday") == 0 || strcmp(daysOfTheWeek[now.dayOfTheWeek()] , "Wednesday") == 0 || strcmp(daysOfTheWeek[now.dayOfTheWeek()], "Thursday") == 0  || strcmp(daysOfTheWeek[now.dayOfTheWeek()] , "Friday") == 0 )){
      Serial.println("Play music");
      myDFPlayer.play(1);
    }
    
  }

oled.clear();
oled.print("       "+str_time_now);


//String(year())
  //myOLED.clrScr();
  //myOLED.print({"Time:"});
  //myOLED.print(Plant1_info, LEFT, 16); 
  //myOLED.update();

  
}
