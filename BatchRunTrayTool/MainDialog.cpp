#include "MainDialog.h"
#include <QDir>
#include <QDebug>
#include <QMenu>
#include <QProcess>
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QFileIconProvider>
#include <QDesktopServices>
#include "Tool.h"

#ifdef WIN32
#include <Windows.h>
#include <tchar.h>
#endif

int const MainDialog::EXIT_CODE_REBOOT = -123456789;

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent)
{
    initUI();
    initConnect();
}

MainDialog::~MainDialog()
{
}

void MainDialog::onActionTrigger(bool checked)
{
    Q_UNUSED(checked)
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action) return;
    SExecItem item = m_actionHash.value(action);
    if(item.executableFilename.isEmpty()) return;

    //QFileInfo info(item.executableFilename);
    //QProcess::startDetached(item.batFilename, QStringList(),info.path());

    processItem(item);
}

void MainDialog::processItem(const SExecItem &item)
{
    if(!QFile::exists(item.executableFilename))
    {
        QMessageBox::critical(this, tr("Error"), tr("File Lost : %1").arg(item.executableFilename));
        return;
    }
#ifdef WIN32
    DWORD nShow = item.hide ? SW_HIDE : SW_SHOW;

    ::ShellExecute(NULL, NULL, item.executableFilename.toStdWString().c_str(),
                   NULL, CTool::GetExeDir(item.executableFilename).toStdWString().c_str(), nShow);
#endif

    if(m_retryMode)
    {
        m_lastItem = item;

        //change icon
        QIcon icon = CTool::GetFileLogo(m_lastItem.executableFilename);
        if(icon.isNull())
        {
            icon = m_defaultRetryIcon;
        }
        m_trayIcon->setIcon(icon);
        updateTooltip();
    }
}

void MainDialog::updateTooltip()
{
    QString tooltip;
    if(!m_lastItem.executableFilename.isEmpty() && m_retryMode)
    {
        tooltip = tr("LeftClick: ") + QFileInfo(m_lastItem.executableFilename).completeBaseName() + "\n" +
                tr("MidClick: Clear LeftClick") + "\n" +
                tr("RightClick: Open Menu");
    }
    else
    {
        tooltip = tr("RightClick: Open Menu");
    }
    m_trayIcon->setToolTip(tooltip);
}

void MainDialog::clearRetryMode()
{
    m_lastItem.executableFilename.clear();
    updateTooltip();
    m_trayIcon->setIcon(m_logo);
}

void MainDialog::onOpenConfigFolder(bool checked)
{
    Q_UNUSED(checked)
    QString path = QApplication::applicationDirPath() + "/config";
    QDesktopServices::openUrl(QUrl("file:///" + path, QUrl::TolerantMode));
}

void MainDialog::onProjectPage(bool checked)
{
    Q_UNUSED(checked)
    QDesktopServices::openUrl(QUrl("https://github.com/guoming0000/BatchRunTrayTool"));
}

void MainDialog::onFeedback(bool checked)
{
    Q_UNUSED(checked)
    QDesktopServices::openUrl(QUrl("https://github.com/guoming0000/BatchRunTrayTool/issues"));
}

void MainDialog::onUsage(bool checked)
{
    Q_UNUSED(checked)
    QDesktopServices::openUrl(QUrl("https://github.com/guoming0000/BatchRunTrayTool/blob/master/README.md"));
}

void MainDialog::onAutoStart(bool checked)
{
    QString application_name = QApplication::applicationName();
    QSettings *settings = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(checked)
    {
        QString application_path = QDir::toNativeSeparators(QApplication::applicationFilePath());
        settings->setValue(application_name, application_path);
    }
    else
    {
        settings->remove(application_name);
    }
    delete settings;
}

void MainDialog::onSetRetryMode(bool checked)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "BrightGuo", QApplication::applicationName());
    if(checked)
    {
        settings.setValue("UseRetry", true);
        m_retryMode = true;
    }
    else
    {
        settings.setValue("UseRetry", false);
        m_retryMode = false;
        clearRetryMode();
    }
}

