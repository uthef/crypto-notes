#ifndef TABLE_CELL_H
#define TABLE_CELL_H

#include <QString>

namespace cryptonotescli {
    struct TableCell {
        TableCell(unsigned int width, QString value);
        std::string toPrintableStr(bool leftSeparator = false);
        TableCell withValue(QString value);
        size_t size(bool leftSeparator = false);
    private:
        unsigned int width = 0;
        QString value;
    };
}

#endif
