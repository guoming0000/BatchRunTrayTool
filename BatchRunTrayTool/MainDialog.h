#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>

#include <QSystemTrayIcon>

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
private:
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;


};
//! [0]

#endif // QT_NO_SYSTEMTRAYICON

#endif // MAINDIALOG_H
