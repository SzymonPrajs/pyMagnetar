#include "Filters.h"

class cMagnetar {
private:
    unique_ptr<Filters> filters;

public:
	cMagnetar();
	~cMagnetar();
    int filterTest();
};
