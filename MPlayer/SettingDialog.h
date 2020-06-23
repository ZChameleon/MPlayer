#pragma once


#include <QMediaPlaylist>
#include <QDialog>


namespace Ui {
class SettingDialog;
}


class SettingDialog : public QDialog {
Q_OBJECT

public:
    explicit SettingDialog(QColor borderColor,
                           QColor highlightColor,
                           QMediaPlaylist::PlaybackMode mode,
                           QWidget* parent = nullptr);
    ~SettingDialog();

signals:
    void borderColorChanged(QColor color);
    void highlightColorChanged(QColor color);
    void playModeChanged(QMediaPlaylist::PlaybackMode mode);

protected:
    void accept() override;

private:
    Ui::SettingDialog* m_ui;
    QColor m_borderColor;
    QColor m_highlightColor;
};
