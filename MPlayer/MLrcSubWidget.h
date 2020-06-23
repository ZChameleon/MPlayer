#pragma once


#include <QQueue>
#include "MWidget.h"


class MLrcSubWidget : public MWidget {
Q_OBJECT

public:
    explicit MLrcSubWidget(QWidget* parent = nullptr);

public slots:
    void addItem(const QString& text) {
        m_items.push_back(text);
        update();
    }

    void setCurrentIndex(int index) {
        m_currentIndex = index;
        update();
    }

    void clear() {
        m_items.clear();
        update();
    }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QQueue<QString> m_items;
    int m_currentIndex = -1;

private:
    static constexpr int PER_LINE_HEIGHT = 28;
    static constexpr int MAX_LINE = 200;

    static constexpr int FIXED_WIDTH = 240;
    static constexpr int FIXED_HEIGHT = PER_LINE_HEIGHT * MAX_LINE;
};
