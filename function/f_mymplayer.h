#ifndef F_MYMPLAYER_H
#define F_MYMPLAYER_H

#include <QObject>
#include <cstdio>
#include <cstring>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <Qlabel>
#include <QDir>
#include <QStringList>

#include "common_use.h"

class F_mymplayer : public QObject
{
    Q_OBJECT
public:
    explicit F_mymplayer(QObject *parent = nullptr);
    int getDayTime();
    QStringList is_Mp3File(QStringList list);

};

#endif // F_MYMPLAYER_H
