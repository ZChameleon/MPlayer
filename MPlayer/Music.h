#pragma once


#include <QString>
#include <QImage>


class Music {

public:
    Music();

public:
    bool load(const QString& filePath);

private:
    bool loadAsMp3(const QString& filePath);

public:
    QString m_name;
    QString m_artist;
    QString m_album;
    QImage m_apic;
};
