#include <QFileDialog>
#include <QStandardPaths>
#include "SettingDialog.h"
#include "MPlayer.h"


MPlayer::MPlayer(QWidget* parent) : MWidget{parent} {

    m_settings.setIniCodec("UTF-8");

    setFixedSize(FIXED_WIDTH, FIXED_HEIGHT);

    auto pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    setPalette(pal);

    m_borderWidth = 2;
    m_borderPath.moveTo(0, 491);
    m_borderPath.lineTo(FIXED_WIDTH, 491);
    m_borderPath.moveTo(601, 0);
    m_borderPath.lineTo(601, FIXED_HEIGHT - 80 - 3);

    setFocusPolicy(Qt::ClickFocus);

    m_playlist = new QMediaPlaylist{this};

    m_player = new QMediaPlayer{this};
    m_player->setNotifyInterval(30);
    m_player->setPlaylist(m_playlist);

    m_musicList = new MMusicList{this};
    m_musicList->move(0, 0);

    m_btnAddMusic = new MPushButton{tr("添加歌曲"), this};
    m_btnAddMusic->setGeometry(20, 20, 80, 30);

    m_btnRemoveMusic = new MPushButton{tr("移除歌曲"), this};
    m_btnRemoveMusic->setGeometry(105, 20, 80, 30);

    m_listComboBox = new MComboBox{this};
    m_listComboBox->setGeometry(200, 20, 130, 30);

    m_btnRemoveList = new MPushButton{tr("删除歌单"), this};
    m_btnRemoveList->setGeometry(335, 20, 80, 30);

    m_btnSetting = new MPushButton{tr("设置"), this};
    m_btnSetting->setGeometry(430, 20, 50, 30);

    m_controlBar = new MControlBar{this};
    m_controlBar->move(0, 491);

    m_lrcControl = new MLrcControl{this};
    m_lrcControl->move(622, 35);


    // 歌曲列表 - 信号槽
    connect(m_musicList, &MMusicList::itemplayed, [this](int index, const Music& music) {
        m_playlist->setCurrentIndex(index);
        m_controlBar->setMusicInfo(music);
        m_controlBar->setPlayStatus(false);
        const auto path = settingsListAt(
            QString::number(m_listComboBox->currentSeletion()), index);
        QFileInfo fileInfo{path};
        m_lrcControl->load(
            music, fileInfo.path() + tr("/") + fileInfo.completeBaseName() + tr(".lrc"));
    });
    connect(m_musicList, &MMusicList::itemStoped, [this] {
        m_controlBar->reset();
        m_lrcControl->reset();
        m_controlBar->setPlayStatus(true);
    });
    connect(m_musicList, &MMusicList::selectionChanged, [this](int index) {
        m_btnRemoveMusic->setEnabled(index != -1);
    });
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged,
            m_musicList, &MMusicList::setCurrentPlaying);


    // 添加删除歌曲 - 信号槽
    connect(m_btnAddMusic, &MPushButton::clicked, this, &MPlayer::onAddMusicClicked);
    connect(m_btnRemoveMusic, &MPushButton::clicked, this, &MPlayer::onRemoveMusicClicked);

    // 歌单列表 - 信号槽
    connect(m_listComboBox, &MComboBox::itemAdded, [this](QString text, bool save) {
        if (save) {
            settingsListAdd(tr("List"), text);
        }
    });
    connect(m_listComboBox, &MComboBox::selectionChanged, [this](QString /*text*/, int index) {
        settingsListSetLast(tr("List"), index);
        m_musicList->clear();
        m_playlist->clear();
        auto count = settingsListCount(QString::number(index));
        for (auto i = 0; i < count;) {
            const auto filePath = settingsListAt(QString::number(index), i);
            if (m_playlist->addMedia(QUrl::fromLocalFile(filePath))) {
                if (m_musicList->addItem(filePath, false)) {
                    ++i;
                } else {
                    m_playlist->removeMedia(i);
                    settingsListRemove(QString::number(index), i);
                    --count;
                }
            } else {
                settingsListRemove(QString::number(index), i);
                --count;
            }
        }
        m_btnRemoveList->setEnabled(index != 0);
        m_controlBar->reset();
        m_lrcControl->reset();
    });
    connect(m_btnRemoveList, &MPushButton::clicked, [this] {
        const auto index = m_listComboBox->currentSeletion();
        settingsListRemove(tr("List"), index);
        m_listComboBox->removeItem(index);
        const auto count = settingsListCount(QString::number(index));
        for (auto i = 0; i < count; ++i) {
            m_settings.remove(QString::number(index) + tr("/") + QString::number(i));
        }
        settingsListSetCount(QString::number(index), 0);
    });

    // 设置 - 信号槽
    connect(m_btnSetting, &MPushButton::clicked, [this] {
        SettingDialog settingDialog{
            QColor::fromRgba(
                m_settings.value(tr("CommonSettings/BorderColor"), m_globalBorderRgba).toUInt()),
            QColor::fromRgba(
                m_settings.value(tr("CommonSettings/HighlightColor"), m_globalHighlightRgba).toUInt()),
            m_playlist->playbackMode(),
            this};
        connect(&settingDialog, &SettingDialog::borderColorChanged, [this](QColor color) {
            setBorderColor(color);
            const auto children = findChildren<MWidget*>();
            for (auto child : children) {
                child->setBorderColor(color);
            }
            m_globalBorderRgba = color.rgba();
            m_settings.setValue(tr("CommonSettings/BorderColor"), m_globalBorderRgba);
        });
        connect(&settingDialog, &SettingDialog::highlightColorChanged, [this](QColor color) {
            setHighlightColor(color);
            const auto children = findChildren<MWidget*>();
            for (auto child : children) {
                child->setHighlightColor(color);
            }
            m_globalHighlightRgba = color.rgba();
            m_settings.setValue(tr("CommonSettings/HighlightColor"), m_globalHighlightRgba);
        });
        connect(&settingDialog, &SettingDialog::playModeChanged, [this](QMediaPlaylist::PlaybackMode mode) {
            m_playlist->setPlaybackMode(mode);
            m_settings.setValue(tr("CommonSettings/PlayMode"), mode);
        });
        settingDialog.exec();
    });

    connect(m_controlBar, &MControlBar::play, m_player, &QMediaPlayer::play);
    connect(m_controlBar, &MControlBar::pause, m_player, &QMediaPlayer::pause);
    connect(m_controlBar, &MControlBar::prev, [this] {
        if (!m_playlist->isEmpty()) {
            m_playlist->previous();
            m_controlBar->setPlayStatus(false);
        }
    });
    connect(m_controlBar, &MControlBar::next, [this] {
        if (!m_playlist->isEmpty()) {
            m_playlist->next();
            m_controlBar->setPlayStatus(false);
        }
    });
    connect(m_player, &QMediaPlayer::durationChanged, m_controlBar, &MControlBar::setDuration);
    connect(m_player, &QMediaPlayer::positionChanged, [this](qint64 position) {
        m_controlBar->setPosition(position);
        m_lrcControl->refresh(position, m_player->state() == QMediaPlayer::PlayingState);
    });
    connect(m_controlBar, &MControlBar::durationChanged, m_player, &QMediaPlayer::setPosition);
    connect(m_controlBar, &MControlBar::durationChangeStart, [this] {
        m_playerLastState = m_player->state();
        if (m_playerLastState == QMediaPlayer::PlayingState) {
            m_player->pause();
        }
    });
    connect(m_controlBar, &MControlBar::durationChangeEnd, [this] {
        if (m_playerLastState == QMediaPlayer::PlayingState) {
            m_player->play();
        }
    });

    connect(m_controlBar, &MControlBar::volumeChanged, [this](int volume) {
        m_player->setVolume(volume);
        m_settings.setValue(tr("CommonSettings/Volume"), volume);
    });


    // 初始化歌单列表
    const auto listCount = settingsListCount(tr("List"));
    if (listCount == 0) {
        m_listComboBox->addItem(tr("默认播放列表"));
        m_listComboBox->select(0);
    } else {
        settingsListForEach(tr("List"), [this](QString text) {
            m_listComboBox->addItem(text, false);
        });
        m_listComboBox->select(settingsListLast(tr("List")));
    }

    // 初始化音量大小
    m_controlBar->setVolume(m_settings.value(tr("CommonSettings/Volume"), 50).toInt());

    // 初始化全局边框配色
    m_globalBorderRgba = m_settings.value(tr("CommonSettings/BorderColor"), 0x800278D7).toUInt();
    setBorderColor(QColor::fromRgba(m_globalBorderRgba));
    auto children = findChildren<MWidget*>();
    for (auto child : children) {
        child->setBorderColor(QColor::fromRgba(m_globalBorderRgba));
    }

    // 初始化全局高亮配色
    m_globalHighlightRgba = m_settings.value(tr("CommonSettings/HighlightColor"), 0x400278D7).toUInt();
    setHighlightColor(QColor::fromRgba(m_globalHighlightRgba));
    children = findChildren<MWidget*>();
    for (auto child : children) {
        child->setHighlightColor(QColor::fromRgba(m_globalHighlightRgba));
    }

    // 初始化播放模式
    m_playlist->setPlaybackMode(
        static_cast<QMediaPlaylist::PlaybackMode>(
            m_settings.value(tr("CommonSettings/PlayMode"), QMediaPlaylist::Loop).toInt()));
}


