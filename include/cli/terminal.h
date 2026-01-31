#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <QString>
#include <storage/notelist.h>
#include <QCoreApplication>

using namespace cryptonotes;

namespace cryptonotescli {
    class Terminal {
    public:
        enum ColumnVisibilityFlags {
            ID_COL_VISIBLE = 0b00000001,
            TITLE_COL_VISIBLE = 0b00000010,
            SUMMARY_COL_VISIBLE = 0b00000100,
            LAST_UPDATED_COL_VISIBLE = 0b00001000,
        };

        static void init();
        static std::string readLine(std::string hint, bool redact = false);
        static std::string readLine();
        static void printLine(QString value);
        static void printLine(std::string value);
        static void printLine();
        static void printNotes(QCoreApplication& app, NoteList* list, int flags);
        static void printNote(QCoreApplication& app, Note& note, int flags);
        static void printNote(FILE* stream, QCoreApplication& app, Note& note, int flags);
        static void printHelpMessage(QCoreApplication& app);
    private:
        static int getChar();
    };
};

#endif