void MainDialog::onReloadConfig(bool checked)
{
    Q_UNUSED(checked)
    qApp->exit(MainDialog::EXIT_CODE_REBOOT);
}

void MainDialog::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Context://right click
        {
            m_trayIcon->contextMenu()->popup(QCursor::pos());
            break;
        }
        case QSystemTrayIcon::Trigger://left click
        {
            if(!m_lastItem.executableFilename.isEmpty() && m_retryMode)
            {
                processItem(m_lastItem);
            }
            break;
        }
        case QSystemTrayIcon::MiddleClick:
        {
            if(m_retryMode)
            {
                clearRetryMode();
            }
            break;
        }
    }
}

void MainDialog::initUI()
{
    m_retryMode = false;
    m_trayIconMenu = new QMenu(this);
    //QFont font = m_trayIconMenu->font();
    //font.setPointSize(font.pointSize() * 1.5);
    //m_trayIconMenu->setFont(font);
    QStringList outFolderList = CTool::ListAllDirs(QApplication::applicationDirPath() + "/config");
    for(int i = 0; i < outFolderList.count(); ++i)
    {
        QString firstDirs = outFolderList.at(i);
        QMenu* firstMenu = 0;
        QFileInfo fi(firstDirs);
        if(firstDirs.contains(PROPERTY_SEPARATOR))
        {
            m_trayIconMenu->addSeparator();
        }
        else if(!CTool::IsPropertyFile(firstDirs) && fi.isFile())
        {
            createActionWithFolder(m_trayIconMenu, firstDirs);
        }
        else if(fi.isDir())
        {
            firstMenu = createMenu(firstDirs);
        }
        if(firstMenu)
        {
            m_trayIconMenu->addMenu(firstMenu);
        }
    }

    //add exit action
    QMenu *aboutMenu = new QMenu(tr("About") ,this);
    aboutMenu->setIcon(QIcon(":/photo/about.png"));

    QAction *openConfig = new QAction(tr("Open Config Folder"));
    openConfig->setIcon(QIcon(":/photo/folder.png"));
    connect(openConfig, &QAction::triggered, this, &MainDialog::onOpenConfigFolder);
    aboutMenu->addAction(openConfig);

    QAction *reloadConfig = new QAction(tr("Reload Config"));
    reloadConfig->setIcon(QIcon(":/photo/reload_config.png"));
    connect(reloadConfig, &QAction::triggered, this, &MainDialog::onReloadConfig);
    aboutMenu->addAction(reloadConfig);

    QAction *projectPageAction = new QAction(tr("Project Page"));
    projectPageAction->setIcon(QIcon(":/photo/project_page.png"));
    connect(projectPageAction, &QAction::triggered, this, &MainDialog::onProjectPage);
    aboutMenu->addAction(projectPageAction);

    QAction *feedbackAction = new QAction(tr("Feedback"));
    feedbackAction->setIcon(QIcon(":/photo/feedback.png"));
    connect(feedbackAction, &QAction::triggered, this, &MainDialog::onFeedback);
    aboutMenu->addAction(feedbackAction);

    QAction *usageAction = new QAction(tr("Usage"));
    usageAction->setIcon(QIcon(":/photo/usage.png"));
    connect(usageAction, &QAction::triggered, this, &MainDialog::onUsage);
    aboutMenu->addAction(usageAction);

    m_autoStart = new QAction(tr("AutoStart"));
    m_autoStart->setCheckable(true);
    connect(m_autoStart, &QAction::triggered, this, &MainDialog::onAutoStart);
    aboutMenu->addAction(m_autoStart);
    initAutoStartAction();

    m_retryAction = new QAction(tr("Retry Mode"));
    m_retryAction->setCheckable(true);
    connect(m_retryAction, &QAction::triggered, this, &MainDialog::onSetRetryMode);
    aboutMenu->addAction(m_retryAction);
    initRetryAction();

    QAction *exitAction = new QAction(tr("Exit"));
    exitAction->setIcon(QIcon(":/photo/exit.png"));
    connect(exitAction, &QAction::triggered, this, &QCoreApplication::quit);
    aboutMenu->addAction(exitAction);
    m_trayIconMenu->addMenu(aboutMenu);
    m_trayIconMenu->setContextMenuPolicy(Qt::ActionsContextMenu);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_logo =  QIcon(":/photo/logo.png");
    m_trayIcon->setIcon(m_logo);

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainDialog::onActivated);

    setWindowIcon(m_logo);
    m_trayIcon->show();

    m_defaultRetryIcon = QIcon(":/photo/retry.png");
    updateTooltip();
}

