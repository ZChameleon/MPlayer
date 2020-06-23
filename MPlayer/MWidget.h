#pragma once


#include <QWidget>
#include <QPainterPath>


class MWidget : public QWidget {
Q_OBJECT

public:
    explicit MWidget(QWidget* parent = nullptr);

public:
    void setBorderColor(const QColor& color) { m_borderColor = color; }
    void setBackgroundColor(const QColor& color) { m_backgroundColor = color; }
    void setHighlightColor(const QColor& color) { m_highlightedColor = color; }

protected:
    void paintEvent(QPaintEvent* event) override;
    void changeEvent(QEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

protected:
    bool m_isAntialiasing = false;

    bool m_isHighLightable = true;
    bool m_isHighlighted = false;

    QPainterPath m_borderPath;
    QColor m_borderColor = QColor{0, 120, 215, 128};
    qreal m_borderWidth = 1;

    QPainterPath m_backgroundPath;
    QColor m_backgroundColor = Qt::white;
    QColor m_highlightedColor = QColor{0, 120, 215, 64};
};
