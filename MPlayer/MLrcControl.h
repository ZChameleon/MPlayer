#pragma once


#include <QTimer>
#include "MWidget.h"
#include "MLrc.h"
#include "Music.h"
#include "MLrcSubWidget.h"


class MLrcControl : public MWidget {
Q_OBJECT

public:
    explicit MLrcControl(QWidget* parent = nullptr);

public:
    void load(Music music, const QString& lrcPath);

public slots:
    void refresh(qint64 position, bool timer);
    void reset();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QList<LyricLine> m_items;
    Music m_music;

    MLrcSubWidget* m_subWidget = nullptr;
    int m_currentIndex = -1;

    QTimer* m_timer = nullptr;
    int m_timerCount = 0;

    QString m_text;

private:
    static constexpr int FIXED_WIDTH = 240;
    static constexpr int FIXED_HEIGHT = 420;

    static constexpr int PER_LINE_HEIGHT = 28;
    static constexpr int MAX_LINE = 15;
    static constexpr int MID_INDEX = 7;
    static constexpr int SUB_MAX_LINE = 200;
};
