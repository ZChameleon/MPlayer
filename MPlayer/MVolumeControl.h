#pragma once


#include "MWidget.h"


class MVolumeControl : public MWidget {
Q_OBJECT

public:
    explicit MVolumeControl(QWidget* parent = nullptr);

public:
    void setVolume(int volume) {
        m_volume = volume;
        emit volumeChanged(m_volume);
        update();
    }

signals:
    void volumeChanged(int volume);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

private:
    int m_volume = 0;
    bool m_isPressed = false;
};
