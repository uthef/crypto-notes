#include "notelist.h"

using namespace cryptonotes;

void NoteList::push(Note& note) {
    _notes.emplace_back(note.id(), note.title(), note.summary(), note.content(), note.timestamp());
}

void NoteList::remove(size_t idx) {
    _notes.erase(_notes.begin() + idx);
}

size_t NoteList::size() {
    return _notes.size();
}

std::vector<Note>::iterator NoteList::begin() {
    return _notes.begin();
}

std::vector<Note>::iterator NoteList::end() {
    return _notes.end();
}

Note NoteList::at(size_t idx) {
    return _notes.at(idx);
}
