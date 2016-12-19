#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>
#define printf(format, ...) qDebug("[%s:%s:%d]:" format, __FILE__, __PRETTY_FUNCTION__, __LINE__, ## __VA_ARGS__)

#endif
