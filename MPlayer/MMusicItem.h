#pragma once


#include "MWidget.h"
#include "Music.h"


class MMusicItem : public MWidget {
Q_OBJECT

public:
    explicit MMusicItem(int index, Music music, QWidget* parent = nullptr);

public:
    int index() const { return m_index; }

    void setIndex(int index) {
        m_index = index;
        update();
    }

    void setPlaying() {
        m_isPlaying = true;
        update();
    }

    Music music() const { return m_music; }

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

signals:
    void lclicked();
    void playing(int index, Music music);
    void selected(int index);

public slots:
    void stop();
    void unselect();

private:
    int m_index;
    bool m_isPlaying = false;
    bool m_isSelected = false;

    Music m_music;

private:
    static constexpr int FIXED_WIDTH = 560;
    static constexpr int FIXED_HEIGHT = 40;

    static constexpr int BLANK_WIDTH = 10;

    static constexpr int INDEX_X = BLANK_WIDTH;
    static constexpr int INDEX_WIDTH = 30;
    static constexpr int NAME_X = INDEX_X + INDEX_WIDTH + BLANK_WIDTH;
    static constexpr int NAME_WIDTH = 180;
    static constexpr int ARTIST_X = NAME_X + NAME_WIDTH + BLANK_WIDTH;
    static constexpr int ARTIST_WIDTH = 120;
    static constexpr int ALBUM_X = ARTIST_X + ARTIST_WIDTH + BLANK_WIDTH;
    static constexpr int ALBUM_WIDTH = 180;
};
