#include <QFile>
#include <QTextStream>
#include <QPainter>
#include "MLrcControl.h"


MLrcControl::MLrcControl(QWidget* parent) : MWidget{parent} {

    resize(FIXED_WIDTH, FIXED_HEIGHT);

    m_subWidget = new MLrcSubWidget{this};
    m_subWidget->move(0, 0);

    m_timer = new QTimer{this};
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, [this] {
        ++m_timerCount;
        m_subWidget->move(0, m_subWidget->y() - 1);
        if (m_timerCount == PER_LINE_HEIGHT) {
            m_timerCount = 0;
            m_timer->stop();
        }
    });

    m_text = tr("----- 欢迎使用 -----");
}


void MLrcControl::load(Music music, const QString& lrcPath) {

    reset();

    m_music = std::move(music);

    QFile lrcFile{lrcPath};
    if (!lrcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_text = tr("未找到歌词文件");
        return;
    }

    QTextStream inStream{&lrcFile};
    if (!processContent(inStream.readAll(), m_items)) {
        m_text = tr("未找到歌词文件");
        lrcFile.close();
        return;
    }

    lrcFile.close();

    auto count = 0;
    for (const auto& lyricLine : m_items) {
        if (count >= SUB_MAX_LINE) {
            break;
        }
        m_subWidget->addItem(lyricLine.text);
        ++count;
    }

    update();
}


void MLrcControl::refresh(qint64 position, bool timer) {

    auto index = m_items.size() - 1;
    for (auto it = m_items.rbegin(); it < m_items.rend(); ++it) {
        if (position >= it->time) {
            break;
        }
        --index;
    }

    if (index != m_currentIndex) {
        m_subWidget->setCurrentIndex(index);
        m_currentIndex = index;
        if (timer) {
            if (index > MID_INDEX && index < m_items.size() - MID_INDEX) {
                m_timer->start(1);
            }
        } else {
            if (index >= MID_INDEX && index < m_items.size() - MID_INDEX) {
                m_subWidget->move(0, (index - MID_INDEX) * -PER_LINE_HEIGHT);
            }
        }
    }
}

void MLrcControl::reset() {

    m_text = tr("----- 欢迎使用 -----");
    m_timer->stop();
    m_timerCount = 0;
    m_subWidget->clear();
    m_subWidget->move(0, 0);
    m_items.clear();

    update();
}


void MLrcControl::paintEvent(QPaintEvent* /*event*/) {

    QPainter painter{this};

    if (m_items.isEmpty()) {
        painter.drawText(
            QRect{0, PER_LINE_HEIGHT * MID_INDEX, FIXED_WIDTH, PER_LINE_HEIGHT},
            Qt::AlignCenter,
            m_text);
    }
}
