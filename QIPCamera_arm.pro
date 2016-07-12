#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T15:52:58
#
#-------------------------------------------------

QT       -= gui

TARGET = QIPCamera
TEMPLATE = lib

DEFINES += QIPCAMERA_LIBRARY

SOURCES += qipcamera.cpp \
    dvo_ipcamera.cpp \
    ipcamera.cpp

HEADERS += qipcamera.h\
        qipcamera_global.h \
    dvo_ipcamera.h \
    ipcamera.h


INCLUDEPATH += ../QSlidingWindow
INCLUDEPATH += ../QSlidingWindowConsume

#ubuntu
#unix:!macx{
#LIBS += -L/usr/local/linux_lib/lib  -lQSlidingWindow -lQSlidingWindowConsume
#}
#arm
unix:!macx{
LIBS += -L/usr/local/arm_lib  -lQSlidingWindow -lQSlidingWindowConsume
}
#mac
unix:macx{
LIBS += -L/usr/local/lib  -lQSlidingWindow -lQSlidingWindowConsume
}

#install
#ubuntu
#unix:!macx {
#    target.path = /usr/local/linux_lib/lib
#    INSTALLS += target
#}
#arm
unix:!macx {
    target.path = /usr/local/arm_lib
    INSTALLS += target
}
#mac
unix:macx {
    target.path = /usr/local/lib
    INSTALLS += target
}
