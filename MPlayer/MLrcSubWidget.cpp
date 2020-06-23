#include "QPainter"
#include "MLrcSubWidget.h"


MLrcSubWidget::MLrcSubWidget(QWidget* parent) : MWidget{parent} {

    resize(FIXED_WIDTH, FIXED_HEIGHT);
}


void MLrcSubWidget::paintEvent(QPaintEvent* event) {

    QPainter painter{this};

    auto font = painter.font();
    font.setPointSize(font.pointSize() - 1);
    painter.setFont(font);

    painter.setBrush(Qt::NoBrush);

    auto index = 0;
    for (const auto& text : m_items) {
        painter.setPen(QPen{index == m_currentIndex ? m_borderColor : Qt::black, 1});
        painter.drawText(QRect{0, index * PER_LINE_HEIGHT, FIXED_WIDTH, PER_LINE_HEIGHT},
                         Qt::AlignCenter, text);
        ++index;
    }
}
