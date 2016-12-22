TEMPLATE = subdirs

SUBDIRS += project/printer
SUBDIRS += test

message($$escape_expand("QT:$${QT}"))
