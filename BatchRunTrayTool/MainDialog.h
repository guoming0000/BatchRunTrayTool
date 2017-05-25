#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QHash>
#include <QSystemTrayIcon>
#include "ConstValue.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#ifndef QT_NO_SYSTEMTRAYICON
class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();
protected slots:
    void onActionTrigger(bool checked);
private:
    void initUI();
    void initConnect();

    QString findBatFilePath(const QString& path);
    QStringList findFolders(const QString& path);
    QIcon getPathLogo(const QString& path);
private:
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;
    QHash<QAction*, SExecItem> m_actionHash;

};
//! [0]

#endif // QT_NO_SYSTEMTRAYICON

#endif // MAINDIALOG_H
