#ifndef NOTELIST_H
#define NOTELIST_H

#include "note.h"
#include <vector>

namespace cryptonotes {
    class NoteList {
    public:
        void push(Note& note);
        size_t size();
        Note at(size_t idx);
        void remove(size_t idx);
        std::vector<Note>::iterator begin();
        std::vector<Note>::iterator end();
    private:
        std::vector<Note> _notes;
    };
}

#endif // NOTELIST_H
