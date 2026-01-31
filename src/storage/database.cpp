#include "storage/database.h"
#include <cassert>
#include <cstring>
#include "storage/queries.h"

using namespace cryptonotes;

void Database::open(const char* filePath, std::string password) {
    assert(!isOpen());

    sqlite3_open(filePath, &_db);
    sqlite3_key(_db, password.c_str(), password.length());
}

void Database::loadExtension(const char* filePath, char** errMsg) {
    assert(isOpen());

    sqlite3_enable_load_extension(_db, 1);
    sqlite3_load_extension(_db, filePath, 0, errMsg);
    sqlite3_enable_load_extension(_db, 0);
}

void Database::makeSureTableNotesExists(char** errMsg) {
    assert(isOpen());

    sqlite3_exec(_db, CREATE_TABLE_NOTES_Q, 0, 0, errMsg);
}

void Database::rekey(std::string newKey) {
    assert(isOpen());
    sqlite3_rekey(_db, newKey.c_str(), newKey.length());
}

Note Database::constructNote(sqlite3_stmt* stmt, bool includeContent) {
    return Note(
        sqlite3_column_int64(stmt, 0),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
        includeContent ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) : "",
        sqlite3_column_int64(stmt, includeContent ? 4 : 3)
    );
}

int Database::addNote(Note& note) {
    assert(isOpen());

    sqlite3_stmt* stmt;

    sqlite3_prepare(_db, INSERT_NOTE_Q, len(INSERT_NOTE_Q), &stmt, nullptr);

    const auto title = note.title();
    const auto summary = note.summary();
    const auto content = note.content();

    sqlite3_bind_text(stmt, 1, title.c_str(), title.length(), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, summary.c_str(), summary.length(), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, content.c_str(), content.length(), SQLITE_STATIC);

    const int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result;
}

int Database::deleteNote(long id) {
    assert(isOpen());

    sqlite3_stmt* stmt;
    sqlite3_prepare(_db, DELETE_NOTE_Q, len(DELETE_NOTE_Q), &stmt, nullptr);
    sqlite3_bind_int64(stmt, 1, id);

    const int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result;
}

std::optional<Note> Database::getNoteById(long id) {
    assert(isOpen());

    sqlite3_stmt* stmt;

    sqlite3_prepare(_db, SELECT_NOTE_BY_ID_Q, len(SELECT_NOTE_BY_ID_Q), &stmt, nullptr);
    sqlite3_bind_int64(stmt, 1, id);

    const int result = sqlite3_step(stmt);

    if (result != SQLITE_ROW || sqlite3_column_count(stmt) != NOTES_COL_COUNT) {
        sqlite3_finalize(stmt);
        return {};
    }

    Note note = constructNote(stmt, true);

    sqlite3_finalize(stmt);

    return std::make_optional(note);
}

int Database::updateNote(Note& note, int flags, bool updateTimestamp) {
    assert(isOpen());

    std::string sql = "update notes set ";

    size_t columnCount = 0;

    if ((flags & Note::TITLE) == Note::TITLE) {
        columnCount++;
        sql.append("title = ?");
    }

    if ((flags & Note::SUMMARY) == Note::SUMMARY) {
        if (columnCount++ > 0) sql.append(", ");
        sql.append("summary = ? ");
    }

    if ((flags & Note::CONTENT) == Note::CONTENT) {
        if (columnCount++ > 0) sql.append(", ");
        sql.append("content = ?");
    }

    assert(columnCount != 0);

    if (updateTimestamp) {
        sql.append(", timestamp = unixepoch()");
    }

    sql.append(" where id = ?");

    sqlite3_stmt* stmt;
    sqlite3_prepare(_db, sql.c_str(), sql.length(), &stmt, nullptr);

    columnCount = 0;

    const auto title = note.title();
    const auto summary = note.summary();
    const auto content = note.content();

    if ((flags & Note::TITLE) == Note::TITLE) {
        sqlite3_bind_text(stmt, ++columnCount, title.c_str(), title.length(), SQLITE_STATIC);
    }

    if ((flags & Note::SUMMARY) == Note::SUMMARY) {
        sqlite3_bind_text(stmt, ++columnCount, summary.c_str(), summary.length(), SQLITE_STATIC);
    }

    if ((flags & Note::CONTENT) == Note::CONTENT) {
        sqlite3_bind_text(stmt, ++columnCount, content.c_str(), content.length(), SQLITE_STATIC);
    }

    sqlite3_bind_int(stmt, ++columnCount, note.id());

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result;
}

std::unique_ptr<NoteList> Database::getRecentNotes(int& code) {
    assert(isOpen());

    auto listPtr = std::make_unique<NoteList>();
    sqlite3_stmt* stmt;
    sqlite3_prepare(_db, SELECT_RECENT_NOTES_Q, len(SELECT_RECENT_NOTES_Q), &stmt, nullptr);

    selectNotes(stmt, *listPtr, code, false);

    sqlite3_finalize(stmt);

    return listPtr;
}

void Database::forEachNote(const std::function<void(Note*)>& func, int& code) {
    assert(isOpen());

    sqlite3_stmt* stmt;
    sqlite3_prepare(_db, SELECT_ALL_NOTES_Q, len(SELECT_ALL_NOTES_Q), &stmt, nullptr);

    while ((code = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (sqlite3_column_count(stmt) != NOTES_COL_COUNT) break;
        auto note = constructNote(stmt, true);
        func(&note);
    }

    sqlite3_finalize(stmt);
}

std::unique_ptr<NoteList> Database::find(const char* query, int& code) {
    assert(isOpen());

    auto listPtr = std::make_unique<NoteList>();
    sqlite3_stmt* stmt;
    sqlite3_prepare(_db, SEARCH_NOTES_Q, len(SEARCH_NOTES_Q), &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, query, len(query), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, query, len(query), SQLITE_STATIC);

    selectNotes(stmt, *listPtr, code, false);

    sqlite3_finalize(stmt);

    return listPtr;
}

void Database::selectNotes(sqlite3_stmt* stmt, NoteList& list, int& code, bool includeContent) {
    auto colCount = NOTES_COL_COUNT;

    if (!includeContent) colCount--;

    while ((code = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (sqlite3_column_count(stmt) != colCount) break;
        auto note = constructNote(stmt, includeContent);
        list.push(note);
    }
}

void Database::close() {
    assert(isOpen());

    sqlite3_close(_db);
    _db = 0;
}

bool Database::isOpen() const {
    return _db;
}

bool Database::isCodeSuccessful(int code) const {
    return code == SQLITE_OK || code == SQLITE_DONE;
}

size_t Database::countNotes(int& code) const {
    assert(isOpen());

    sqlite3_stmt* stmt = 0;
    sqlite3_prepare(_db, COUNT_NOTES_Q, len(COUNT_NOTES_Q), &stmt, nullptr);
    int result = sqlite3_step(stmt);

    if (result != SQLITE_ROW || sqlite3_column_count(stmt) != 1) {
        code = SQLITE_ERROR;
        return 0;
    }

    size_t count = sqlite3_column_int64(stmt, 0);

    sqlite3_finalize(stmt);

    return count;
}
