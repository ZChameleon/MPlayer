#pragma once


#include "MWidget.h"


class MProgressDot : public MWidget {
Q_OBJECT

public:
    explicit MProgressDot(QWidget* parent = nullptr);

signals:
    void moveMe(QPoint pos);
    void mousePressed();
    void mouseReleased();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    int m_dotPos = 0;
    bool m_isPressed = false;
    QPoint m_startPos;

private:
    static constexpr int FIXED_DIAMETER = 14;
};
