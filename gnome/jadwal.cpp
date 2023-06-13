#include "jadwal.h"
int jadwal::adjust = 0;//values in jadwal_data.cpp is from jakarta barat

void jadwal::setCity(city c) {
  if (c==jadwal::city::jogja) {
    jadwal::adjust = (0);
  }
  else if (c==jadwal::city::sleman) {
    jadwal::adjust = (-1);
  }
  else if (c==jadwal::city::jakarta_barat) {
    jadwal::adjust = 15;
  }
  else if (c==jadwal::city::jakarta) {
    jadwal::adjust = 15;
  }
}

int jadwal::subuh(int bulan, int tanggal) {
  return (int)(
    (jadwal_subuh[((bulan-1)*31+tanggal)])
  ) + jadwal::adjust;
}
int jadwal::terbit(int bulan, int tanggal) {
  return (int)(
    (jadwal_terbit[((bulan-1)*31+tanggal)])
  ) + jadwal::adjust;
}
int jadwal::dzuhur(int bulan, int tanggal) {
  return (int)(
    (jadwal_dzuhur[((bulan-1)*31+tanggal)])
  ) + jadwal::adjust;
}
int jadwal::ashar(int bulan, int tanggal) {
  return (int)(
    (jadwal_ashar[((bulan-1)*31+tanggal)])
  ) + jadwal::adjust;
}
int jadwal::maghrib(int bulan, int tanggal) {
  return (int)(
    (jadwal_maghrib[((bulan-1)*31+tanggal)])
  ) + jadwal::adjust;
}
int jadwal::isya(int bulan, int tanggal) {
  return (int)(
    (jadwal_isya[((bulan-1)*31+tanggal)])
  ) + jadwal::adjust;
}

int jadwal::hhmmToMinuteValue(int hh, int mm) {
  return hh*60 + mm;
}

jadwal::Time::Time(int hh, int mm) {
  this->hour = hh;
  this->minute = mm;
}
jadwal::Time::Time(int minutevalue) {
  this->hour = minutevalue/60;
  this->minute = minutevalue%60;
}
int jadwal::Time::toMinuteValue() {
  return jadwal::hhmmToMinuteValue(this->hour,  this->minute);
}


int absolute(int n) {
  if (n>=0) return n;
  else return -n;
}
/**
 * get closest jadwal in today bulan,tanggal
 * @param bulan: month
 * @param tanggal: month-date
 * @param now_inminuteValue: current time, in MinuteValue (hh*60 + mm)
 * @return the different between now and the closest jadwal in MinuteValue, 
 * if current_time has passed the closest, return positive number,
 * else if current_time has not passed the closest time return negative number
 * 
 * example March, day 09, current time is 19.00, isya is 19.07, return -7
 * example March, day 09, current time is 20.00, isya is 19.07, return 53
 */
int jadwal::getClosestJadwal(int bulan, int tanggal, int now_MinuteValue) {
  int subuh = jadwal::subuh(bulan,tanggal);
  int terbit = jadwal::terbit(bulan,tanggal);
  int dzuhur = jadwal::dzuhur(bulan,tanggal);
  int ashar = jadwal::ashar(bulan,tanggal);
  int maghrib = jadwal::maghrib(bulan, tanggal);
  int isya = jadwal::isya(bulan,tanggal);
  
  int diff_subuh = absolute(now_MinuteValue - subuh);
  int diff_terbit = absolute(now_MinuteValue - terbit);
  int diff_dzuhur = absolute(now_MinuteValue - dzuhur);
  int diff_ashar = absolute(now_MinuteValue - ashar);
  int diff_maghrib = absolute(now_MinuteValue - maghrib);
  int diff_isya = absolute(now_MinuteValue - isya);
  
  uint8_t negative = 1;//current time hasn't passed the closest 
  int min = diff_subuh;
  if (subuh<now_MinuteValue) negative = 0;//current time has passed the closest 
  
  if (diff_terbit<min){
    min = diff_terbit;
    negative = 1;
    if (terbit<now_MinuteValue) negative = 0;//current time has passed the closest 
  }
  if (diff_dzuhur<min){
    min = diff_dzuhur;
    negative = 1;
    if (dzuhur<now_MinuteValue) negative = 0;//current time has passed the closest 
  }
  if (diff_ashar<min) {
    min = diff_ashar;
    negative = 1;
    if (ashar<now_MinuteValue) negative = 0;//current time has passed the closest 
  }
  if (diff_maghrib<min) {
    min = diff_maghrib;
    negative = 1;
    if (maghrib<now_MinuteValue) negative = 0;//current time has passed the closest 
  }
  if (diff_isya<min) {
    min = diff_isya;
    negative = 1;
    if (isya<now_MinuteValue) negative = 0;//current time has passed the closest 
  }
  if (negative==1) {
    return -min;
  }
  return min;
}
