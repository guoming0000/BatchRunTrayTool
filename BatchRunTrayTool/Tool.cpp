#include "Tool.h"
#include "ConstValue.h"
#include <QtCore>
#include <QFileIconProvider>

QString CTool::NicerName(const QString &filename)
{
    QString file = filename;
    file.remove(".lnk");
    QFileInfo info(file);
    return info.completeBaseName();
}

QIcon CTool::GetFileLogo(const QString &filename)
{
    QFileInfo info(filename);
    QIcon icon;
    if(QFile::exists(info.absolutePath() + "/" + info.baseName() + PROPERTY_LOGO))
    {
        icon = QIcon(info.absolutePath() + "/" + info.baseName() + PROPERTY_LOGO);
    }
    else if(QFile::exists(info.absolutePath() + "/" + info.completeBaseName() + PROPERTY_LOGO))
    {
        icon = QIcon(info.absolutePath() + "/" + info.completeBaseName() + PROPERTY_LOGO);
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

QIcon CTool::GetPathLogo(const QString &path)
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

//check if it's a property file
bool CTool::IsPropertyFile(const QString &dirname)
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

QStringList CTool::ListAllDirs(const QString &path)
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

QString CTool::GetExeDir(const QString &filename)
{
    QString dir;
    QFileInfo info(filename);
    if(info.isSymLink())
    {
        QString newFilename = info.symLinkTarget();
        dir = QFileInfo(newFilename).canonicalPath();
    }
    else
    {
        dir = QFileInfo(filename).canonicalPath();
    }
    return dir;
}


