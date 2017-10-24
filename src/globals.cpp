#include "globals.h"

#include <QDebug>
#include <QFile>
#include <QApplication>

QMap<QString,QString> StyleDefs::map = QMap<QString,QString>();

bool resetApplicationStyle()
{
    /// read style definitions from .def file
    {
        DEF_SETTINGS;
        QString defsFilename = settings.value(SET_STYLE,"default").toString();
        QFile defsFile( "../styles/"+defsFilename+".def" );

        if (!defsFile.open( QFile::ReadOnly ))
        {
            qCritical() << QString("Can't open style definitions file '%1'").arg(defsFile.fileName());
            return false;
        }

        foreach(QString line, QString(defsFile.readAll()).split('\n',QString::SkipEmptyParts))
        {
            QStringList x = line.split('=');
            QString def = x.at(0).trimmed();
            QString value = x.at(1).trimmed();
            StyleDefs::map[def] = value;
        }

        defsFile.close();
    }

    /// read style sheet from style.qss file and set it to application
    {
        QFile styleSheetFile( "../styles/style.qss" );

        if (!styleSheetFile.open( QFile::ReadOnly ))
        {
            qCritical() << QString("Can't open style sheet file '%1'").arg(styleSheetFile.fileName());
            return false;
        }

        QString styleSheet = styleSheetFile.readAll();
        for(auto it=StyleDefs::map.begin(); it!=StyleDefs::map.end(); it++)
        {
            styleSheet.replace(it.key(),it.value());
        }

        qApp->setStyleSheet( styleSheet );
        return true;
    }
}
