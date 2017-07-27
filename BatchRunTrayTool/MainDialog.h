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
public:
    static int const EXIT_CODE_REBOOT;
protected slots:
    void onActionTrigger(bool checked);
    void onOpenConfigFolder(bool checked = false);
    void onReloadConfig(bool checked = false);
    void onProjectPage(bool checked = false);
    void onFeedback(bool checked = false);
    void onUsage(bool checked = false);
    void onAutoStart(bool checked = false);
    void onSetRetryMode(bool checked = false);
    void onActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void initUI();
    QMenu* createMenu(const QString& path);
    QAction* createActionWithFolder(QMenu* menu, const QString& filename);

    void processItem(const SExecItem& item);
    void updateTooltip();
    void clearRetryMode();

private:
    void initAutoStartAction();
    void initRetryAction();

private:
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;
    QHash<QAction*, SExecItem> m_actionHash;
    QAction *m_autoStart;
    QAction *m_retryAction;

    SExecItem m_lastItem;
    QIcon m_defaultRetryIcon;
    QIcon m_logo;
    bool m_retryMode;
};
//! [0]

#endif // QT_NO_SYSTEMTRAYICON

#endif // MAINDIALOG_H