void MPlayer::onAddMusicClicked() {

    auto filePathList = QFileDialog::getOpenFileNames(
        this, tr("添加歌曲"),
        QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
        tr("歌曲文件 (*.mp3 *.wav *.flac)"));

    for (const auto& filePath : filePathList) {
        if (m_playlist->addMedia(QUrl::fromLocalFile(filePath))) {
            const auto index = m_playlist->mediaCount() - 1;
            if (m_musicList->addItem(filePath)) {
                settingsListAdd(QString::number(m_listComboBox->currentSeletion()), filePath);
            } else {
                m_playlist->removeMedia(index);
            }
        }
    }
}


void MPlayer::onRemoveMusicClicked() {

    const auto index = m_musicList->currentSelectionIndex();
    m_playlist->setCurrentIndex(-1);
    m_playlist->removeMedia(index);
    m_musicList->setCurrentPlaying(-1);
    m_musicList->removeItem(index);
    settingsListRemove(QString::number(m_listComboBox->currentSeletion()), index);
    m_controlBar->reset();
    m_lrcControl->reset();
}


void MPlayer::settingsListAdd(const QString& group, const QString& text) {

    const auto count = settingsListCount(group);
    settingsListSetAt(group, count, text);
    settingsListSetCount(group, count + 1);
}


void MPlayer::settingsListRemove(const QString& group, int index) {

    const auto count = settingsListCount(group);
    settingsListSetCount(group, count - 1);
    for (auto i = index; i < count; ++i) {
        settingsListSetAt(group, i, settingsListAt(group, i + 1));
    }
    m_settings.remove(group + tr("/") + QString::number(count - 1));
}


void MPlayer::settingsListForEach(const QString& group,
                                  const std::function<void(QString)>& f) const {

    const auto count = settingsListCount(group);
    for (auto i = 0; i < count; ++i) {
        f(settingsListAt(group, i));
    }
}
