#pragma once


#include "Music.h"
#include "MWidget.h"
#include "MControlButton.h"
#include "MProgressBar.h"
#include "MVolumeButton.h"
#include "MVolumeControl.h"


class MControlBar : public MWidget {
Q_OBJECT

public:
    explicit MControlBar(QWidget* parent = nullptr);

signals:
    void play();
    void pause();
    void prev();
    void next();
    void durationChanged(qint64 duration);
    void durationChangeStart();
    void durationChangeEnd();
    void volumeChanged(int volume);

public slots:
    void setPlayStatus(bool play = true) const;
    void setMusicInfo(const Music& music);
    void setDuration(qint64 duration);
    void setPosition(qint64 position);
    void reset();
    void setVolume(int volume) const { m_volumeControl->setVolume(volume); }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QImage* m_albumImage = nullptr;

    MControlButton* m_playButton = nullptr;
    MControlButton* m_prevButton = nullptr;
    MControlButton* m_nextButton = nullptr;

    QString m_text;
    int m_duration = 0;
    int m_position = 0;
    MProgressBar* m_progressBar = nullptr;

    MVolumeButton* m_volumeButton = nullptr;
    MVolumeControl* m_volumeControl = nullptr;

private:
    static constexpr int FIXED_WIDTH = 850;
    static constexpr int FIXED_HEIGHT = 80;

    static constexpr int BLANK_WIDTH = 25;
    static constexpr int BUTTON_BLANK_WIDTH = 10;

    static constexpr int ALBUM_X = 10;
    static constexpr int ALBUM_WIDTH = 60;

    static constexpr int PREV_BUTTON_X = ALBUM_X + ALBUM_WIDTH + BLANK_WIDTH;
    static constexpr int PREV_WIDTH = 36;
    static constexpr int PLAY_BUTTON_X = PREV_BUTTON_X + PREV_WIDTH + BUTTON_BLANK_WIDTH;
    static constexpr int PLAY_WIDTH = 40;
    static constexpr int NEXT_BUTTON_X = PLAY_BUTTON_X + PLAY_WIDTH + BUTTON_BLANK_WIDTH;
    static constexpr int NEXT_WIDTH = 36;

    static constexpr int PROGRESS_BAR_X = NEXT_BUTTON_X + NEXT_WIDTH + BLANK_WIDTH;
    static constexpr int PROGRESS_BAR_WIDTH = 514;

    static constexpr int VOLUME_BUTTON_X = PROGRESS_BAR_X + PROGRESS_BAR_WIDTH + BLANK_WIDTH + 10;
    static constexpr int VOLUME_BUTTON_WIDTH = 20;
};
