#ifndef PARSE_ARGUMENTS_H
#define PARSE_ARGUMENTS_H

#include <QCoreApplication>
#include <cli/cliaction.h>

namespace cryptonotescli {
    int parseArguments(
        QCoreApplication& app,
        QStringList& args,
        CLI_ACTION& action,
        int& flags, long& id,
        QString& query
    );
}

#endif
