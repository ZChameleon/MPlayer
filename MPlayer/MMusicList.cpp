#include "MMusicList.h"


MMusicList::MMusicList(QWidget* parent) : QWidget{parent} {

    resize(FIXED_WIDTH, FIXED_HEIGHT);

    m_btnPrevPage = new MPushButton{"<", this};
    m_btnPrevPage->setEnabled(false);
    m_btnPrevPage->setGeometry(515, 20, 30, 30);
    connect(m_btnPrevPage, &MPushButton::clicked, [this] {
        --m_currentPageIndex;
        refreshItems();
    });

    m_btnNextPage = new MPushButton{">", this};
    m_btnNextPage->setEnabled(false);
    m_btnNextPage->setGeometry(550, 20, 30, 30);
    connect(m_btnNextPage, &MPushButton::clicked, [this] {
        ++m_currentPageIndex;
        refreshItems();
    });
}


bool MMusicList::addItem(const QString& filePath, bool swichPage) {

    if (filePath.isEmpty()) {
        return false;
    }

    if (m_items.size() >= 99) {
        return false;
    }

    Music music;
    if (!music.load(filePath)) {
        return false;
    }

    auto item = std::make_shared<MMusicItem>(m_items.size(), music, this);

    connect(item.get(), &MMusicItem::playing, [this](int index, const Music& music) {
        m_currentPlayingIndex = index;
        emit itemplayed(index, music);
    });

    connect(item.get(), &MMusicItem::selected, [this](int index) {
        m_currentSelectionIndex = index;
        emit selectionChanged(index);
    });

    for (const auto& other : m_items) {
        connect(item.get(), &MMusicItem::playing, other.get(), &MMusicItem::stop);
        connect(other.get(), &MMusicItem::playing, item.get(), &MMusicItem::stop);
        connect(item.get(), &MMusicItem::selected, other.get(), &MMusicItem::unselect);
        connect(other.get(), &MMusicItem::selected, item.get(), &MMusicItem::unselect);
    }

    m_items.push_back(item);

    if (m_items.size() > (m_lastPageIndex + 1) * ITEMS_PER_PAGE) {
        ++m_lastPageIndex;
    }

    if (swichPage) {
        m_currentPageIndex = m_lastPageIndex;
    }

    refreshItems();

    return true;
}


void MMusicList::removeItem(int index) {

    if (index < 0 || index >= m_items.size()) {
        return;
    }

    if (index == m_currentPlayingIndex) {
        m_currentPlayingIndex = -1;
    }

    if (index == m_currentSelectionIndex) {
        m_currentSelectionIndex = -1;
        emit selectionChanged(-1);
    }

    m_items.erase(m_items.begin() + index);

    if (m_items.size() <= m_lastPageIndex * ITEMS_PER_PAGE) {
        if (isIndexInPage(index, m_lastPageIndex)) {
            m_currentPageIndex = m_lastPageIndex - 1;
        }
        --m_lastPageIndex;
    }

    refreshItems();
}


void MMusicList::clear() {

    m_currentPlayingIndex = -1;
    m_currentSelectionIndex = -1;
    emit selectionChanged(-1);
    m_items.clear();
    m_currentPageIndex = 0;
    m_lastPageIndex = 0;

    refreshItems();
}


void MMusicList::setCurrentPlaying(int index) {

    m_currentPlayingIndex = index;
    if (m_currentPlayingIndex != -1) {
        m_items[index]->setPlaying();
        emit itemplayed(index, m_items[index]->music());
    } else {
        emit itemStoped();
    }
}


void MMusicList::refreshItems() {

    m_btnPrevPage->setEnabled(m_currentPageIndex > 0);
    m_btnNextPage->setEnabled(m_currentPageIndex < m_lastPageIndex);

    auto index = 0;
    for (const auto& item : m_items) {
        item->setIndex(index);

        item->move(20, 70 + index % ITEMS_PER_PAGE * item->height());
        if (isIndexInPage(item->index(), m_currentPageIndex)) {
            item->show();
        } else {
            item->hide();
        }

        ++index;
    }
}
