#include <cli/tablecell.h>
#include <utils/formatter.h>

using namespace cryptonotescli;
using namespace cryptonotes;

TableCell::TableCell(unsigned int width, QString value) {
    value = Formatter::makePrintSafeString(value);

    int initialSize = value.size();
    this->width = width;
    this->value = value.slice(0, value.size() > width ? width : value.size());

    if (this->value.size() > 3 && initialSize > width) {
        this->value = this->value.slice(0, this->value.size() - 3).append("...");
    }

    this->value.append(QString().fill(' ', width - this->value.size()));
}

std::string TableCell::toPrintableStr(bool leftSeparator) {
    if (value.isEmpty()) return "";
    return (QString(leftSeparator ? "| " : "") + value + QString(" | ")).toStdString();
}

TableCell TableCell::withValue(QString value) {
    return TableCell(width, value);
}

size_t TableCell::size(bool leftSeparator) {
    int printed = !value.isEmpty();
    return leftSeparator ? width + 5 * printed : width + 3 * printed;
}
