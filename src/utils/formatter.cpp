#define WIN _WIN

#include "utils/formatter.h"
#include <QDateTime>

using namespace cryptonotes;

const char* MONTHS[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

QString Formatter::timestampToReadableDateTime(long timestamp) {
    QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp);
    QDate currentDate = QDateTime::currentDateTime().date();
    QString str;

    if (dt.date().year() != currentDate.year()) {
        str = QString("%1, %2 %3, %4:%5")
                  .arg(dt.date().year())
                  .arg(MONTHS[dt.date().month() - 1]).arg(dt.date().day())
                  .arg(dt.time().hour(), 2, 10, QChar('0'))
                  .arg(dt.time().minute(), 2, 10, QChar('0'));

        return str;
    }

    if (dt.date().day() != currentDate.day() || dt.date().month() != currentDate.month()) {
        str = QString("%1 %2, %3:%4")
                  .arg(MONTHS[dt.date().month() - 1]).arg(dt.date().day())
                  .arg(dt.time().hour(), 2, 10, QChar('0'))
                  .arg(dt.time().minute(), 2, 10, QChar('0'));

        return str;
    }

    return QString("%1:%2")
        .arg(dt.time().hour(), 2, 10, QChar('0'))
        .arg(dt.time().minute(), 2, 10, QChar('0'));
}

QString Formatter::escapeQuery(QString query, char escapeChar) {
    for (size_t i = 0; i < query.length(); i++) {
        if (query[i] == '%' || query[i] == '_') {
            query = query.insert(i++, escapeChar);
        }
    }

    return query;
}

QString Formatter::removePathPrefix(QString path) {
    if (path.startsWith("file:///")) {
        path.remove("file:///");
    }

    return path;
}
