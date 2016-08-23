#include "Magnetar.h"
using namespace std;

cMagnetar::cMagnetar(string filterPath) : filters_(new Filters(filterPath)), cosmology_(new Cosmology(0.0)) {

}

cMagnetar::~cMagnetar() {
}
