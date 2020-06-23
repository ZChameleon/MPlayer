#pragma once


#include <QDialog>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>


class NewListDialog : public QDialog {
Q_OBJECT

public:
    explicit NewListDialog(QWidget* parent = nullptr);

signals:
    void newListNameGenerated(QString text);

protected:
    void accept() override;

private:
    QHBoxLayout* m_layout = nullptr;
    QLineEdit* m_lineEdit = nullptr;
    QPushButton* m_pushButton = nullptr;
};
