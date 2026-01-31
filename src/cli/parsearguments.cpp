#include <cli/parsearguments.h>
#include <cli/terminal.h>
#include <cli/globals.h>

using namespace cryptonotescli;

namespace cryptonotescli {
    int parseArguments(QCoreApplication& app, QStringList& args, CLI_ACTION& action, int& flags, long& id, QString& query) {
        if (!args.isEmpty()) {
            QString firstArg = args.at(0);

            if (firstArg == "help" || firstArg == "--help" || firstArg == "-help" ) {
                Terminal::printHelpMessage(app);
                return 1;
            }

            if (firstArg == "recent") {
                action = LIST_RECENT_NOTES;
            }
            else if (firstArg == "find" && args.size() > 1) {
                action = FIND_NOTES;
                query = args.at(1);
            }
            else if (firstArg == "details" && args.size() > 1) {
                try {
                    id = std::stol(args.at(1).toStdString());
                    action = SHOW_NOTE_DETAILS;
                }
                catch (const std::exception&) {
                    Terminal::printLine(app.translate(APP_TR_CONTEXT, "Unable to fetch note"));
                    return -1;
                }
            }
            else if (firstArg == "dump" && args.size() > 1) {
                action = DUMP_NOTES;
                query = args.at(1);
            }
            else {
                Terminal::printHelpMessage(app);
                return 1;
            }

            bool filter = false;

            for (auto& arg : args) {
                int flag = 0;

                if (arg == "-id") {
                    flag = Terminal::ID_COL_VISIBLE;
                }
                else if (arg == "-title") {
                    flag = Terminal::TITLE_COL_VISIBLE;
                }
                else if (arg == "-summary") {
                    flag = Terminal::SUMMARY_COL_VISIBLE;
                }
                else if (arg == "-time") {
                    flag = Terminal::LAST_UPDATED_COL_VISIBLE;
                }

                if (!flag) continue;

                if (!filter) {
                    flags = flag;
                    filter = true;
                    continue;
                }

                flags |= flag;
            }

            return 0;
        }

        Terminal::printHelpMessage(app);
        return 1;
    }
}
