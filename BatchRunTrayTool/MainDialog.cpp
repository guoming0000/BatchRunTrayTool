#include "MainDialog.h"
#include <QDir>
#include <QDebug>
#include <QMenu>
#include <QProcess>
#include <QCoreApplication>
#ifdef WIN32
#include <Windows.h>
#endif
MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent)
{
    initUI();
    initConnect();

}

MainDialog::~MainDialog()
{
}

#include <tchar.h>
void MainDialog::onActionTrigger(bool checked)
{
    qDebug()<<"on Action Trigger";
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action) return;
    SExecItem item = m_actionHash.value(action);
    if(item.batFilename.isEmpty()) return;
    QFileInfo info(item.batFilename);

    //QProcess::startDetached(item.batFilename, QStringList(),info.path());

#ifdef WIN32
    DWORD nShow = item.hide ? SW_HIDE : SW_SHOW;
    ::ShellExecute(NULL, NULL, item.batFilename.toStdWString().c_str(), NULL, NULL, nShow);

#endif
}

void MainDialog::initUI()
{
    m_trayIconMenu = new QMenu(this);
    QStringList outFolderList = findFolders("config");
    for(int i = 0; i < outFolderList.count(); ++i)
    {
        QString outFolderName = outFolderList.at(i);
        QDir folderDir(outFolderName);
        QString dirName = folderDir.dirName();;
        if(dirName.count() < 2) continue;
        if(dirName.count() == 2)
        {
            m_trayIconMenu->addSeparator();
            m_trayIconMenu->addSection("add a section");
        }
        else
        {
            QString batPath = findBatFilePath(outFolderName);
            if(!batPath.isEmpty())
            {
                QAction *action = new QAction(dirName.remove(0, 2));
                action->setIcon(getPathLogo(outFolderName));
                m_trayIconMenu->addAction(action);
                connect(action, &QAction::triggered, this, &MainDialog::onActionTrigger);
                SExecItem item;
                item.batFilename = batPath;
                item.hide = !QFile::exists(outFolderName + "/show.txt");
                m_actionHash.insert(action, item);
            }
            else
            {
                //enter deeper folder

            }
        }
    }

    //add exit action
    QAction *action = new QAction(tr("Exit"));
    action->setIcon(QIcon("config/exit.png"));
    connect(action, &QAction::triggered, this, &QCoreApplication::quit);
    m_trayIconMenu->addAction(action);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    QIcon icon("config/logo.png");
    m_trayIcon->setIcon(icon);

    setWindowIcon(icon);
    m_trayIcon->show();
}

void MainDialog::initConnect()
{

}

QString MainDialog::findBatFilePath(const QString &path)
{
    QDir dir(path);
    QStringList list = dir.entryList(QStringList()<<"*.bat");
    if(list.count() > 0)
    {
        return dir.absoluteFilePath(list.at(0));
    }
    else
    {
        return QString();
    }
}

QStringList MainDialog::findFolders(const QString &path)
{
    QDir dir(path);
    QStringList list;
    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < folders.count(); ++i)
    {
        list.push_back(dir.absoluteFilePath(folders.at(i)));
    }
    return list;
}

QIcon MainDialog::getPathLogo(const QString &path)
{
    QString logoPath = path + "/logo.png";
    if(QFile::exists(logoPath))
    {
        return QIcon(logoPath);
    }
    else
    {
        return QIcon();
    }
}
