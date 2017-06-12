QT += widgets network printsupport

TARGET = readimg
TEMPLATE = app

message($$escape_expand("QT:$${QT}"))

SOURCES += \
    main.cpp \
    dialog.cpp \
    ../../app/application.cpp

HEADERS += \
    dialog.h \
    ../../app/application.h

unix:!macx: QMAKE_CXXFLAGS += -I$$PWD/../../

RESOURCES += \
    rsrc.qrc
