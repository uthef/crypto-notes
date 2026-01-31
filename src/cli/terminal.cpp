#include <cli/terminal.h>

#include <sstream>
#include <cli/tablecell.h>
#include <cli/tablerow.h>
#include <utils/formatter.h>

#ifdef UNIX
#include <termios.h>
#include <unistd.h>
#endif

using namespace cryptonotescli;
using namespace cryptonotes;

std::string Terminal::readLine(std::string hint, bool redact) {
#ifdef UNIX
    struct termios terminal;

    if (redact) {
        auto attr = tcgetattr(STDIN_FILENO, &terminal);
        terminal.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
    }
#endif

    if (!hint.empty()) printf("%s: ", hint.c_str());

    std::stringstream ss;
    int c = getc(stdin);
    int charCount = 0;

    while (c && c != '\n') {
        if ((c == 127 || c == 8)) {
            if (charCount > 0) {
                printf("\b \b");
                charCount--;
                ss << "\b \b";
            }
        }
        else {
            if (redact) putchar('*');
            charCount++;
            ss << (char)c;
        }

        c = getc(stdin);
    }

    if (redact) printf("\n");

#ifdef UNIX
    if (redact) {
        terminal.c_lflag |= (ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
    }
#endif

    return ss.str();
}

std::string Terminal::readLine() {
    return readLine("");
}

void Terminal::printLine(QString value) {
    printf("%s\n", value.toStdString().c_str());
}

void Terminal::printLine(std::string value) {
    printf("%s\n", value.c_str());
}

void Terminal::printLine() {
    printf("\n");
}

void Terminal::printNotes(QCoreApplication& app, NoteList* list, int flags) {
    TableCell
        idCol(6 * ((flags & ID_COL_VISIBLE) == ID_COL_VISIBLE), "ID"),
        titleCol(20 * ((flags & TITLE_COL_VISIBLE) == TITLE_COL_VISIBLE), app.translate("List", "Title")),
        summaryCol(48 * ((flags & SUMMARY_COL_VISIBLE) == SUMMARY_COL_VISIBLE), app.translate("List", "Summary")),
        lastUpdatedCol(
            24 * ((flags & LAST_UPDATED_COL_VISIBLE) == LAST_UPDATED_COL_VISIBLE),
            app.translate("List", "Last updated")
        );

    TableRow header({idCol, titleCol, summaryCol, lastUpdatedCol});

    Terminal::printLine(header.toPrintableStr(true));

    for (int i = 0; i < list->size(); i++) {
        auto note = list->at(i);

        TableRow row({
            idCol.withValue(QString::number(note.id())),
            titleCol.withValue(QString::fromStdString(note.title())),
            summaryCol.withValue(QString::fromStdString(note.summary())),
            lastUpdatedCol.withValue(Formatter::timestampToReadableDateTime(
                note.timestamp(),
                app.translate("cryptonotes::NoteListModel", "en") == "ru")
            )
        });

        Terminal::printLine(row.toPrintableStr());
    }
}

void Terminal::printNote(QCoreApplication& app, Note& note, int flags) {
    Terminal::printNote(stdout, app, note, flags);
}

void Terminal::printNote(FILE* stream, QCoreApplication& app, Note& note, int flags) {
    if ((flags & ID_COL_VISIBLE) == ID_COL_VISIBLE) {
        fprintf(stream, "ID: %ld\n", note.id());
    }

    if ((flags & LAST_UPDATED_COL_VISIBLE) == LAST_UPDATED_COL_VISIBLE) {
        fprintf(stream, "\t%s: %s\n",
            app.translate("List", "Last updated").toStdString().c_str(),
            Formatter::timestampToReadableDateTime(
                note.timestamp(),
                app.translate("cryptonotes::NoteListModel", "en") == "ru"
            ).toStdString().c_str()
        );
    }

    if ((flags & TITLE_COL_VISIBLE) == TITLE_COL_VISIBLE) {
        fprintf(stream, "\t%s:\n\t\t%s\n",
            app.translate("List", "Title").toStdString().c_str(),
            Formatter::identLineBreaks(QString::fromStdString(note.title())).toStdString().c_str()
        );
    }

    if ((flags & SUMMARY_COL_VISIBLE) == SUMMARY_COL_VISIBLE) {
        fprintf(stream, "\t%s:\n\t\t%s\n",
            app.translate("List", "Summary").toStdString().c_str(),
            Formatter::identLineBreaks(QString::fromStdString(note.summary())).toStdString().c_str()
        );
    }

    fprintf(stream, "\t%s:\n\t\t%s\n",
        app.translate("Editor", "Note").toStdString().c_str(),
        Formatter::identLineBreaks(QString::fromStdString(note.content())).toStdString().c_str()
    );
}

void Terminal::printHelpMessage(QCoreApplication& app) {
    printf("Crypto Notes v%s\n", APP_VERSION);

    printf(
        "%s:\n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n\n",
        app.translate("CLI", "Commands").toStdString().c_str(),
        "help",
        "dump <file>"
        "recent",
        "find <query>",
        "details <id>"
    );

    printf("%s:\n\t%s\n\t%s\n\t%s\n\t%s\n",
        app.translate("CLI",  "Column visibility flags").toStdString().c_str(),
        "-id",
        "-title",
        "-summary",
        "-time"
    );
}
