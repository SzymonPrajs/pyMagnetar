#include "Magnetar.h"

cMagnetar::cMagnetar() : filters(new Filters("filters")) {
}

cMagnetar::~cMagnetar() {
}

int cMagnetar::filterTest() {
    return filters->filterID_["SDSS_g"];
}
