#define WIN _WIN

#include "formatter.h"
#include <QDateTime>

using namespace cryptonotes;

const char* MONTHS[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

QString Formatter::timestampToReadableDateTime(long timestamp) {
    time_t tt(timestamp);
    tm tm;

#if WIN == 1
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif

    QDate currentDate = QDateTime::currentDateTime().date();
    QString str;

    if (tm.tm_year + 1900 != currentDate.year()) {
        str = QString("%1, %2 %3, %4:%5")
                  .arg(tm.tm_year + 1900)
                  .arg(MONTHS[tm.tm_mon]).arg(tm.tm_mday)
                  .arg(tm.tm_hour, 2, 10, QChar('0'))
                  .arg(tm.tm_min, 2, 10, QChar('0'));

        return str;
    }

    if (tm.tm_mday != currentDate.day() || tm.tm_mon != currentDate.month()) {
        str = QString("%1 %2, %3:%4")
                  .arg(MONTHS[tm.tm_mon]).arg(tm.tm_mday)
                  .arg(tm.tm_hour, 2, 10, QChar('0'))
                  .arg(tm.tm_min, 2, 10, QChar('0'));

        return str;
    }

    return QString("%1:%2")
        .arg(tm.tm_hour, 2, 10, QChar('0'))
        .arg(tm.tm_min, 2, 10, QChar('0'));
}

QString Formatter::escapeQuery(QString query, char escapeChar) {
    for (size_t i = 0; i < query.length(); i++) {
        if (query[i] == '%' || query[i] == '_') {
            query = query.insert(i++, escapeChar);
        }
    }

    return query;
}
