#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
        QDEBUG("An incorrect version of BASS was loaded");
        return 0;
    }

    // initialize default recording device
    if (!BASS_RecordInit(-1)) {
        QDEBUG(BASS_ErrorGetCode());
        QDEBUG("Can't initialize recording device");
        return 0;
    }

    w.show();


    return a.exec();
}
