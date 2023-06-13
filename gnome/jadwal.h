#ifndef JADWAL_H
#define JADWAL_H

#include "inttypes.h"
extern const  uint16_t jadwal_subuh[];
extern const  uint16_t jadwal_terbit[];
extern const  uint16_t jadwal_dzuhur[];
extern const  uint16_t jadwal_ashar[];
extern const  uint16_t jadwal_maghrib[];
extern const  uint16_t jadwal_isya[];

extern int absolute(int n);

#define shubuh subuh
#define ashr ashar
namespace jadwal {
  class Time {
    public:
      int hour;
      int minute;
      Time(int hh, int mm);
      Time(int minutevalue);
      // convert 
      int toMinuteValue();
  };
  
  /**
   * adjust the jadwal to return by this value, can be + or -
   */
  extern int adjust;
  /**
   * city name enum
   */
  enum city {
    sleman,
    jogja,
    jakarta,
    jakarta_barat,
  };
  
  
  void setCity(city what_city);
  
  /**
   * get jadwal subuh
   * @param bulan: month number 1-12 (starts from 1 for january)
   * @param tanggal: day number 1-31 (starts from 1 for day 1)
   * return jadwal in minutes (which is hh*60+mm)
   */
  int subuh(int bulan, int tanggal);
  int terbit(int bulan, int tanggal);
  int dzuhur(int bulan, int tanggal);
  int ashar(int bulan, int tanggal);
  int maghrib(int bulan, int tanggal);
  int isya(int bulan, int tanggal);
  
  int hhmmToMinuteValue(int hh, int mm);
  int getClosestJadwal(int bulan, int tanggal, int now_MinuteValue);
}
#endif
