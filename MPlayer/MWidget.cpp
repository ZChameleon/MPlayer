#include <QPainter>
#include <QEvent>
#include "MWidget.h"


MWidget::MWidget(QWidget* parent) : QWidget{parent} {}


void MWidget::paintEvent(QPaintEvent* /*event*/) {

    QPainter painter{this};

    if (m_isAntialiasing) {
        painter.setRenderHint(QPainter::Antialiasing);
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_isHighlighted ? m_highlightedColor : m_backgroundColor);
    painter.drawPath(m_backgroundPath);

    painter.setPen(QPen{m_borderColor, m_borderWidth});
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(m_borderPath);
}


void MWidget::changeEvent(QEvent* event) {

    if (event->type() == QEvent::EnabledChange) {
        m_isHighlighted = false;
        update();
    }
}


void MWidget::enterEvent(QEvent* /*event*/) {

    if (m_isHighLightable) {
        m_isHighlighted = true;
        update();
    }
}


void MWidget::leaveEvent(QEvent* /*event*/) {

    if (m_isHighLightable) {
        m_isHighlighted = false;
        update();
    }
}
