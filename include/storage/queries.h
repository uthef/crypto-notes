#ifndef QUERIES_H
#define QUERIES_H

#include <cstddef>
#include <string>

namespace cryptonotes {
    const int NOTES_COL_COUNT = 5;

    constexpr const char* CREATE_TABLE_NOTES_Q = R"(
        create table if not exists notes(
            id integer primary key,
            title text not null,
            summary text not null,
            content text not null,
            timestamp integer not null
        );

        create index if not exists title_idx
        on notes(title);
    )";

    constexpr const char* INSERT_NOTE_Q = R"(
        insert into notes (title, summary, content, timestamp)
        values (?, ?, ?, unixepoch())
    )";

    constexpr const char* DELETE_NOTE_Q = R"(
        delete from notes
        where id = ?
    )";

    constexpr const char* SELECT_NOTE_BY_ID_Q = R"(
        select * from notes
        where id = ?
    )";

    constexpr const char* SELECT_RECENT_NOTES_Q = R"(
        select id, title, summary, timestamp from notes
        order by timestamp desc
        limit 200
    )";

    constexpr const char* SEARCH_NOTES_Q = R"(
        select id, title, summary, timestamp from notes
        where title like '%' || ? || '%' escape '!' or summary like '%' || ? || '%' escape '!'
        order by timestamp desc
        limit 200
    )";

    constexpr size_t len(const char* query) {
        return std::string::traits_type::length(query);
    }
}

#endif
