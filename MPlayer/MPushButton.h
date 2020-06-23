#pragma once


#include "MWidget.h"


class MPushButton : public MWidget {
Q_OBJECT

public:
    explicit MPushButton(QString text = "", QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void changeEvent(QEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

signals:
    void clicked();

private:
    QString m_text;
};
