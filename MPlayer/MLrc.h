#pragma once


#include <QList>


class LyricLine
{
public:
    LyricLine(int time, QString text):time(time), text(text){}
    int time;
    QString text;
};


bool processContent(const QString &content, QList<LyricLine>& mLines);
