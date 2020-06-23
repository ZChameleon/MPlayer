#include <QColorDialog>
#include "ui_SettingDialog.h"
#include "SettingDialog.h"


SettingDialog::SettingDialog(QColor borderColor,
                             QColor highlightColor,
                             QMediaPlaylist::PlaybackMode mode,
                             QWidget* parent) :
    QDialog{parent}, m_ui{new Ui::SettingDialog},
    m_borderColor{std::move(borderColor)},
    m_highlightColor{std::move(highlightColor)} {

    m_ui->setupUi(this);

    m_ui->borderFrame->setPalette(QPalette{m_borderColor});
    m_ui->highlightFrame->setPalette(QPalette{m_highlightColor});

    connect(m_ui->borderButton, &QPushButton::clicked, [this] {
        QColorDialog colorDialog{m_borderColor, this};
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        connect(&colorDialog, &QColorDialog::colorSelected, [this](const QColor& color) {
            m_borderColor = color;
            m_ui->borderFrame->setPalette(QPalette{m_borderColor});
        });
        colorDialog.exec();
    });

    connect(m_ui->highlightButton, &QPushButton::clicked, [this] {
        QColorDialog colorDialog{m_highlightColor, this};
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        connect(&colorDialog, &QColorDialog::colorSelected, [this](const QColor& color) {
            m_highlightColor = color;
            m_ui->highlightFrame->setPalette(QPalette{m_highlightColor});
        });
        colorDialog.exec();
    });

    switch (mode) {

    case QMediaPlaylist::Loop:
        m_ui->sxRadio->setChecked(true);
        break;

    case QMediaPlaylist::Random:
        m_ui->sjRadio->setChecked(true);
        break;

    case QMediaPlaylist::CurrentItemInLoop:
        m_ui->dqRadio->setChecked(true);
        break;

    default:
        m_ui->sxRadio->setChecked(true);
    }

    connect(m_ui->okButton, &QPushButton::clicked, this, &SettingDialog::accept);
}


SettingDialog::~SettingDialog() {

    delete m_ui;
}


void SettingDialog::accept() {

    emit borderColorChanged(m_borderColor);
    emit highlightColorChanged(m_highlightColor);

    if (m_ui->sxRadio->isChecked()) {
        emit playModeChanged(QMediaPlaylist::Loop);
    } else if (m_ui->sjRadio->isChecked()) {
        emit playModeChanged(QMediaPlaylist::Random);
    } else if (m_ui->dqRadio->isChecked()) {
        emit playModeChanged(QMediaPlaylist::CurrentItemInLoop);
    }

    QDialog::accept();
}
