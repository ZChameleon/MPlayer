#pragma once


#include "MWidget.h"


class MVolumeButton : public MWidget {
Q_OBJECT

public:
    explicit MVolumeButton(QWidget* parent = nullptr);

signals:
    void lclicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    static constexpr int FIXED_WIDTH = 20;
    static constexpr int FIXED_HEIGHT = 20;
};
