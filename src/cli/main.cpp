#include "storage/appconfig.h"
#include "storage/database.h"
#include "utils/formatter.h"
#include <iostream>
#include "cli/terminal.h"
#include <QCoreApplication>
#include <QTranslator>
#include <cli/tablecell.h>
#include <cli/tablerow.h>
#include <cli/cliaction.h>
#include <cli/parsearguments.h>
#include <cli/globals.h>
#include <stdio.h>

using namespace cryptonotes;
using namespace cryptonotescli;

void setupTranslation(AppConfig& config, QCoreApplication& app, QTranslator& translator);

int main(int argc, char** argv) {
    Terminal::init();

    char* errMsg = 0;
    int sqlCode = 0;

    CLI_ACTION action = NO_ACTION;
    QString query;
    long id = 0;
    int flags = 0;

    QCoreApplication app(argc, argv);
    QStringList args = app.arguments();
    args.pop_front();

    QTranslator translator;

    AppConfig config;
    setupTranslation(config, app, translator);

    int parserCode = parseArguments(app, args, action, flags, id, query);
    if (parserCode == 1) return 0;
    if (parserCode != 0) return parserCode;

    Database database;

    database.open(config.dbPath().toStdString().c_str(),
        Terminal::readLine(app.translate("Auth", "Master password").toStdString(), true)
    );

    Terminal::printLine();

    database.makeSureTableNotesExists(&errMsg);

    if (errMsg) {
        Terminal::printLine(app.translate("Window", "Database connection error."));
        database.close();
        return -1;
    }

    std::unique_ptr<NoteList> notes = nullptr;
    std::optional<Note> note = std::nullopt;

    switch (action) {
        case NO_ACTION:
            return 0;
        case LIST_RECENT_NOTES:
            notes = database.getRecentNotes(sqlCode);
            break;
        case FIND_NOTES:
            notes = database.find(query.toStdString().c_str(), sqlCode);
            break;
        case SHOW_NOTE_DETAILS:
            note = database.getNoteById(id);
            break;
    }

    if (!database.isCodeSuccessful(sqlCode)) {
        Terminal::printLine(app.translate(APP_TR_CONTEXT, "Fetching notes from the database resulted in an error"));
        database.close();
        return -1;
    }

    if (flags == 0) {
        if (action == SHOW_NOTE_DETAILS)
            flags =
                Terminal::ID_COL_VISIBLE | Terminal::TITLE_COL_VISIBLE |
                Terminal::SUMMARY_COL_VISIBLE | Terminal::LAST_UPDATED_COL_VISIBLE;
        else
            flags =
                Terminal::ID_COL_VISIBLE | Terminal::TITLE_COL_VISIBLE |
                Terminal::SUMMARY_COL_VISIBLE;
    }

    switch (action) {
        case DUMP_NOTES:
        {
            FILE* file = fopen(query.toStdString().c_str(), "w");

            if (!file) {
                Terminal::printLine(QString("%1 \"%2\"")
                    .arg(app.translate("CLI", "Cannot open file"))
                    .arg(query)
                );

                return -1;
            }

            database.forEachNote([&app, &file, &flags](Note* note) {
                Terminal::printNote(file, app, *note, flags);
                fprintf(file, "\n");

            }, sqlCode);

            fclose(file);

            if (!database.isCodeSuccessful(sqlCode)) {
                Terminal::printLine(app.translate(APP_TR_CONTEXT, "Fetching notes from the database resulted in an error"));
                database.close();
                return -1;
            }

            break;
        }
        case LIST_RECENT_NOTES:
        case FIND_NOTES:
            Terminal::printNotes(app, notes.get(), flags);
            break;
        case SHOW_NOTE_DETAILS:
            if (flags == 0)

            if (!note.has_value()) {
                Terminal::printLine(app.translate(APP_TR_CONTEXT, "Unable to fetch note"));
                database.close();
                return -1;
            }

            Terminal::printNote(app, note.value(), flags);
            break;
    }


    database.close();

    return 0;
}


void setupTranslation(AppConfig& config, QCoreApplication& app, QTranslator& translator) {
    QString lang = config.language();

    if (lang != "en") {
        if (translator.load(QString(":/translations/cryptonotes_%1.qm").arg(lang)))
            app.installTranslator(&translator);
    }

    QStringList monthNames = {
        app.translate(APP_TR_CONTEXT, "Jan"), app.translate(APP_TR_CONTEXT, "Feb"),
        app.translate(APP_TR_CONTEXT, "Mar"), app.translate(APP_TR_CONTEXT, "Apr"), app.translate(APP_TR_CONTEXT, "May"),
        app.translate(APP_TR_CONTEXT, "Jun"), app.translate(APP_TR_CONTEXT, "Jul"), app.translate(APP_TR_CONTEXT, "Aug"),
        app.translate(APP_TR_CONTEXT, "Sep"), app.translate(APP_TR_CONTEXT, "Oct"), app.translate(APP_TR_CONTEXT, "Nov"),
        app.translate(APP_TR_CONTEXT, "Dec")
    };

    Formatter::setMonthNames(monthNames);
}
