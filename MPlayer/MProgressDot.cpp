#include <QPainter>
#include <QMouseEvent>
#include <MProgressDot.h>


MProgressDot::MProgressDot(QWidget* parent) : MWidget{parent} {

    resize(FIXED_DIAMETER, FIXED_DIAMETER);

    m_borderWidth = 2;
    m_borderPath.addEllipse(1, 1, FIXED_DIAMETER - 2, FIXED_DIAMETER - 2);
    m_backgroundPath.addEllipse(0, 0, FIXED_DIAMETER, FIXED_DIAMETER);

    m_isAntialiasing = true;
}


void MProgressDot::paintEvent(QPaintEvent* event) {

    MWidget::paintEvent(event);

    QPainter painter{this};

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_borderColor);
    painter.drawRect(6, 6, 2, 2);
}


void MProgressDot::mousePressEvent(QMouseEvent* event) {

    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        emit mousePressed();
    }
}


void MProgressDot::mouseReleaseEvent(QMouseEvent* event) {

    if (event->button() == Qt::LeftButton) {
        m_isPressed = false;
        emit mouseReleased();
    }
}


void MProgressDot::mouseMoveEvent(QMouseEvent* event) {

    if (m_isPressed) {
        emit moveMe(mapToParent(event->pos()));
    }
}
