#ifndef FORMATTER_H
#define FORMATTER_H

#include <QString>

namespace cryptonotes {
    class Formatter
    {
    public:
        Formatter() = delete;

        static QString timestampToReadableDateTime(long timestamp, bool monthNameLast = false);
        static QString escapeQuery(QString query, char escapeChar);
        static QString removePathPrefix(QString path);
        static void setMonthNames(QStringList names);
        static QString makePrintSafeString(QString str);
        static QString identLineBreaks(QString str);
    private:
        static QStringList _monthNames;
    };
}
#endif
