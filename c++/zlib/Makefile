PREFIX=.

INCLUDE=-I${PREFIX}/include

LIB=${PREFIX}/lib/libz.a

CXX=g++
CXXFLAG = -g -Wall -O3 --std=c++11

all: main

main: main.o decompressor.o
	$(CXX) Decompressor.o -o exp_zlib ${CXXFLAG} main.cpp ${LIB}

main.o: main.cpp
	$(CXX) ${CXXFLAG} -c main.cpp

decompressor.o: Decompressor.cpp
	$(CXX) ${CXXFLAG} -c Decompressor.cpp

clean:
	rm exp_zlib
	rm main.o
	rm Decompressor.o
