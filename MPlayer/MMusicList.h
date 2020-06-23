#pragma once


#include <memory>
#include "MPushButton.h"
#include "MMusicItem.h"


class MMusicList : public QWidget {
Q_OBJECT

public:
    explicit MMusicList(QWidget* parent = nullptr);

public:
    int currentPlayingIndex() const { return m_currentPlayingIndex; }
    int currentSelectionIndex() const { return m_currentSelectionIndex; }

signals:
    void itemplayed(int index, Music music);
    void itemStoped();
    void selectionChanged(int index);

public slots:
    bool addItem(const QString& filePath, bool swichPage = true);
    void removeItem(int index);
    void clear();
    void setCurrentPlaying(int index);

private:
    void refreshItems();

    static bool isIndexInPage(int index, int pageIndex) {
        return index >= pageIndex * ITEMS_PER_PAGE &&
            index < (pageIndex + 1) * ITEMS_PER_PAGE;
    }

private:
    QVector<std::shared_ptr<MMusicItem>> m_items;

    MPushButton* m_btnPrevPage = nullptr;
    MPushButton* m_btnNextPage = nullptr;

    int m_currentPlayingIndex = -1;
    int m_currentSelectionIndex = -1;

    int m_currentPageIndex = 0;
    int m_lastPageIndex = 0;

private:
    static constexpr int FIXED_WIDTH = 600;
    static constexpr int FIXED_HEIGHT = 470;
    static constexpr int ITEMS_PER_PAGE = 10;
};
