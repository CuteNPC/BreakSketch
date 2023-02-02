#
# Makefile
#
CC = g++
CFLAGS = -fopenmp -Wall -Wextra -std=c++17 -mavx -march=native -O3 -Wno-unused-function -Wno-unused-parameter

all: TestAcc DataProcess

TestAcc: TestAcc.cpp
	$(CC) $(CFLAGS) -o TestAcc TestAcc.cpp
	
DataProcess: DataProcess.cpp
	$(CC) $(CFLAGS) -o DataProcess DataProcess.cpp

clean:
	rm TestAcc DataProcess



