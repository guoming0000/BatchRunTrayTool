#include "MainDialog.h"
#include <QApplication>

#ifndef QT_NO_SYSTEMTRAYICON
#include <QMessageBox>
#include <QSystemTrayIcon>

int main(int argc, char *argv[])
{
    int currentExitCode = 0;

    do
    {
        QApplication app(argc, argv);

        if(QLocale::system().language() == QLocale::Chinese)
        {
            QTranslator *translator = new QTranslator;
            translator->load(QApplication::applicationDirPath() + "/batchRunTrayTool_zh_CN.qm");
            app.installTranslator(translator);
        }

        if (!QSystemTrayIcon::isSystemTrayAvailable()) {
            QMessageBox::critical(0, QObject::tr("Systray"),
                                  QObject::tr("I couldn't detect any system tray "
                                              "on this system."));
            return 1;
        }
        QApplication::setQuitOnLastWindowClosed(false);

        MainDialog window;
        window.close();
        currentExitCode = app.exec();
    }while(currentExitCode == MainDialog::EXIT_CODE_REBOOT);

    return currentExitCode;
}

#else

#include <QLabel>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString text("QSystemTrayIcon is not supported on this platform");

    QLabel *label = new QLabel(text);
    label->setWordWrap(true);

    label->show();
    qDebug() << text;

    app.exec();
}

#endif
