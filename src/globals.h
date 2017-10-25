#ifndef GLOBALS_H
#define GLOBALS_H

#include <QMap>
#include <QString>
#include <QSettings>
#include <QApplication>

#define DEF_SETTINGS QSettings settings("phoenix.conf",QSettings::IniFormat)
#define SET_AUTOUPDATE      "autoupdate"
#define SET_AUTORESET       "autoreset"
#define SET_STYLE           "style"
#define SET_AUTOBRIGHT      "autobright"

class StyleDefs
{
public:
     static QMap<QString,QString> map;
};

bool resetApplicationStyle();

#endif // GLOBALS_H
