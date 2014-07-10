OPENCV_DIR = `pkg-config --cflags opencv` -I/usr/include/opencv2
OPENCV_LIBS = `pkg-config --libs opencv`
CFLAGS = -Wall -ggdb

all : imagify

imagify : imagify.cpp
	g++ ${CFLAGS} $(OPENCV_DIR) $(OPENCV_LIBS) -o imagify imagify.cpp detector.cpp

detector.o : detector.cpp detector.hpp
	g++ ${CFLAGS} detector.cpp

