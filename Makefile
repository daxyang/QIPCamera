os = macx
ifeq ($(os),macx)
TARGET=libQIPCamera.dylib
else
TARGET=libQIPCamera.so
endif

CC=g++

INCLUDE = -I../QSlidingWindow -I../QSlidingWindowConsume -I../QNetProtocol -I../GetDVOStream
LIB = -L/usr/local/lib -lQSlidingWindow -l QSlidingWindowConsume -lQNetProtocol
.PHONY:all

$(TARGET):ipcamera.o  dvo_ipcamera.o ant_ipcamera.o qipcamera.o
	$(CC) -shared -fPIC -o $(TARGET) $(LIB)  ipcamera.o  dvo_ipcamera.o ant_ipcamera.o qipcamera.o

ant_ipcamera.o:ant_ipcamera.cpp ant_ipcamera.h
	$(CC) $(INCLUDE) -c ant_ipcamera.cpp ant_ipcamera.h

dvo_ipcamera.o:dvo_ipcamera.cpp dvo_ipcamera.h
	$(CC) $(INCLUDE) -c dvo_ipcamera.cpp dvo_ipcamera.h

ipcamera.o:ipcamera.cpp ipcamera.h
	$(CC) $(INCLUDE) -c ipcamera.cpp ipcamera.h

qipcamera.o:qipcamera.cpp qipcamera.h
	$(CC) $(INCLUDE) -c qipcamera.cpp qipcamera.h

clean:
	rm -f *.o lib*.dylib lib*.so

install:
	cp $(TARGET) /usr/local/lib
ifeq ($(os),macx)
	ln -sf /usr/local/lib/$(TARGET) /usr/local/lib/libQIPCamera.1.dylib
endif
