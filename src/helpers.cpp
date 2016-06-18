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

std::string monthFromIndex(int i) {
    std::string selectedMonth;

    switch (i) {
    case -2:
        selectedMonth = "prevTwo";
        break;
    case -1:
        selectedMonth = "prevOne";
        break;
    case 0:
        selectedMonth = "monthOne";
        break;
    case 1:
        selectedMonth = "monthTwo";
        break;
    case 2:
        selectedMonth = "monthThree";
    default:
        break;
    }

    return selectedMonth;
}
