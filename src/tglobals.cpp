#include "tglobals.h"
#include <QColor>

Tglobals::Tglobals() {

    version = "0.3 alpha";

//score widget settings
    keySignatureEnabled = true;
    showKeySignName = true;
    nameStyleInKeySign = Tnote::e_deutsch_His;
    majKeyNameSufix = " " ; //like 'major' or 'dur'
    minKeyNameSufix = " " ; //like 'minor' or 'moll'
    pointerColor = -1;

//common for score widget and note name
    doubleAccidentalsEnabled = true;
    showEnharmNotes = true;
    enharmNotesColor = -1;


//note name settings
    NnameStyleInNoteName = Tnote::e_deutsch_His;
    NoctaveInNoteNameFormat = true;
//    NoctaveNameInNoteName = true;

// guitar settings
   GfretsNumber = 19;
   GisRightHanded = true;
   GshowOtherPos = true;
   GfingerColor = -1;
   GselectedColor = -1;
   Gtune = Ttune::stdTune;
   GpreferFlats = false;


}

Tglobals::~Tglobals() {
}
