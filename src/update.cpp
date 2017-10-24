#include "update.h"
#include "ui_update.h"

#include <QProcess>

#include "globals.h"

Update::Update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Update)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    QProcess * process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start("../script/update.sh");

    connect(process, &QProcess::readyRead, this, [this,process](){
        ui->plainTextEdit->appendPlainText(process->readAll());
    });

    connect(process, QOverload<int>::of(&QProcess::finished), this, [this](int){
        DEF_SETTINGS;
        bool autoreset = settings.value(SET_AUTORESET,false).toBool();
        if (autoreset)
        {
            int res = system( "sudo reboot" );
            Q_UNUSED(res);
        }
        else
        {
            ui->label->setText("Aktualizace dokončena\nRestartujte zařízení");
        }
    });
}

Update::~Update()
{
    delete ui;
}
