TEMPLATE    = app
QT         += opengl

LIBS += -lGLEW
LIBS += -L/usr/local/Cellar/glew/1.11.0/lib -lGLEW

INCLUDEPATH +=  /usr/include
INCLUDEPATH += /usr/local/Cellar/glew/1.11.0/include
INCLUDEPATH += ../Model

HEADERS += MyGLWidget.h

SOURCES += main.cpp \
        MyGLWidget.cpp
