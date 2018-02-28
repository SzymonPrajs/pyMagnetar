#ifndef PYMAGNETAR_FILTERS_H_
#define PYMAGNETAR_FILTERS_H_

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include "utils.h"
#include "vmath/loadtxt.h"
#include "vmath/integrate.h"
#include "vmath/interp.h"
#include "vmath/range.h"
#include "vmath/algebra.h"

using namespace std;
using namespace vmath;


struct FilterData {
    string name_;
    vector<double> inputWavelength_;
    vector<double> inputBandpass_;
    vector<double> restWavelength_;
    vector<double> wavelength_;
    vector<double> bandpass_;
    double area_;
    double zp_;
    double toJy_;
    double min_;
    double max_;
    double centralWavelength_;
};

class Filters {
private:
    // private data members
    string folderPath_;
    vector<string> fileList_;

    // private functions
    void readFolder();
    void loadFilter(int);

public:
    // public data members
    map<string,int> filterID_;
    map<int,string> filterName_;
    vector<FilterData> filters_;

    // constructor
    Filters(string);

    // public functions
    void rescale(double,double,double);   // start, end, step
    void rescale(const vector<double>&);  // wavelength range
    void rescale(double);         // step, start and end set automatically
    double flux(const vector<double>&, const string&);
};

#endif
