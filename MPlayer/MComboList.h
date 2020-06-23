#pragma once


#include "MWidget.h"


class MComboList : public MWidget {
Q_OBJECT

public:
    explicit MComboList(int width, QWidget* parent = nullptr);

public:
    int addItem(QString text);
    void removeItem(int index) {
        m_items.erase(m_items.begin() + index);
        resize(width(), 2 + m_items.size() * FIXED_ITEM_HEIGHT);
        update();
    }
    void select(int index) {
        emit itemSelected(m_items[index], index);
    }

signals:
    void itemSelected(QString text, int index);
    void newItemSelected();
    void focusOuted();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QVector<QString> m_items;
    int m_highlightedItem = -1;

private:
    static constexpr int FIXED_ITEM_HEIGHT = 26;
    static constexpr int MAX_ITEMS = 11;
};
