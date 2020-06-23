#include <QPainter>
#include <QMouseEvent>
#include "MVolumeButton.h"


MVolumeButton::MVolumeButton(QWidget* parent) : MWidget{parent} {

    resize(FIXED_WIDTH, FIXED_HEIGHT);

    const QVector<QPoint> points = {
        {2, 6}, {6, 6}, {9, 2},
        {9, 16}, {6, 12}, {2, 12}
    };
    m_backgroundPath.addPolygon(QPolygon{points});
}


void MVolumeButton::paintEvent(QPaintEvent* event) {

    MWidget::paintEvent(event);

    QPainter painter{this};

    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen{m_borderColor, 2});
    painter.setBrush(Qt::NoBrush);

    const QVector<QPoint> points = {
        {1, 5}, {5, 5}, {10, 1},
        {10, 17}, {5, 13}, {1, 13}
    };

    painter.drawPolygon(QPolygon{points});
    painter.drawLine(14, 6, 14, 12);
    painter.drawLine(18, 3, 18, 15);
}


void MVolumeButton::mousePressEvent(QMouseEvent* event) {

    if (event->button() == Qt::LeftButton) {
        emit lclicked();
    }
}
