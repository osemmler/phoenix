#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QLabel>

namespace Ui {
class Widget;
}
class PropModel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    struct TWeatherData {
        QLabel * temp;
        QLabel * icon;
        QLabel * day;
        QString iconSvgData;
    };

    //int commCount;

    Ui::Widget *ui;
    PropModel * propModel;
    QProcess weatherLoadProcess;
    QProcess tempLoadProcess;
    QProcess checkUpdateProcess;
    QList<TWeatherData> weatherDataMap;
};

#endif // WIDGET_H
