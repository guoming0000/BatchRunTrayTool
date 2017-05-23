#include "MainDialog.h"
#include <QDir>
#include <QDebug>
#include <QMenu>
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
    qDebug()<<"on Action Trigger";
}

void MainDialog::initUI()
{
    m_trayIconMenu = new QMenu(this);

    QFile menuListFile("config/menu_sort.txt");
    if(menuListFile.open(QFile::ReadOnly))
    {
        QTextStream in(&menuListFile);
        while(!in.atEnd())
        {
            QString str = in.readLine();qDebug()<<str<<str.size();
            if(str.isEmpty())
            {
                //add separator line
                m_trayIconMenu->addSeparator();
                m_trayIconMenu->addSection("add a section");
            }
            else
            {
                QAction *action = new QAction(str);
                m_trayIconMenu->addAction(m_trayIconMenu);
                connect(action, &QAction::triggered, this, MainDialog::onActionTrigger);
            }
        }
    }

    QDir dir("config");

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->show();
}

void MainDialog::initConnect()
{

}
