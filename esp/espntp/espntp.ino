#define VERBOSE

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

//for dotmatrix
#include <LedControl.h>
int DIN = 16; // D0
int CS =  5;  // D1
int CLK = 4;  // D2
LedControl lc=LedControl(DIN,CLK,CS,0);

//enddotmatrix

#include "jadwal.h"
#include <TimeLib.h>
const char *ssid     = "yesdata";
const char *password = "ldks012789";
const char *ssidbpk     = "recording_your_data";
const char *passwordbpk = "ldks297599_--";

WiFiUDP udp;

//EasyNTPClient ntpClient(udp, "pool.ntp.org", ((7*60*60)+(0*60))); // GMT + 7:00
NTPClient timeClient(udp,((7*60*60)+(0*60)) );


void setup_dotmatrix() {
  //for dot matrix
  lc.shutdown(0,false);     //The MAX72XX is in power-saving mode on startup
  lc.shutdown(1,false);     //setting up next 8x8 max72xx on the same i2c wire
  lc.shutdown(2,false);     //total 4 displays
  lc.shutdown(3,false);
  lc.setIntensity(0,5);     // Set the brightness
  lc.setIntensity(1,5);
  lc.setIntensity(2,5);
  lc.setIntensity(3,5);
  lc.clearDisplay(0);        // and clear the display
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
}
void led(String value_sequence) {
  int len = value_sequence.length();
  int i=0;
  int display_n = 0;
  int display_line = 0;
  while (i<len) {
    int v = value_sequence.substring(i,i+3).toInt();
    lc.setRow(display_n, display_line, (byte)v);
    display_line+=1;
    if (display_line>7) {
      display_n += 1;
      display_line = 0;
    }
    i+=3;
  }
}


