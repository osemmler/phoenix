#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QCommandLineParser>

#include <iostream>

#include "globals.h"
#include "systemconfig.h"   // run qmake to generate this file

void qtMessageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    static QFile logFile("/tmp/phoenix.log");

    QString typeString;
    switch (type)
    {
    case QtDebugMsg:
        typeString = "[D]";
        break;
    case QtInfoMsg:
        typeString = "[I]";
        break;
    case QtWarningMsg:
        typeString = "[W]";
        break;
    case QtCriticalMsg:
        typeString = "[C]";
        break;
    case QtFatalMsg:
        typeString = "[F]";
    default:
        typeString = "[?]";
    }

    QString outputMsg = QString("%1 %2 : %3\n").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz")).arg(typeString).arg(msg);
    if(logFile.isOpen() || logFile.open(QIODevice::Append)) logFile.write(outputMsg.toUtf8());
    std::cout << outputMsg.toStdString() << std::flush;
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(qtMessageHandler);

    QApplication app(argc, argv);
    QApplication::setOrganizationName("MONSTER");
    QApplication::setApplicationName("Pheonix");
    QApplication::setApplicationVersion(GIT_VERSION);

    QList<QCommandLineOption> options = {
        {{"f", "fullscreen"},QCoreApplication::translate("main", "Show fullscreen.")},
    };

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addOptions(options);
    parser.process(app);

    if (!resetApplicationStyle())
    {
        qCritical() << "Can't set application style.";
    }

    Widget w;
    if (parser.isSet("f")) w.showFullScreen();
    else w.show();

    return app.exec();
}
