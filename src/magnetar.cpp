#include "magnetar.h"
using namespace std;

cMagnetar::cMagnetar(string filterPath) : filters_(new Filters(filterPath)), cosmology_(new Cosmology(0.0)) {
    modelParams_.resize(3);
    SEDParams_.resize(2);
}

cMagnetar::~cMagnetar() {
}

void cMagnetar::_calcDerivedParams() {
    double temp;
    /* Constant quantities */
    opacity_ = 0.1;
    energyRadiation_ = 0;
    alpha_ = 10;

    /* Magnetar spin down timescale */
    tauP_ = 4.7 * pow(modelParams_[1], -2.0) * pow(modelParams_[2], 2.0);

    /* Energy produced by the spin down of a magnetar */
    energyMagnetar_ = 4.9e46 * pow(modelParams_[1], 2.0) * pow(modelParams_[2], -4.0) * tauP_ * 86400;

    /* Kinetic energy of the system */
    energyKinetic_ = 1.0e51 + 0.5 * (energyMagnetar_ - energyRadiation_);

    /* Mass ejected in the explosion */
    // temp = 10 * pow(energyKinetic_ / 1.0e51, -0.25) * pow(opacity_ / 0.1, 0.5);
    // ejectedMass_ = pow(modelParams_[0] / temp, 4.0/3.0);
    ejectedMass_ = pow(modelParams_[0] / 10, 4.0/3.0) * pow(energyKinetic_ / 1.0e51, 1.0 / 3.0) * pow(opacity_ / 0.1, -2.0 / 3.0);

    /* core velocity */
    temp = energyKinetic_ * 10.0 / (3.0 * ejectedMass_ * 2.0e33);
    velocityCore_ = sqrt(temp) * 86400;

    derivedParams_ = {energyMagnetar_, energyKinetic_, ejectedMass_, velocityCore_};

    Wang14_ = 3.0 * opacity_ * ejectedMass_ * 1.989e33 / (4.0 * M_PI * pow(velocityCore_, 2.0));
}


double cMagnetar::_lumMagnetar(double t) {
    return 4.9e46 * pow(modelParams_[1], 2.0) * pow(modelParams_[2], -4.0) / pow(1 + t / tauP_, 2.0);
}


double integralLumSN(double t, void *param) {
    double *par = (double *)param;
    double tauP = par[0];
    double modelParams0 = par[1];
    double modelParams1 = par[2];
    double modelParams2 = par[3];

    double f = 4.9e46 * pow(modelParams1, 2.0) * pow(modelParams2, -4.0) / pow(1 + t / tauP, 2.0);
    double res = 2 * f * t * exp(pow(t / modelParams0, 2)) / pow(modelParams0, 2);

    return res;
}


double cMagnetar::_lumSN(double t) {
    double res = exp(-1 * pow(t / modelParams_[0], 2.0));

    /*Factor based on Wang et. al. (2014) */
    res *= (1 - exp(-Wang14_ * pow(t, -2.0)));

    gsl_integration_workspace *w = gsl_integration_workspace_alloc(10000);
    double integ, error;
    double par[] = {tauP_, modelParams_[0], modelParams_[1], modelParams_[2]};

    gsl_function F;
    F.function = &integralLumSN;
    F.params = &par;

    if (t > 200) {
        return 0;
    }

    gsl_integration_qags(&F, 0, t, 1e39, 1e-7, 10000, w, &integ, &error);
    res *= integ;

    gsl_integration_workspace_free (w);
    return res;
}


double cMagnetar::_radius(double t) {
    double radiusCore = velocityCore_ * (t + modelParams_[3]);
    double rhoCore = 3 * ejectedMass_ * 2e33 / (4 * M_PI * pow(radiusCore, 3));
    double tauCore = opacity_ * rhoCore * radiusCore;

    double rad19 = radiusCore * pow((alpha_ - 1) / tauCore, 1 / (1 - alpha_));
    double rad20 = radiusCore - (1 - tauCore / (alpha_ - 1)) / (opacity_ * rhoCore);

    if (rad19 > radiusCore) {
        return rad19;
    } else if (rad20 > 0) {
        return rad20;
    } else {
        return 0;
    }
}


double cMagnetar::_temperature(double t) {
    return pow(_lumSN(t) / (CGS_SIGMA * 4 * M_PI * pow(_radius(t), 2)), 0.25);
}


void cMagnetar::_calcSEDParams(double t) {
    SEDParams_[0] = _radius(t);
    SEDParams_[1] = _temperature(t);
}


double cMagnetar::_calcSED(double wav) {
    double res;
    res = 2.0 * CGS_H * M_PI * pow(CGS_C,2) / pow(wav * 1e-8, 5);
    res /= exp(CGS_H * CGS_C / (wav * 1e-8 * CGS_K * SEDParams_[1])) - 1.0;
    res *= 4 * M_PI * pow(SEDParams_[0],2);
    res *= 1e-8;

    return res;
}


void cMagnetar::setup(double Tau, double B, double P, double t0, double z) {
    cosmology_->set(z);
    modelParams_ = {Tau, B, P, t0};
    _calcDerivedParams();
}


double cMagnetar::flux(double t, string f) {
    if (t < 0 && t > 200) {
        return 0.0;
    }

    _calcSEDParams(t * cosmology_->a_);
    int ID = filters_->filterID_.at(f);
    vector<double> sed(filters_->filters_[ID].restWavelength_.size(), 0);

    for(size_t i = 0; i < filters_->filters_[ID].restWavelength_.size(); ++i) {
        sed[i] = _calcSED(filters_->filters_[ID].restWavelength_[i]);
        sed[i] *= cosmology_->a_ / (4 * M_PI * pow(cosmology_->lumDisCGS_, 2));
    }

    return filters_->flux(sed, f);
}

vector<double> cMagnetar::flux(double* tArr, size_t size, string f) {
    vector<double> res(size,0);
    for (size_t i = 0; i < size; ++i) {
        res[i] = flux(tArr[i], f);
    }

    return res;
}
