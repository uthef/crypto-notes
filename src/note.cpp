#include "note.h"

using namespace cryptonotes;

Note::Note() {
    _id = 0;
    _timestamp = 0;
    _title = "";
    _summary = "";
    _content = "";
}

Note::Note(long id, std::string title, std::string summary, std::string content, long timestamp) {
    _id = id;
    _title = title;
    _summary = summary;
    _content = content;
    _timestamp = timestamp;
}

long Note::id() const {
    return _id;
}

std::string Note::title() const {
    return _title;
}

std::string Note::summary() const {
    return _summary;
}

std::string Note::content() const {
    return _content;
}

long Note::timestamp() const {
    return _timestamp;
}

// std::string Note::getTime() {
//     time_t time_t(_timestamp);
//     struct tm tm;

//     localtime_r(&time_t, &tm);

//     return QString("%1:%2:%3").arg(tm.tm_hour).arg(tm.tm_min).arg(tm.tm_sec).toStdString();
// }

void Note::setTitle(std::string title) {
    _title = title;
}

void Note::setSummary(std::string summary) {
    _summary = summary;
}

void Note::setContent(std::string content) {
    _content = content;
}
