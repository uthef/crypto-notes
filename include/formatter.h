#ifndef FORMATTER_H
#define FORMATTER_H

#include <QString>

namespace cryptonotes {
    class Formatter
    {
    public:
        Formatter();
        static QString timestampToReadableDateTime(long timestamp);
        static QString escapeQuery(QString query, char escapeChar);
    };
}
#endif
