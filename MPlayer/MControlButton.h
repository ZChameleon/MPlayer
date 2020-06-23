#pragma once


#include "MWidget.h"


enum class MControlButtonType {
    play,
    pause,
    prev,
    next
};


class MControlButton : public MWidget {
Q_OBJECT

public:
    explicit MControlButton(MControlButtonType buttonType, QWidget* parent = nullptr);

public:
    void setPlayable(bool playable = true) { m_playable = playable; }

signals:
    void play();
    void pause();
    void prev();
    void next();

public slots:
    void setButtonType(MControlButtonType type);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    MControlButtonType m_buttonType;

    bool m_playable = false;
};
