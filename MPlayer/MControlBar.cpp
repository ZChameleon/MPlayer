#include <QTime>
#include <QPainter>
#include "MControlBar.h"


MControlBar::MControlBar(QWidget* parent) : MWidget{parent} {

    resize(FIXED_WIDTH, FIXED_HEIGHT);

    m_albumImage = new QImage;
    m_albumImage->load(":/new/pictures/apic.png");

    m_playButton = new MControlButton{MControlButtonType::play, this};
    m_playButton->move(PLAY_BUTTON_X, 20);
    connect(m_playButton, &MControlButton::play, this, &MControlBar::play);
    connect(m_playButton, &MControlButton::pause, this, &MControlBar::pause);

    m_prevButton = new MControlButton{MControlButtonType::prev, this};
    m_prevButton->move(PREV_BUTTON_X, 22);
    connect(m_prevButton, &MControlButton::prev, this, &MControlBar::prev);

    m_nextButton = new MControlButton{MControlButtonType::next, this};
    m_nextButton->move(NEXT_BUTTON_X, 22);
    connect(m_nextButton, &MControlButton::next, this, &MControlBar::next);

    m_progressBar = new MProgressBar{this};
    m_progressBar->move(PROGRESS_BAR_X, 46);
    connect(m_progressBar, &MProgressBar::positionChanged, [this](int position) {
        emit durationChanged(m_duration / 500 * position);
    });
    connect(m_progressBar, &MProgressBar::positionChangeStart,
            this, &MControlBar::durationChangeStart);
    connect(m_progressBar, &MProgressBar::positionChangeEnd,
            this, &MControlBar::durationChangeEnd);

    m_volumeButton = new MVolumeButton{this};
    m_volumeButton->move(VOLUME_BUTTON_X, 32);

    m_volumeControl = new MVolumeControl{parent};
    m_volumeControl->setGeometry(VOLUME_BUTTON_X, 410, 18, 104);
    m_volumeControl->hide();
    connect(m_volumeButton, &MVolumeButton::lclicked, [this] {
        m_volumeControl->raise();
        m_volumeControl->setFocus();
        m_volumeControl->show();
    });
    connect(m_volumeControl, &MVolumeControl::volumeChanged,
            this, &MControlBar::volumeChanged);
}


void MControlBar::setPlayStatus(bool play) const {

    m_playButton->setButtonType(play
                                    ? MControlButtonType::play
                                    : MControlButtonType::pause);
}


void MControlBar::setMusicInfo(const Music& music) {

    delete m_albumImage;
    m_albumImage = new QImage{music.m_apic};
    m_text = music.m_name + " - " + music.m_artist;

    m_playButton->setPlayable();

    update();
}


void MControlBar::setDuration(qint64 duration) {

    if (duration != 0) {
        m_duration = duration;
        update();
    }
}


void MControlBar::setPosition(qint64 position) {

    if (m_duration != 0) {
        m_position = position;

        const auto progressPos = m_position * 500 / m_duration;
        m_progressBar->setPosition(progressPos >= 498 // fucking workaround.
                                       ? 500 // fucking workaround.
                                       : progressPos);
        update();
    }
}


void MControlBar::reset() {

    delete m_albumImage;
    m_albumImage = new QImage;
    m_albumImage->load(":/new/pictures/apic.png");
    m_text.clear();
    m_duration = 0;
    m_position = 0;
    m_progressBar->setPosition(0);

    m_playButton->setPlayable(false);

    update();
}


void MControlBar::paintEvent(QPaintEvent* /*event*/) {

    QPainter painter{this};

    painter.drawImage(ALBUM_X, 10, *m_albumImage, 0, 0, 60, 60);

    painter.drawText(QRect{PROGRESS_BAR_X + 2, 20, PROGRESS_BAR_WIDTH - 4, 20},
                     Qt::AlignLeft | Qt::AlignTop, m_text);

    auto m = 0;
    auto s = 0;
    const auto foo = [&m, &s](int& t) {
        m = t / 60000;
        s = qRound(t % 60000 / 1000.0);
        if (s == 60) {
            ++m;
            s = 0;
        }
    };

    foo(m_duration);
    QTime duration{0, m, s};
    foo(m_position);
    QTime position{0, m, s};

    auto font = painter.font();
    font.setPointSize(font.pointSize() - 2);
    painter.setFont(font);
    painter.setPen(Qt::darkGray);

    painter.drawText(QRect{PROGRESS_BAR_X + 2, 20, PROGRESS_BAR_WIDTH - 4, 20},
                     Qt::AlignRight | Qt::AlignBottom,
                     position.toString("mm:ss ") + duration.toString("/ mm:ss"));
}
