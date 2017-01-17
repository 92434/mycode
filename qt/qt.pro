TEMPLATE = subdirs

SUBDIRS += project/printer \
    image
SUBDIRS += test

message($$escape_expand("QT:$${QT}"))
