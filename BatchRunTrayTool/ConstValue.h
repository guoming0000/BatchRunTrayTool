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
const QString PROPERTY_SEPARATOR = "__separator__";

//#define _countof(arr) (sizeof(arr) / sizeof((arr)[0]))

const QString PROPERTYS[] =
{
    PROPERTY_SHOW,
    PROPERTY_LOGO,
    PROPERTY_SEPARATOR,

};
#endif // CONSTVALUE_H
