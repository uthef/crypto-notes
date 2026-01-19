#ifndef DATABASE_H
#define DATABASE_H

#define SQLITE_HAS_CODEC 1

#include <string>
#include <memory>
#include "note.h"
#include <sqlcipher/sqlite3.h>
#include <optional>
#include <notelist.h>

namespace cryptonotes {
    class Database final {
    public:
        void open(const char* filePath, std::string password);
        void makeSureTableNotesExists(char** errMsg);
        int addNote(Note& note);
        void loadExtension(const char* filePath, char** errMsg);
        std::optional<Note> getNoteById(long id);
        int updateNote(Note& note, int flags, bool updateTimestamp);
        int deleteNote(long id);
        std::unique_ptr<NoteList> getRecentNotes(int& code);
        std::unique_ptr<NoteList> find(const char* query, int& code);
        void rekey(std::string newKey);
        void close();
        bool isOpen() const;
        bool isCodeSuccessful(int code) const;
    private:
        Note constructNote(sqlite3_stmt* stmt, bool includeContent);
        void selectNotes(sqlite3_stmt* stmt, NoteList& list, int& code, bool includeContent);
        sqlite3* _db = 0;
    };
}
#endif
