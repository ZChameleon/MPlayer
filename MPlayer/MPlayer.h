#pragma once


#include <functional>
#include <QSettings>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "MMusicList.h"
#include "MControlBar.h"
#include "MComboBox.h"
#include "MLrcControl.h"


class MPlayer : public MWidget {
Q_OBJECT

public:
    explicit MPlayer(QWidget* parent = nullptr);

signals:
    void positionChanged(qint64 position);

private slots:
    void onAddMusicClicked();
    void onRemoveMusicClicked();

private:
    void settingsListAdd(const QString& group, const QString& text);
    void settingsListRemove(const QString& group, int index);
    void settingsListForEach(const QString& group, const std::function<void(QString)>& f) const;

    QString settingsListAt(const QString& group, int index) const {
        return m_settings.value(group + tr("/") + QString::number(index)).toString();
    }
    void settingsListSetAt(const QString& group, int index, const QString& text) {
        m_settings.setValue(group + tr("/") + QString::number(index), text);
    }

    int settingsListLast(const QString& group) const {
        return m_settings.value(group + tr("/") + tr("Last"), 0).toInt();
    }
    void settingsListSetLast(const QString& group, int index) {
        m_settings.setValue(group + tr("/") + tr("Last"), index);
    }

    int settingsListCount(const QString& group) const {
        return m_settings.value(group + tr("/") + tr("Count"), 0).toInt();
    }
    void settingsListSetCount(const QString& group, int count) {
        m_settings.setValue(group + tr("/") + tr("Count"), count);
    }

private:
    QSettings m_settings{tr("MPlayer.data"), QSettings::IniFormat};

    QMediaPlayer* m_player = nullptr;
    QMediaPlayer::State m_playerLastState = QMediaPlayer::StoppedState;

    QMediaPlaylist* m_playlist = nullptr;

    MMusicList* m_musicList = nullptr;
    MPushButton* m_btnAddMusic = nullptr;
    MPushButton* m_btnRemoveMusic = nullptr;

    MComboBox* m_listComboBox = nullptr;
    MPushButton* m_btnRemoveList = nullptr;

    MPushButton* m_btnSetting = nullptr;

    MControlBar* m_controlBar = nullptr;

    MLrcControl* m_lrcControl = nullptr;

    QRgb m_globalBorderRgba = QColor{0, 120, 215, 128}.rgba();
    QRgb m_globalHighlightRgba = QColor{0, 120, 215, 64}.rgba();

private:
    static constexpr int FIXED_WIDTH = 882;
    static constexpr int FIXED_HEIGHT = 572;
};