#define NUMBER_SPACE 10
#define NUMBER_DASH 11
byte number[][8] = {
  //0
  0b00111100,
  0b01100110,
  0b01100110,
  0b01100110,
  0b01100110,
  0b01100110,
  0b01100110,
  0b00111100,
  
  //1
  0b00011000,
  0b00111000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00111100,
  
  //2
  0b00111100,
  0b01111110,
  0b01100110,
  0b00001100,
  0b00011000,
  0b00110000,
  0b01100000,
  0b01111110,
  
  //3
  0b00111100,
  0b01111110,
  0b01100110,
  0b00001100,
  0b00011110,
  0b00000110,
  0b01111110,
  0b00111100,
  
  //4,
  0b00110110,
  0b00110110,
  0b01100110,
  0b01111110,
  0b01111110,
  0b00000110,
  0b00000110,
  0b00000110,
  
  //5,
  0b01111110,
  0b01100000,
  0b01111110,
  0b01111110,
  0b00000110,
  0b01100110,
  0b01111110,
  0b00111100,
  //6,
  0b00111100,
  0b01100110,
  0b01100000,
  0b01111100,
  0b01111110,
  0b01100110,
  0b01111110,
  0b00111100,
  
  //7,
  0b01111110,
  0b01111110,
  0b00001100,
  0b00001100,
  0b00011000,
  0b00111000,
  0b00110000,
  0b00110000,
  
  //8
  0b00111100,
  0b01100110,
  0b01100110,
  0b00111100,
  0b01111110,
  0b01100110,
  0b01100110,
  0b00111100,
  
  //9
  0b00111100,
  0b01100110,
  0b01100110,
  0b01111110,
  0b00111110,
  0b00000110,
  0b01100110,
  0b00111100,
  
  //space,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  
  //dash,
  0b00000000,
  0b00000000,
  0b00000000,
  0b01111110,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

void displayDraw(uint8_t displayN, byte frame[]) {
  displayDraw(displayN, frame, 0);
}
/**
 * draw frame to 8x8 led, can be adjusted
 * @param displayN: the display address, starts from 0 closest to arduino wire
 * @param frame[]: the frame array with length8 to display
 * @param adjust: shift left or right, -1 shift left by 1, +1 shift right by 1
 */
void displayDraw(uint8_t displayN, byte frame[], int adjust) {
  for (int i = 0; i < 8; i++)
  {
    byte d = frame[i];
    if (adjust<0) {
      d = d<<(-adjust);
    }
    else if (adjust>0) {
      d = d>>adjust;
    }
    lc.setRow(displayN,i,d);
  }
}
//display colon : in the middle of 2nd and 3rd display (hardcoded)
//@param display_or_clear true display, false clear
void displayMidColon(boolean display_or_clear) {
  // 3rd display
  lc.setLed(2, 1, 7, display_or_clear);
  lc.setLed(2, 2, 7, display_or_clear);
  lc.setLed(2, 5, 7, display_or_clear);
  lc.setLed(2, 6, 7, display_or_clear);
  // 2nd display
  lc.setLed(1, 1, 0, display_or_clear);
  lc.setLed(1, 2, 0, display_or_clear);
  lc.setLed(1, 5, 0, display_or_clear);
  lc.setLed(1, 6, 0, display_or_clear);
}

//display number n
//@param n: -999<n<=9999
void displayNumber(int n) {
  int8_t sign = 1;
  if (n<0) {
    sign = (-1);
    n = n*(-1);
  }
  
  
  
  int digit3 = n/1000;
  displayDraw(3, number[digit3]);
  n = n%1000;
  int digit2 = n/100;
  displayDraw(2, number[digit2]);
  n = n%100;
  int digit1 = n/10;
  displayDraw(1, number[digit1]);
  n = n%10;
  displayDraw(0, number[n]);
  
  if (sign<0) {
    //negative
    if (digit1==0) {
      displayDraw(1, number[NUMBER_DASH]);
      displayDraw(2, number[NUMBER_SPACE]);
      displayDraw(3, number[NUMBER_SPACE]);
    }
    else if (digit2==0) {
      displayDraw(2, number[NUMBER_DASH]);
      displayDraw(3, number[NUMBER_SPACE]);
    }
    else if (digit3==0) {
      displayDraw(3, number[NUMBER_DASH]);
    }
  }
  else {
    //positive
    if (digit1==0) {
      displayDraw(1, number[NUMBER_SPACE]);
    }
    if (digit2==0) {
      displayDraw(2, number[NUMBER_SPACE]);
    }
    if (digit3==0) {
      displayDraw(3, number[NUMBER_SPACE]);
    }
  }
}


void setup(){
  Serial.begin(115200);
  setup_dotmatrix();
  jadwal::setCity(jadwal::city::sleman);
  WiFi.begin(ssidbpk, passwordbpk);
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count += 1;
    if (count>=15) {
      Serial.println("Can't connect to recording_, try yesdata");
      count = -100;
      WiFi.begin(ssid,password);
    }
  }
}

void loop() {
  timeClient.update();
  //~ Serial.println(timeClient.getFormattedTime());
  //Serial.println(ntpClient.getUnixTime());
  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();
  int epoch = timeClient.getEpochTime();
  int _month = month(epoch); //month from TimeLib.h
  int _day = day(epoch);// from TimeLib.h
  int minutevalue = jadwal::hhmmToMinuteValue(hour,minute);
  
  int hour1 = hour/10;
  int hour0 = hour%10;
  int minute1 = minute/10;
  int minute0 = minute%10;
  displayDraw(3, number[hour1]);
  displayDraw(2, number[hour0], -1);
  displayDraw(1, number[minute1], 1);
  displayDraw(0, number[minute0]);
  displayMidColon(true);
  delay(495);
  displayMidColon(false);
  delay(495);
  Serial.println(timeClient.getEpochTime());
  Serial.println(month(timeClient.getEpochTime()));
 
  int closestJadwal = jadwal::getClosestJadwal(_month, _day, minutevalue);
  #ifdef VERBOSE
  Serial.print("closestJadwal: ");
  Serial.println(closestJadwal);
  #endif
  if (closestJadwal<30 && closestJadwal>(-30)) {
    displayNumber(closestJadwal);
    delay(1000);
  }
}
