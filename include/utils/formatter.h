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
    private:
        static QStringList _monthNames;
    };
}
#endif
