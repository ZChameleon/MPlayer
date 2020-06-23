#include <QApplication>
#include "NewListDialog.h"


NewListDialog::NewListDialog(QWidget* parent) : QDialog{parent} {

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("新建歌单"));
    setFixedSize(220, 60);

    m_lineEdit = new QLineEdit{this};
    m_lineEdit->setFixedSize(120, 24);
    m_lineEdit->setMaxLength(6);

    m_pushButton = new QPushButton{tr("确定"), this};
    m_pushButton->setFixedSize(60, 26);
    connect(m_pushButton, &QPushButton::clicked, [this] {
        if (m_lineEdit->text().isEmpty()) {
            QApplication::beep();
            m_lineEdit->setFocus();
        } else {
            accept();
        }
    });

    m_layout = new QHBoxLayout{this};
    m_layout->addWidget(m_lineEdit);
    m_layout->addWidget(m_pushButton);

    setLayout(m_layout);
}


void NewListDialog::accept() {

    emit newListNameGenerated(m_lineEdit->text());

    QDialog::accept();
}
