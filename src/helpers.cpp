#include "helpers.h"

QString intToQs(int i) {
    QString iString = QString::number(i);

    if (iString.length() == 1) {
        iString.prepend("00");
    } else if (iString.length() == 2) {
        iString.prepend("0");
    }
    iString.insert(iString.length() - 2, ".");

    return iString;
}
