#include "MLrc.h"


bool lyricTimeLessThan(const LyricLine& line1, const LyricLine& line2);


bool processContent(const QString& content, QList<LyricLine>& mLines) {

    if (!mLines.isEmpty()) {
        mLines.clear();
    }

    const QRegExp rx(R"(\[(\d+):(\d+(\.\d+)?)\])");

    auto pos = rx.indexIn(content);
    if (pos == -1) {
        auto list = content.split('\n', QString::SkipEmptyParts);
        foreach (const QString line, list)
            mLines.append(LyricLine(0, line));
    } else {
        QList<int> timeLabels;
        do {
            timeLabels << (rx.cap(1).toInt() * 60 + rx.cap(2).toDouble()) * 1000;
            const auto lastPos = pos + rx.matchedLength();
            pos = rx.indexIn(content, lastPos);
            if (pos == -1) {
                const auto text = content.mid(lastPos).trimmed();
                if (!text.isEmpty()) {
                    foreach (const int& time, timeLabels)
                        mLines.append(LyricLine(time, text));
                }
                break;
            }

            auto text = content.mid(lastPos, pos - lastPos);
            if (!text.isEmpty()) {
                auto t = text.trimmed();
                if (!t.isEmpty()) {
                    foreach (const int& time, timeLabels)
                        mLines.append(LyricLine(time, t));
                }
                timeLabels.clear();
            }
        } while (true);

        qStableSort(mLines.begin(), mLines.end(), lyricTimeLessThan);
    }

    return !mLines.isEmpty();
}


bool lyricTimeLessThan(const LyricLine& line1, const LyricLine& line2) {
    return line1.time < line2.time;
}
