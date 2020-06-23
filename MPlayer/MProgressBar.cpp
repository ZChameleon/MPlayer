#include <QPainter>
#include "MProgressBar.h"
#include "MControlBar.h"


MProgressBar::MProgressBar(QWidget* parent) : MWidget{parent} {

    resize(FIXED_WIDTH, FIXED_HEIGHT);

    m_borderPath.addRect(FIXED_HEIGHT, 6, FIXED_WIDTH, 1);

    m_dot = new MProgressDot{this};
    m_dot->move(0, 0);
    connect(m_dot, &MProgressDot::moveMe, [this](QPoint position) {
        if (position.x() >= 0 && position.x() <= 500) {
            setPosition(position.x());
            emit positionChanged(position.x());
        }
    });
    connect(m_dot, &MProgressDot::mousePressed, this, &MProgressBar::positionChangeStart);
    connect(m_dot, &MProgressDot::mouseReleased, this, &MProgressBar::positionChangeEnd);
}


void MProgressBar::setPosition(int position) {

    if (position >= 0 && position <= 500) {
        m_dotPosition = position;
    }

    update();
}


void MProgressBar::paintEvent(QPaintEvent* event) {

    QPainterPath borderPath;

    if (m_dotPosition != 0) {
        borderPath.addRect(0, 6, m_dotPosition - 1, 1);
    }

    if (m_dotPosition != 500) {
        borderPath.addRect(m_dotPosition + FIXED_HEIGHT, 6, FIXED_WIDTH, 1);
    }

    m_borderPath = borderPath;

    m_dot->move(m_dotPosition, 0);

    MWidget::paintEvent(event);
}
