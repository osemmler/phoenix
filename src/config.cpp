#include "config.h"
#include "ui_config.h"

#include <QFile>
#include <QDir>
#include <QDebug>

#include "globals.h"
#include "backlight.h"

Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    DEF_SETTINGS;

    connect(ui->checkBoxAutoBright, &QCheckBox::toggled, [this](bool checked){
        DEF_SETTINGS;
        settings.setValue(SET_AUTOBRIGHT,checked);
        ui->horizontalSliderBright->setEnabled( !checked );
    });
    ui->checkBoxAutoBright->setChecked( settings.value(SET_AUTOBRIGHT,false).toBool() );

    ui->horizontalSliderBright->setValue( Backlight::Instance().getValue() );
    connect(ui->horizontalSliderBright, &QSlider::valueChanged, this, [](int value){
        if (!Backlight::Instance().setValue(value)) qCritical() << "Can't set backlight value";
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
