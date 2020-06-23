#pragma once


#include "MWidget.h"
#include "MProgressDot.h"


class MProgressBar : public MWidget {
Q_OBJECT

public:
    explicit MProgressBar(QWidget* parent = nullptr);

public:
    void setPosition(int position);

signals:
    void positionChanged(int position);
    void positionChangeStart();
    void positionChangeEnd();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    MProgressDot* m_dot = nullptr;
    int m_dotPosition = 0;

private:
    static constexpr int FIXED_WIDTH = 514;
    static constexpr int FIXED_HEIGHT = 14;
};
