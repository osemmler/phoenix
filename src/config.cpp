#include "config.h"
#include "ui_config.h"

#include <QFile>
#include <QProcess>
#include <QDir>

#include "globals.h"

Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    DEF_SETTINGS;

    QProcess p;
    p.start("cat",QStringList()<<"/sys/class/backlight/rpi_backlight/actual_brightness");
    p.waitForFinished();
    QString s = p.readAll();
    int b = s.toInt();
    ui->horizontalSliderBright->setValue(b);
    connect(ui->horizontalSliderBright, &QSlider::valueChanged, this, [](int value){
        QFile file("/sys/class/backlight/rpi_backlight/brightness");
        if (file.exists())
        {
            QString cmd = QString("echo %1 | sudo tee /sys/class/backlight/rpi_backlight/brightness").arg(value);
            int res = system( cmd.toUtf8() );
            Q_UNUSED(res);
        }
    });

    ui->checkBoxAutoUpdate->setChecked( settings.value(SET_AUTOUPDATE,false).toBool() );
    connect(ui->checkBoxAutoUpdate, &QCheckBox::toggled, [this](bool checked){
        DEF_SETTINGS;
        settings.setValue(SET_AUTOUPDATE,checked);
    });

    ui->checkBoxAutoReset->setChecked( settings.value(SET_AUTORESET,false).toBool() );
    connect(ui->checkBoxAutoReset, &QCheckBox::toggled, [this](bool checked){
        DEF_SETTINGS;
        settings.setValue(SET_AUTORESET,checked);
    });

    ui->comboBoxStyle->clear();
    foreach(QString str, QDir("../styles","*.def",QDir::Name).entryList()) ui->comboBoxStyle->addItem(str.left(str.length()-4));
    ui->comboBoxStyle->setCurrentText( settings.value(SET_STYLE,"default").toString() );
    connect(ui->comboBoxStyle, &QComboBox::currentTextChanged, this, [this](QString style){
        DEF_SETTINGS;
        settings.setValue(SET_STYLE,style);
        resetApplicationStyle();
        emit styleChanged();
    });

    connect(ui->pushButtonBack, &QPushButton::clicked, this, &Config::accept);
}

Config::~Config()
{
    delete ui;
}
