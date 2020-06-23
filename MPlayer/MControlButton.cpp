#include <QPainter>
#include <QMouseEvent>
#include "MControlButton.h"


MControlButton::MControlButton(MControlButtonType type, QWidget* parent)
    : MWidget{parent}, m_buttonType(type) {

    auto diameter = 0;

    if (type == MControlButtonType::play ||
        type == MControlButtonType::pause) {
        diameter = 40;
    } else {
        diameter = 36;
    }

    resize(diameter, diameter);

    m_borderWidth = 2;
    m_borderPath.addEllipse(1, 1, diameter - 2, diameter - 2);
    m_backgroundPath.addEllipse(0, 0, diameter, diameter);

    m_isAntialiasing = true;
}


void MControlButton::setButtonType(MControlButtonType type) {

    m_buttonType = type;

    if (type == MControlButtonType::play) {
        emit pause();
    } else if (type == MControlButtonType::pause) {
        emit play();
    }

    update();
}


void MControlButton::paintEvent(QPaintEvent* event) {

    MWidget::paintEvent(event);

    QPainter painter{this};
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_borderColor);

    switch (m_buttonType) {

    case MControlButtonType::play: {
        QPoint points[] = {{16, 12}, {16, 28}, {28, 20}};
        painter.drawPolygon(points, 3);
        break;
    }

    case MControlButtonType::pause: {
        painter.drawRect(14, 12, 3, 16);
        painter.drawRect(23, 12, 3, 16);
        break;
    }

    case MControlButtonType::prev: {
        QPoint points[] = {{23, 12}, {23, 24}, {13, 18}};
        painter.drawPolygon(points, 3);
        painter.drawRect(11, 13, 2, 10);
        break;
    }

    case MControlButtonType::next: {
        QPoint points[] = {{13, 12}, {13, 24}, {23, 18}};
        painter.drawPolygon(points, 3);
        painter.drawRect(23, 13, 2, 10);
        break;
    }

    default:
        break;
    }
}


void MControlButton::mousePressEvent(QMouseEvent* event) {

    if (event->button() != Qt::LeftButton) {
        return;
    }

    switch (m_buttonType) {

    case MControlButtonType::play: {
        if (m_playable) {
            m_buttonType = MControlButtonType::pause;
            emit play();
            update();
        }
        break;
    }

    case MControlButtonType::pause: {
        m_buttonType = MControlButtonType::play;
        emit pause();
        update();
        break;
    }

    case MControlButtonType::prev: {
        emit prev();
        break;
    }

    case MControlButtonType::next: {
        emit next();
        break;
    }

    default:
        break;
    }
}
