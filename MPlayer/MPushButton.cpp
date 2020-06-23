#include <QPainter>
#include <QResizeEvent>
#include "MPushButton.h"


MPushButton::MPushButton(QString text, QWidget* parent) :
    MWidget{parent}, m_text{std::move(text)} {

    m_isAntialiasing = true;
    m_borderWidth = 2;
}


void MPushButton::paintEvent(QPaintEvent* event) {

    MWidget::paintEvent(event);

    QPainter painter{this};

    painter.setPen(isEnabled() ? Qt::black : Qt::lightGray);
    painter.setBrush(Qt::NoBrush);
    painter.drawText(0, 0, width(), height() - 2, Qt::AlignCenter, m_text);
}


void MPushButton::resizeEvent(QResizeEvent* event) {

    const auto newWidth = event->size().width();
    const auto newHeight = event->size().height();

    QPainterPath borderPath;
    borderPath.addRect(1, 1, newWidth - 2, newHeight - 2);
    m_borderPath = borderPath;

    QPainterPath backgroundPath;
    backgroundPath.addRect(0, 0, newWidth, newHeight);
    m_backgroundPath = backgroundPath;
}


void MPushButton::mousePressEvent(QMouseEvent* event) {

    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}


void MPushButton::changeEvent(QEvent* event) {

    if (event->type() == QEvent::EnabledChange) {
        m_isHighlighted = false;
        update();
    }
}


void MPushButton::enterEvent(QEvent* /*event*/) {

    if (isEnabled()) {
        m_isHighlighted = true;
        update();
    }
}


void MPushButton::leaveEvent(QEvent* /*event*/) {

    if (isEnabled()) {
        m_isHighlighted = false;
        update();
    }
}
