#ifndef TABLE_H
#define TABLE_H

#include <cli/tablecell.h>
#include <vector>

namespace cryptonotescli {
    struct TableRow {
        TableRow(std::vector<TableCell> cells);
        std::string toPrintableStr(bool bottomSeparator = false);
    private:
        std::vector<TableCell> cells;
    };
}

#endif
