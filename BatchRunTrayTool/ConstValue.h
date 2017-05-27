#ifndef CONSTVALUE_H
#define CONSTVALUE_H
#include <QtCore>
struct SExecItem
{
    SExecItem()
    {
        hide = false;
    }

    QString executableFilename;
    bool hide;
    QList<SExecItem*> list;
};

const QString PROPERTY_SHOW = "__show__";
const QString PROPERTY_LOGO = "__logo__.png";
const QString PROPERTY_EXIT_LOGO = "__exit__.png";
const QString PROPERTY_SEPARATOR = "__separator__";
#endif // CONSTVALUE_H
