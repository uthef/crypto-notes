#define WIN _WIN

#include "utils/formatter.h"
#include <QDateTime>

using namespace cryptonotes;

QStringList Formatter::_monthNames = {};

QString Formatter::timestampToReadableDateTime(long timestamp, bool monthNameLast) {
    QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp);
    QDate currentDate = QDateTime::currentDateTime().date();
    QString month;
    QString str;

    QString mName = _monthNames[dt.date().month() - 1];
    int day = dt.date().day();

    if (monthNameLast) {
        month = QString("%1 %2").arg(day).arg(mName);
    }
    else {
        month = QString("%1 %2").arg(mName).arg(day);
    }

    if (dt.date().year() != currentDate.year()) {
        str = QString("%1, %2, %3:%4")
                  .arg(dt.date().year())
                  .arg(month)
                  .arg(dt.time().hour(), 2, 10, QChar('0'))
                  .arg(dt.time().minute(), 2, 10, QChar('0'));

        return str;
    }

    if (dt.date().day() != currentDate.day() || dt.date().month() != currentDate.month()) {
        str = QString("%1, %2:%3")
                  .arg(month)
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
        if (query[i] == '%' || query[i] == '_' || query[i] == escapeChar) {
            query = query.insert(i++, escapeChar);
        }
    }

    return query;
}

QString Formatter::removePathPrefix(QString path) {
    if (path.startsWith("file:///")) {
        path.remove("file:///");
    }

#ifdef UNIX
    if (!path.startsWith("/")) path.prepend("/");
#endif

    return path;
}

void Formatter::setMonthNames(QStringList names) {
    _monthNames = names;
}

QString Formatter::makePrintSafeString(QString str) {
    return str.replace("\n", "\\n").replace("\r", "\\r").replace("\b", "\\b");
}

QString Formatter::identLineBreaks(QString str) {
    return str.replace("\n", "\n\t\t").replace("\r", "\\r").replace("\b", "\\b");
}