void MainDialog::initConnect()
{

}


QMenu *MainDialog::createMenu(const QString &path)
{
    QDir sencondFolderDir(path);
    //QString dirName = sencondFolderDir.dirName();
    QMenu *secondMenu = new QMenu(sencondFolderDir.dirName(), this);
    secondMenu->setIcon(CTool::GetPathLogo(path));
    QStringList folderList = CTool::ListAllDirs(path);
    for(int i = 0; i < folderList.count(); ++i)
    {
        QString secondDirs = folderList.at(i);
        QDir secondFolderDir(secondDirs);
        QFileInfo fi(secondDirs);
        if(secondDirs.contains(PROPERTY_SEPARATOR))
        {
            secondMenu->addSeparator();
        }
        else
        {
            if(!CTool::IsPropertyFile(secondDirs) && fi.isFile())
            {
                createActionWithFolder(secondMenu, secondDirs);
            }
            else if(fi.isDir())
            {
                QMenu *thirdMenu = new QMenu(secondFolderDir.dirName(), this);
                QStringList thirdFolderList = CTool::ListAllDirs(secondDirs);
                for(int i = 0; i < thirdFolderList.count(); ++i)
                {
                    QString thirdDirs = thirdFolderList.at(i);
                    QFileInfo newFi(thirdDirs);
                    if(thirdDirs.contains(PROPERTY_SEPARATOR))
                    {
                        thirdMenu->addSeparator();
                    }
                    else
                    {
                        if(!CTool::IsPropertyFile(thirdDirs) && newFi.isFile())
                        {
                            createActionWithFolder(thirdMenu, thirdDirs);
                        }
                    }
                }
                if(thirdMenu)
                {
                    thirdMenu->setIcon(CTool::GetPathLogo(secondDirs));
                    secondMenu->addMenu(thirdMenu);
                }
            }
        }
    }
    return secondMenu;
}


QAction* MainDialog::createActionWithFolder(QMenu* menu, const QString &filename)
{
    QAction *action = 0;
    QFileInfo fi(filename);
    if(fi.exists())
    {
        action = new QAction(CTool::NicerName(filename));
        action->setIcon(CTool::GetFileLogo(filename));
        connect(action, &QAction::triggered, this, &MainDialog::onActionTrigger);

        SExecItem item;
        item.executableFilename = filename;

        ///////////////////////////////////////////////
        CTool::GetExeDir(filename);
        /////////////////////////

        item.hide = true;
        if(QFile::exists(fi.absolutePath() + "/" + fi.fileName() + PROPERTY_SHOW) ||
                QFile::exists(fi.absolutePath() + "/" + fi.baseName() + PROPERTY_SHOW) ||
                QFile::exists(fi.absolutePath() + "/" + fi.completeBaseName() + PROPERTY_SHOW))
        {
            item.hide = false;
        }

        m_actionHash.insert(action, item);
        if(menu)
        {
            menu->addAction(action);
        }
    }
    return action;
}

void MainDialog::initAutoStartAction()
{
    QString application_name = QApplication::applicationName();
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString application_path = QApplication::applicationFilePath();
    QString regeditPath = QDir::fromNativeSeparators(settings.value(application_name).toString());
    QString realPath = QDir::fromNativeSeparators(application_path);
    if(regeditPath.compare(realPath) == 0)
    {
        m_autoStart->setChecked(true);
    }
    else
    {
        m_autoStart->setChecked(false);
    }
}

void MainDialog::initRetryAction()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "BrightGuo", QApplication::applicationName());
    if(settings.value("UseRetry").toBool())
    {
        m_retryAction->setChecked(true);
        m_retryMode = true;
    }
    else
    {
        m_retryAction->setChecked(false);
        m_retryMode = false;
    }
}
