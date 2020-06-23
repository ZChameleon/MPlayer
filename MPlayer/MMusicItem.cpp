#include <utility>
#include <QPainter>
#include <QResizeEvent>
#include "MMusicItem.h"


MMusicItem::MMusicItem(int index, Music music, QWidget* parent) :
    MWidget{parent}, m_index{index}, m_music{std::move(music)} {

    resize(FIXED_WIDTH, FIXED_HEIGHT);

    m_borderWidth = 2;
    m_borderPath.moveTo(0, 1);
    m_borderPath.lineTo(FIXED_WIDTH - 1, 1);
    m_backgroundPath.addRect(0, 0, FIXED_WIDTH, FIXED_HEIGHT);
}


void MMusicItem::paintEvent(QPaintEvent* /*event*/) {

    QPainter painter{this};

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_isHighlighted || m_isSelected ? m_highlightedColor : m_backgroundColor);
    painter.drawPath(m_backgroundPath);

    painter.setPen(QPen{m_borderColor, m_borderWidth});
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(m_borderPath);

    painter.setPen(Qt::black);
    painter.drawText(QRect{NAME_X, 0, NAME_WIDTH, FIXED_HEIGHT},
                     Qt::AlignLeft | Qt::AlignVCenter, m_music.m_name);
    painter.drawText(QRect{ARTIST_X, 0, ARTIST_WIDTH, FIXED_HEIGHT},
                     Qt::AlignLeft | Qt::AlignVCenter, m_music.m_artist);
    painter.drawText(QRect{ALBUM_X, 0, ALBUM_WIDTH, FIXED_HEIGHT},
                     Qt::AlignLeft | Qt::AlignVCenter, m_music.m_album);

    auto font = painter.font();
    font.setPointSize(font.pointSize() - 1);
    painter.setFont(font);
    painter.drawText(QRect{INDEX_X, 0, INDEX_WIDTH, FIXED_HEIGHT},
                     Qt::AlignCenter, QString{}.sprintf("%02d", m_index + 1));
}


void MMusicItem::mousePressEvent(QMouseEvent* event) {

    if (event->button() == Qt::LeftButton) {
        emit lclicked();

        if (!m_isSelected) {
            m_isSelected = true;
            emit selected(m_index);
            update();
        }
    }
}


void MMusicItem::mouseDoubleClickEvent(QMouseEvent* event) {

    if (event->button() == Qt::LeftButton) {
        m_isPlaying = true;
        emit playing(m_index, m_music);
        update();
    }
}


void MMusicItem::stop() {

    m_isPlaying = false;
    update();
}


void MMusicItem::unselect() {

    m_isSelected = false;
    update();
}
