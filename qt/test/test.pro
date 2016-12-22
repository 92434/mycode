QT += widgets network printsupport

TARGET = test
TEMPLATE = app

message($$escape_expand("QT:$${QT}"))

SOURCES += \
    main.cpp \
    dialog.cpp \
    ../app/single_application.cpp

HEADERS += \
    dialog.h \
    ../app/single_application.h

