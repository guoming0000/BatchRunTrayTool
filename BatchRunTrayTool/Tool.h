#ifndef TOOL_H
#define TOOL_H

#include <QWidget>

class CTool
{
public:
    static QString NicerName(const QString& filename);
    static QIcon GetFileLogo(const QString& filename);
    static QIcon GetPathLogo(const QString& path);
    static bool IsPropertyFile(const QString& dirname);
    static QStringList ListAllDirs(const QString& path);
    static QString GetExeDir(const QString& filename);
private:
    CTool(){}

};

#endif // TOOL_H
