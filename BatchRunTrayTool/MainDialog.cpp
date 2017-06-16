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

    if(!QFile::exists(item.executableFilename))
    {
        QMessageBox::critical(this, tr("Error"), tr("File Lost : %1").arg(item.executableFilename));
    }

    //QFileInfo info(item.executableFilename);
    //QProcess::startDetached(item.batFilename, QStringList(),info.path());

#ifdef WIN32
    DWORD nShow = item.hide ? SW_HIDE : SW_SHOW;
    ::ShellExecute(NULL, NULL, item.executableFilename.toStdWString().c_str(), NULL, NULL, nShow);
#endif
}

void MainDialog::onOpenConfigFolder(bool checked)
{
    Q_UNUSED(checked)
    QString path = QDir::currentPath() + "/config";
    QDesktopServices::openUrl(QUrl(path));
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
        QString application_path = QApplication::applicationFilePath();
        settings->setValue(application_name, application_path.replace("/", "\\"));
    }
    else
    {
        settings->remove(application_name);
    }
    delete settings;
}

void MainDialog::onReloadConfig(bool checked)
{
    Q_UNUSED(checked)
    qApp->exit(MainDialog::EXIT_CODE_REBOOT);
}

void MainDialog::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(QSystemTrayIcon::Trigger == reason ||
            QSystemTrayIcon::Context == reason)
    {
        m_trayIcon->contextMenu()->popup(QCursor::pos());
    }
}

void MainDialog::initUI()
{
    m_trayIconMenu = new QMenu(this);
    //QFont font = m_trayIconMenu->font();
    //font.setPointSize(font.pointSize() * 1.5);
    //m_trayIconMenu->setFont(font);
    QStringList outFolderList = listAllDirs("config");
    for(int i = 0; i < outFolderList.count(); ++i)
    {
        QString firstDirs = outFolderList.at(i);
        QMenu* firstMenu = 0;
        QFileInfo fi(firstDirs);
        if(firstDirs.contains(PROPERTY_SEPARATOR))
        {
            m_trayIconMenu->addSeparator();
        }
        else if(!isPropertyFile(firstDirs) && fi.isFile())
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

    QAction *exitAction = new QAction(tr("Exit"));
    exitAction->setIcon(QIcon(":/photo/exit.png"));
    connect(exitAction, &QAction::triggered, this, &QCoreApplication::quit);
    aboutMenu->addAction(exitAction);
    m_trayIconMenu->addMenu(aboutMenu);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    QIcon icon(":/photo/logo.png");
    m_trayIcon->setIcon(icon);

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainDialog::onActivated);

    setWindowIcon(icon);
    m_trayIcon->show();
}

void MainDialog::initConnect()
{

}

QString MainDialog::findFilePath(const QString &path)
{
    QDir dir(path);
    QStringList list = dir.entryList(QDir::Files);

    if(list.count() > 0)
    {
        return dir.absoluteFilePath(list.at(0));
    }
    else
    {
        return QString();
    }
}

QStringList MainDialog::listAllDirs(const QString &path)
{
    QDir dir(path);
    QStringList list;
    QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files);
    for(int i = 0; i < dirs.count(); ++i)
    {
        list.push_back(dir.absoluteFilePath(dirs.at(i)));
    }
    return list;
}

QIcon MainDialog::getPathLogo(const QString &path)
{
    QString logoPath = path + "/" + PROPERTY_LOGO;
    if(QFile::exists(logoPath))
    {
        return QIcon(logoPath);
    }
    else
    {
        return QIcon();
    }
}

QIcon MainDialog::getFileLogo(const QString &filename)
{
    QFileInfo info(filename);
    QIcon icon;
    if(QFile::exists(info.absolutePath() + "/" + info.baseName() + PROPERTY_LOGO))
    {
        icon = QIcon(info.absolutePath() + "/" + info.baseName() + PROPERTY_LOGO);
    }
    else if(QFile::exists(info.absolutePath() + "/" + info.fileName() + PROPERTY_LOGO))
    {
        icon = QIcon(info.absolutePath() + "/" + info.fileName() + PROPERTY_LOGO);
    }
    else
    {
        QFileIconProvider provider;
        icon = provider.icon(info);
    }
    return icon;
}

QMenu *MainDialog::createMenu(const QString &path)
{
    QDir sencondFolderDir(path);
    QString dirName = sencondFolderDir.dirName();
    QMenu *secondMenu = new QMenu(sencondFolderDir.dirName(), this);
    secondMenu->setIcon(getPathLogo(path));
    QStringList folderList = listAllDirs(path);
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
            if(!isPropertyFile(secondDirs) && fi.isFile())
            {
                createActionWithFolder(secondMenu, secondDirs);
            }
            else if(fi.isDir())
            {
                QMenu *thirdMenu = new QMenu(secondFolderDir.dirName(), this);
                QStringList thirdFolderList = listAllDirs(secondDirs);
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
                        if(!isPropertyFile(thirdDirs) && newFi.isFile())
                        {
                            createActionWithFolder(thirdMenu, thirdDirs);
                        }
                    }
                }
                if(thirdMenu)
                {
                    thirdMenu->setIcon(getPathLogo(secondDirs));
                    secondMenu->addMenu(thirdMenu);
                }
            }
        }
    }
    return secondMenu;
}

//check if it's a property file
bool MainDialog::isPropertyFile(const QString &dirname)
{
    int count = _countof(PROPERTYS);
    for(int i = 0; i < count; ++i)
    {
        if(dirname.endsWith(PROPERTYS[i]))
        {
            return true;
        }
    }
    return false;
}

QAction* MainDialog::createActionWithFolder(QMenu* menu, const QString &filename)
{
    QAction *action = 0;
    QFileInfo fi(filename);
    if(fi.exists())
    {
        action = new QAction(fi.baseName());
        action->setIcon(getFileLogo(filename));
        connect(action, &QAction::triggered, this, &MainDialog::onActionTrigger);

        SExecItem item;
        item.executableFilename = filename;
        item.hide = !QFile::exists(fi.absolutePath() + "/" + fi.baseName() + PROPERTY_SHOW);
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
    if(settings.value(application_name).toString().compare(application_path.replace("/", "\\")) == 0)
    {
        m_autoStart->setChecked(true);
    }
    else
    {
        m_autoStart->setChecked(false);
    }
}
