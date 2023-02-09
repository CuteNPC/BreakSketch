#
# Makefile
#
CC = g++
CFLAGS = -fopenmp -std=c++17 -mavx -march=native -O3

all: TestAcc DataProcess

TestAcc: TestAcc.cpp
	$(CC) $(CFLAGS) -o TestAcc TestAcc.cpp
	
DataProcess: DataProcess.cpp
	$(CC) $(CFLAGS) -o DataProcess DataProcess.cpp

clean:
	rm TestAcc DataProcess



