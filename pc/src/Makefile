
# Makefile #


# $@ ··· [target]
# $^ ··· {Dependencies}


# Variables

EXE=play
SOURCES=main.o SerialPort/SerialPort.o Wave-Parser/riff.o Wave-Parser/wave.o comstatus.o
HEADERS=SerialPort/SerialPort.hpp comstatus.h Wave-Parser/riff.h Wave-Parser/wave.h Wave-Parser/wave_endian.h

# REMOVE=out.txt

CXX=C:/Strawberry/c/bin/g++
CXXFLAGS=--std=c++11 -Wall -lws2_32 -lpthread -mthreads -pthread


# Basic Commands

all: clean ${EXE}

${EXE}: ${SOURCES} ${HEADERS}
	${CXX} -o $@ ${SOURCES} ${CXXFLAGS}

debug: ${SOURCES} ${HEADERS}
	${CXX} -g ${SOURCES} ${CXXFLAGS}

clean:
	rm -f ${SOURCES} ${EXE} ${REMOVE}

