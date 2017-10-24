#include "backlight.h"

#include <QProcess>
#include <QFile>

#define BACKLIGHT_ACTUAL    "/sys/class/backlight/rpi_backlight/actual_brightness"
#define BACKLIGHT_SET       "/sys/class/backlight/rpi_backlight/brightness"

Backlight & Backlight::Instance()
{
    static Backlight instance;
    return instance;
}

int Backlight::getValue() const
{
    QFile file(BACKLIGHT_ACTUAL);
    if (!file.exists()) return -1;

    QProcess p;
    p.start("cat",QStringList()<<BACKLIGHT_ACTUAL);
    p.waitForFinished();
    QString s = p.readAll();
    bool ok;
    int value = s.toInt(&ok);
    return ok ? value : -1;
}

bool Backlight::setValue(const int &value)
{
    QFile file(BACKLIGHT_SET);
    if (!file.exists()) return false;

    QString cmd = QString("echo %1 | sudo tee %2").arg(value).arg(BACKLIGHT_SET);
    int res = system( cmd.toUtf8() );
    Q_UNUSED(res);
    return true;
}

Backlight::Backlight()
{
}

Backlight::~Backlight()
{
}
