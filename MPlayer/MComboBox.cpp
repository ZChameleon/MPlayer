#include <QPainter>
#include <QMouseEvent>
#include "NewListDialog.h"
#include "MComboBox.h"


MComboBox::MComboBox(QWidget* parent) : MWidget{parent} {

    m_comboList = new MComboList{width(), parent};
    m_comboList->move(pos().x(), pos().y() + FIXED_HEIGHT);
    m_comboList->hide();
    connect(m_comboList, &MComboList::focusOuted, [this] {
        m_isListed = false;
        m_comboList->hide();
        update();
    });
    connect(m_comboList, &MComboList::itemSelected, [this](QString text, int index) {
        m_text = text;
        m_isListed = false;
        if (index != m_currentSelection) {
            m_currentSelection = index;
            emit selectionChanged(text, index);
        }
        update();
    });
    connect(m_comboList, &MComboList::newItemSelected, [this, parent] {
        NewListDialog dialog{parent};
        connect(&dialog, &NewListDialog::newListNameGenerated, [this](QString text) {
            const auto index = m_comboList->addItem(text);
            m_comboList->select(index);
            m_text = text;
            emit itemAdded(text, index);
            update();
        });
        dialog.exec();
    });
}


void MComboBox::removeItem(int index) const {

    if (index >= 1) {
        m_comboList->removeItem(index);
        m_comboList->select(0);
    }
}


void MComboBox::paintEvent(QPaintEvent* /*event*/) {

    QPainter painter{this};
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_isHighlighted || m_isListed
                         ? m_highlightedColor
                         : m_backgroundColor);
    painter.drawRect(rect());

    painter.setPen(QPen{m_borderColor, 2});
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(1, 1, width() - 2, FIXED_HEIGHT - 2);
    painter.drawLine(width() - FIXED_HEIGHT + 4, 13,
                     width() - FIXED_HEIGHT + 9, 18);
    painter.drawLine(width() - FIXED_HEIGHT + 10, 18,
                     width() - FIXED_HEIGHT + 15, 13);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawText(QRect{14, 0, width() - FIXED_HEIGHT + 5, FIXED_HEIGHT - 2},
                     Qt::AlignLeft | Qt::AlignVCenter, m_text);
}

void MComboBox::moveEvent(QMoveEvent* event) {

    m_comboList->move(event->pos().x(), event->pos().y() + FIXED_HEIGHT);
}


void MComboBox::resizeEvent(QResizeEvent* event) {

    m_comboList->resize(event->size().width(), m_comboList->height());
}


void MComboBox::mousePressEvent(QMouseEvent* event) {

    if (event->button() != Qt::LeftButton) {
        return;
    }

    if (!m_isListed) {
        m_isListed = true;
        m_comboList->setFocus();
        m_comboList->show();
    } else {
        m_isListed = false;
    }
}
