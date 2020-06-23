#include <QPainter>
#include "MVolumeControl.h"
#include <QMouseEvent>


MVolumeControl::MVolumeControl(QWidget* parent) : MWidget{parent} {

    m_isAntialiasing = true;

    m_isHighLightable = false;

    m_backgroundColor = Qt::white;

    m_borderWidth = 2;
    m_borderPath.addRect(0, 0, width(), height());
}


void MVolumeControl::paintEvent(QPaintEvent* event) {

    MWidget::paintEvent(event);

    QPainter painter{this};

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_highlightedColor);
    painter.drawRect(2, 100 - m_volume + 2, width() - 4, m_volume);
}


void MVolumeControl::resizeEvent(QResizeEvent* event) {

    QPainterPath backgroundPath;
    backgroundPath.addRect(0, 0, event->size().width(), event->size().height());
    m_backgroundPath = backgroundPath;

    QPainterPath borderPath;
    borderPath.addRect(1, 1, event->size().width() - 2, event->size().height() - 2);
    m_borderPath = borderPath;
}


void MVolumeControl::mousePressEvent(QMouseEvent* event) {

    if (event->button() != Qt::LeftButton) {
        return;
    }

    m_isPressed = true;

    auto newY = event->y();
    if (newY <= 1) {
        newY = 0;
    } else if (newY >= 102) {
        newY = 100;
    }

    m_volume = 100 - newY;

    emit volumeChanged(m_volume);

    update();
}


void MVolumeControl::mouseReleaseEvent(QMouseEvent* event) {

    if (event->button() == Qt::LeftButton) {
        m_isPressed = false;
    }
}


void MVolumeControl::mouseMoveEvent(QMouseEvent* event) {

    if (!m_isPressed) {
        return;
    }

    auto newY = event->y();
    if (newY <= 1) {
        newY = 0;
    } else if (newY >= 102) {
        newY = 100;
    }

    m_volume = 100 - newY;

    emit volumeChanged(m_volume);

    update();
}


void MVolumeControl::focusOutEvent(QFocusEvent* /*event*/) {

    hide();
}
