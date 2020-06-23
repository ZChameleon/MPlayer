#include <QPainter>
#include <QMouseEvent>
#include "MComboList.h"


MComboList::MComboList(int width, QWidget* parent) : MWidget{parent} {

    setMouseTracking(true);
    resize(width, 2);
    m_backgroundColor = Qt::white;

    m_items.push_back(tr("新建歌单  [+]"));
}


int MComboList::addItem(QString text) {

    if (m_items.size() >= MAX_ITEMS) {
        return -1;
    }

    auto index = 0;
    for (const auto& t : m_items) {
        if (t == text) {
            return index;
        }
        ++index;
    }

    m_items.pop_back();
    m_items.push_back(std::move(text));
    m_items.push_back(tr("新建歌单  [+]"));

    resize(width(), 2 + m_items.size() * FIXED_ITEM_HEIGHT);

    return m_items.size() - 2;
}


void MComboList::paintEvent(QPaintEvent* /*event*/) {

    QPainter painter{this};
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backgroundColor);
    painter.drawRect(rect());

    painter.setPen(QPen{m_borderColor, 2});
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(1, -1, width() - 2, m_items.size() * FIXED_ITEM_HEIGHT + 2);

    auto index = 0;
    for (const auto& text : m_items) {

        if (index == m_highlightedItem) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(m_highlightedColor);
            painter.drawRect(0, index * FIXED_ITEM_HEIGHT, width(), FIXED_ITEM_HEIGHT);
        }

        painter.setPen(Qt::black);
        painter.setBrush(Qt::NoBrush);
        painter.drawText(QRect{14, index * FIXED_ITEM_HEIGHT, width() - 28, FIXED_ITEM_HEIGHT - 2},
                         Qt::AlignLeft | Qt::AlignVCenter, text);

        ++index;
    }
}


void MComboList::mouseMoveEvent(QMouseEvent* event) {

    const auto index = event->y() / FIXED_ITEM_HEIGHT;
    if (index < m_items.size()) {
        m_highlightedItem = index;
        update();
    }
}


void MComboList::leaveEvent(QEvent* event) {

    m_highlightedItem = -1;

    MWidget::leaveEvent(event);
}


void MComboList::focusOutEvent(QFocusEvent* /*event*/) {

    emit focusOuted();
}


void MComboList::mousePressEvent(QMouseEvent* event) {

    if (event->button() != Qt::LeftButton) {
        return;
    }

    const auto index = event->y() / FIXED_ITEM_HEIGHT;
    if (index < m_items.size()) {
        if (index + 1 == m_items.size()) {
            emit newItemSelected();
        } else {
            emit itemSelected(m_items[index], index);
            hide();
        }
    }
}
