CXX=g++
RM=rm -f

SRCS=jadwal.cpp jadwal_data.cpp main.cpp
OBJS=$(subst .cc,.o,$(SRCS))

all: $(OBJS)
	$(CXX) -o main.exe $(OBJS)

#~ all:
#~   g++ -c jadwal.cpp -o jadwal.o
#~   g++ -c jadwal_data.cpp -o jadwal_data.o
#~   g++ -Wall -o jadwal.exe jadwal.o jadwal_data.o main.cpp
