#include "cli/tablerow.h"

#include <sstream>

using namespace cryptonotescli;

TableRow::TableRow(std::vector<TableCell> cells) : cells(cells) {

}

std::string TableRow::toPrintableStr(bool bottomSeparator) {
    std::stringstream ss;
    int width = 0;

    for (int i = 0, j = 0; i < cells.size(); i++) {
        auto& cell = cells.at(i);
        auto str = cell.toPrintableStr(j == 0);
        if (str.empty()) continue;

        width += cell.size(j == 0);
        ss << str;
        j++;
    }

    if (bottomSeparator) {
        ss << "\n";
        ss << QString().fill('-', width - 1).toStdString();
    }

    return ss.str();
}
