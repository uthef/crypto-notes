#ifndef CLI_ACTION_H
#define CLI_ACTION_H

namespace cryptonotescli {
    enum CLI_ACTION {
        NO_ACTION = 0,
        LIST_RECENT_NOTES = 1,
        FIND_NOTES = 2,
        SHOW_NOTE_DETAILS = 3,
        DUMP_NOTES = 4
    };
}

#endif
