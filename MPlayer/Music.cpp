#include <QFileInfo>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <attachedpictureframe.h>
#include "Music.h"


Music::Music() {

    m_apic.load(":/new/pictures/apic.png");
}


bool Music::load(const QString& filePath) {

    auto result = false;

    const auto fileExtension = QFileInfo{filePath}.suffix();

    if (fileExtension == "mp3") {
        result = loadAsMp3(filePath);
    }

    return result;
}


bool Music::loadAsMp3(const QString& filePath) {

    TagLib::MPEG::File file{reinterpret_cast<const wchar_t*>(filePath.utf16())};
    if (!file.isOpen()) {
        return false;
    }

    const auto tag = file.ID3v2Tag();
    m_name = tag->title().toCString(true);
    m_artist = tag->artist().toCString(true);
    m_album = tag->album().toCString(true);

    auto frameList = tag->frameListMap()["APIC"];
    if (!frameList.isEmpty()) {
        const auto apic = reinterpret_cast<
            TagLib::ID3v2::AttachedPictureFrame*>(frameList.front());
        if (apic && !apic->picture().isNull()) {
            m_apic.loadFromData(QByteArray::fromRawData(
                apic->picture().data(), apic->picture().size()));
            m_apic = m_apic.scaled(60, 60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
    }

    return true;
}
