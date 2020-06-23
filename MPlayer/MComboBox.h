#pragma once


#include "MWidget.h"
#include "MComboList.h"


class MComboBox : public MWidget {
Q_OBJECT

public:
    explicit MComboBox(QWidget* parent = nullptr);

public:
    QString text() const { return m_text; }
    void addItem(const QString& text, bool save = true) {
        auto index = m_comboList->addItem(text);
        emit itemAdded(text, save);
    }
    void removeItem(int index) const;
    int currentSeletion() const { return m_currentSelection; }
    void select(int index) const {
        m_comboList->select(index);
    }

signals:
    void selectionChanged(QString text, int index);
    void itemAdded(QString text, bool save);

protected:
    void paintEvent(QPaintEvent* event) override;
    void moveEvent(QMoveEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    MComboList* m_comboList = nullptr;
    QString m_text;
    int m_currentSelection = -1;
    bool m_isListed = false;

private:
    static constexpr int FIXED_HEIGHT = 30;
};
