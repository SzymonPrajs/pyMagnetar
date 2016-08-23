#ifndef PYMAGNETAR_MAGNETAR_H_
#define PYMAGNETAR_MAGNETAR_H_

#include "Filters.h"
#include "Cosmology.h"
#include <string>

class cMagnetar {
private:
    unique_ptr<Filters> filters_;
    unique_ptr<Cosmology> cosmology_;

public:
	cMagnetar(string);
	~cMagnetar();
};

#endif
