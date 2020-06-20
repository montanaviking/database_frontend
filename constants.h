#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "sqlsetup.h"
// Table column labels
enum PaperColumns {
TITLECOL=1,
AUTHCOL=2,
JOURNALCOL=3,
LINKCOL=4,
AREACOL=5,
AREA2COL=6,
KEYWORDSCOL=7
};
enum UndoRedo {
    undo=0,
    redo=1
};

// other constants
const int MAXundoredostacksize=20;

const QString DATAROOTDIRECTORY="/home/viking/library/downloadpaper";

#endif // CONSTANTS_H
