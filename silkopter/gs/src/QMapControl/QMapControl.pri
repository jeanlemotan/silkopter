win32 {
LIBS += -L../bin -lqmapcontrol0
}
else {
LIBS += -L../bin -lqmapcontrol
}
INCLUDEPATH += ../../src/

QT += network
