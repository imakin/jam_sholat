#include <iostream>
#include "jadwal.h"
#include <string>
int main(int argc, char **argv) {
    if (argc<5) {
        std::cout<<"usage: ./main.exe bb tt hh mm\n"
            <<"bb bulan, tt tanggal\n"
            <<"hh jam sekarang, mm menit sekarang\n"
            <<"output the closest jadwal in minute\n";
    }
    int minval = jadwal::hhmmToMinuteValue(std::__cxx11::stoi(argv[3]),std::__cxx11::stoi(argv[4]));
    int closest = jadwal::getClosestJadwal(std::__cxx11::stoi(argv[1]),std::__cxx11::stoi(argv[2]),minval);
    
    std::cout<<(closest>0?"+":"")<<closest<<"\n";
    return 0;
}
