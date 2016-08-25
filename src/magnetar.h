#ifndef PYMAGNETAR_MAGNETAR_H_
#define PYMAGNETAR_MAGNETAR_H_

#include "Filters.h"
#include "Cosmology.h"
#include <string>
#include <gsl/gsl_integration.h>

class cMagnetar {
private:
    unique_ptr<Filters> filters_;
    unique_ptr<Cosmology> cosmology_;
    vector<double> SEDParams_;
    vector<double> modelParams_;
    vector<double> derivedParams_;

    double tauP_;
    double energyMagnetar_;
    double energyKinetic_;
    double energyRadiation_;
    double opacity_;
    double ejectedMass_;
    double velocityCore_;
    double alpha_;
    double Wang14_;

    // Member functions
    void _calcDerivedParams();
    double _calcSED(double);
    void _calcSEDParams(double);

    double _lumMagnetar(double);
    double _lumSN(double);
    double _radius(double);
    double _temperature(double);

public:
	cMagnetar(string);
	~cMagnetar();
    void setup(double,double,double,double);
    double flux(double,string);
    vector<double> flux(double*,size_t,string);
};

#endif
