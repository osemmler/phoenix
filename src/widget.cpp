#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSvgRenderer>
#include <QPainter>
#include <QDateTime>
#include <QFileInfo>
#include <QRegExp>
#include <QTableView>

#include "config.h"
#include "update.h"
#include "globals.h"
//#include "comm.h"
#include "backlight.h"
#include "phoenix.h"
#include "propmodel.h"
#include "model.h"

inline QDebug &operator<<(QDebug d, const Message &msg)
{
    d.noquote() << "Message[ Type=" << QString().sprintf("0x%02X",msg.type) << ", ";
    d << "MonsterId=" << msg.monsterId << ", ";
    d << "PartId=" << msg.partId << ", ";
    d << "PropId=" << msg.propId << ", ";
    d << "ValueSize=" << msg.valueSize << ", ";
    d << "Value={";
    for(int i=0; i<msg.valueSize; i++)
    {
        d <<  QString().sprintf("0x%02X", (&msg.value)[i] ).trimmed();
        d << (i<msg.valueSize-1 ? "," : "" );
    }
    d << "} ]";
    return d;
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    ui->pushButtonUpdate->setVisible(false);

    ui->labelVersion->setText(QApplication::applicationVersion());

    weatherDataMap.append({ui->labelTemp0, ui->labelWeather0, ui->labelDay0, QString()});
    weatherDataMap.append({ui->labelTemp1, ui->labelWeather1, ui->labelDay1, QString()});
    weatherDataMap.append({ui->labelTemp2, ui->labelWeather2, ui->labelDay2, QString()});
    weatherDataMap.append({ui->labelTemp3, ui->labelWeather3, ui->labelDay3, QString()});
    weatherDataMap.append({ui->labelTemp4, ui->labelWeather4, ui->labelDay4, QString()});

    propModel = new PropModel(this);
    Phoenix * p = new Phoenix;
    p->setup();

    //commCount = 0;
    /*Comm::Instance().f = [this]()
    {
        commCount++;
        std::string str(Comm::Instance().data);
        ui->labelTempIn->setText(QString::fromStdString(str)+"°");
    };*/

    auto runUpdate =[this](){
        Update update(this);
        update.setGeometry( this->geometry() );
        update.exec();
    };

    auto updateWeatherIcons = [this](){
        for(int i=0; i<5; i++)
        {
            QString svgData = weatherDataMap[i].iconSvgData;
            QSvgRenderer render(svgData.replace("#d09683",StyleDefs::map["@text"]).toUtf8());
            QPixmap pm(80, 80);
            pm.fill(QColor(StyleDefs::map["@background"]));
            QPainter painter(&pm);
            render.render(&painter, pm.rect());
            weatherDataMap[i].icon->setPixmap(pm);
        }
    };


    // load weather
    connect(&weatherLoadProcess, QOverload<int>::of(&QProcess::finished), this, [this,updateWeatherIcons](int exitCode){
        qDebug() << "Weather update process finished with code " << exitCode;
        QString out = weatherLoadProcess.readAll();

        if (exitCode !=0)
        {
            qCritical() << "Error while running weather process.";
            qDebug() << out;
            return;
        }

        QStringList days = out.split('\n',QString::SkipEmptyParts);
        if (days.size()!=5) //5days
        {
            qCritical() << "Can't parse weather.";
            qDebug() << out;
            return;
        }

        int dayOfWeek = QDateTime::currentDateTime().date().dayOfWeek();

        for(int i=0; i<5; i++)
        {
            QStringList data = days.at(i).split('#',QString::SkipEmptyParts);

            QString temp = data.at(0) + "°/" + data.at(1) + "°";
            QString icon = data.at(2);
            weatherDataMap[i].temp->setText(temp);
            weatherDataMap[i].icon->setText(icon);
            weatherDataMap[i].day->setText( i==0 ? "DNES" : QLocale::system().dayName(dayOfWeek).toUpper() );
            dayOfWeek = dayOfWeek==7 ? 1 : dayOfWeek+1;

            QFile iconFile(":/weather_icons/icons/weather/"+icon);
            if (iconFile.exists() && iconFile.open(QIODevice::ReadOnly))
                weatherDataMap[i].iconSvgData = iconFile.readAll();
        }

        updateWeatherIcons();
        ui->labelWeatherUpdateTime->setText( "Poslední aktualizace: " + QDateTime::currentDateTime().toString("h:mm d.M.") );
    });

    // load temp
    connect(&tempLoadProcess, QOverload<int>::of(&QProcess::finished), this, [this](int exitCode){
        qDebug() << "Temp update process finished with code " << exitCode;
        QString out = tempLoadProcess.readAll();

        if (exitCode !=0)
        {
            qCritical() << "Error while running temp process.";
            qDebug() << out;
            return;
        }

        double temp = out.toDouble();

        ui->labelTempOut->setText( QString::number(temp,'f',1)+"°" );
    });

    connect(&checkUpdateProcess, QOverload<int>::of(&QProcess::finished), this, [this,runUpdate](int exitCode){
        qDebug() << "Check update process finished with code " << exitCode;
        bool visible = false;

        if (exitCode == 1)
        {
            DEF_SETTINGS;
            bool autoupdate = settings.value(SET_AUTOUPDATE,false).toBool();
            if (autoupdate) runUpdate();
            else visible = true;
        }

        ui->pushButtonUpdate->setVisible( visible );
    });

    // load namedays
    QFile file(":/json/svatky.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        qCritical() << "Can't open namedays file.";
        return;
    }
    QByteArray ba = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(ba);
    QJsonObject obj = doc.object();

    // function to get current nameday
    auto getNameDay = [obj](){
        QString nameday = "-";
        QString nameDayKey = QDateTime::currentDateTime().toString("ddMM");
        if (obj[nameDayKey].isArray())
        {
            nameday = "";
            QJsonArray arr = obj[nameDayKey].toArray();
            for(int i=0; i<arr.size(); i++)
            {
                nameday += arr.at(i).toString();
                if (i!=arr.size()-1) nameday += " & ";
            }
        }
        else
        {
            nameday = obj[nameDayKey].toString();
        }
        return nameday;
    };

    auto updateWeather = [this](){
        qDebug() << "Updating weather - start process";
        weatherLoadProcess.start("../script/weather.py");
    };

    auto backlightDesiredValue = [this](){
        int night = 8;
        int day = 40;
        int evening = 15;
        int hour = QTime::currentTime().hour();
        if (hour < 7) return night;
        if (hour < 18) return day;
        if (hour < 21) return evening;
        return night;
    };

    // main timer loop
    auto timerTimeout = [this,getNameDay,updateWeather,backlightDesiredValue,p](){

        Message msg;
        while(p->readMessage(msg))
        {
            qDebug() << msg;
            propModel->update(msg);
            //printf("IN: ");
            //for(int i=0; i<MSG_SIZE; i++) printf("%02X ",msg[i]);
            //printf("\n");
        }

        DEF_SETTINGS;
        if (settings.value(SET_AUTOBRIGHT,false).toBool())
        {
            int curr_value = Backlight::Instance().getValue();
            if (curr_value == -1)
            {
                qCritical() << "Can't read current backlight value.";
            }
            else
            {
                int des_value = backlightDesiredValue();
                if (des_value != curr_value)
                {
                    qDebug() << "Changing backlight to new auto value =" << des_value;
                    if (!Backlight::Instance().setValue( des_value ))
                        qCritical() << "Can't change backlight to new auto value";
                }
            }
        }

        QString time = QDateTime::currentDateTime().toString("h:mm");
        if (time != ui->labelTime->text())
        {
            qDebug() << "Updating time";
            ui->labelTime->setText(time);

            qDebug() << "Updating temp - start process";
            tempLoadProcess.start("../script/temp.py");

            qDebug() << "Check update - start process";
            checkUpdateProcess.start("../script/check_update.sh");

            /*if (commCount == 0)
            {
                ui->labelTempIn->setText("-");
            }
            else
            {
                commCount = 0;
            }*/

            int minute = QDateTime::currentDateTime().toString("m").toInt();
            if (ui->labelWeatherUpdateTime->text().isEmpty() || minute == 15 || minute == 45)
            {
                updateWeather();
            }

            QString date = QDateTime::currentDateTime().toString("ddd d.M.yyyy");
            if (date != ui->labelDate->text())
            {
                qDebug() << "Updating date";
                ui->labelDate->setText(date);

                qDebug() << "Updating nameday";
                QString nameday = getNameDay();
                ui->labelNameday->setText(nameday);
            }
        }
    };
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, timerTimeout);
    timer->start(1000);
    timerTimeout();

    connect(ui->pushButtonConfig, &QPushButton::clicked, this, [this,updateWeatherIcons](){
        bool styleChanged = false;
        Config config(this);
        connect(&config, &Config::styleChanged, this, [&styleChanged](){styleChanged=true;});
        config.setGeometry( this->geometry() );
        config.exec();
        if (styleChanged) updateWeatherIcons();
    });
    connect(ui->pushButtonModel, &QPushButton::clicked, this, [this](){
        Model m(this);
        m.setModel(propModel);
        m.setGeometry( this->geometry() );
        m.exec();
    });
    connect(ui->pushButtonUpdate, &QPushButton::clicked, this, [this,runUpdate](){
        runUpdate();
    });
}

Widget::~Widget()
{
    delete ui;
}
