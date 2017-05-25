#ifndef CONSTVALUE_H
#define CONSTVALUE_H
#include <QtCore>
struct SExecItem
{
    SExecItem()
    {
        hide = false;
    }

    QString batFilename;
    bool hide;
};

#endif // CONSTVALUE_H
