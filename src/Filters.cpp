#include "Filters.h"

using namespace std;
using namespace vmath;


Filters::Filters(string path) : folderPath_(path) {
    readFolder();
}


void Filters::readFolder() {
    vector<string> list = loadtxt<string>(folderPath_ + "/list.txt", 1)[0];

    vector<string> temp;
    for (size_t i = 0; i < list.size(); ++i) {
        fileList_.push_back(list[i]);
        split(list[i],'.',temp);

        filterID_[temp[0]] = i;
        filterName_[i] = temp[0];

        loadFilter(i);
    }
}


void Filters::loadFilter(int ID) {
    FilterData filter;

    vector< vector<double> > data;
    string path = folderPath_+"/"+fileList_[ID];
    loadtxt<double>(path,2,data);

    double janskyConst = 3631 * 1e-23 * 299792458 * 1e10;
    vector<double> waveSq = mult<double>(data[0], data[0]);
    vector<double> jansky = div<double>(janskyConst, waveSq);
    jansky = mult<double>(jansky, data[1]);
    double fluxZp = trapz<double>(jansky, data[0][1] - data[0][0]);

    filter.name_ = filterName_[ID];
    filter.inputWavelength_ = data[0];
    filter.inputBandpass_ = data[1];
    filter.wavelength_ = filter.inputWavelength_;
    filter.bandpass_ = filter.inputBandpass_;
    filter.restWavelength_ = filter.inputWavelength_;

    filter.area_ = trapz<double>(data[1], data[0][1] - data[0][0]);
    filter.zp_ = -2.5 * log10(fluxZp / filter.area_);
    filter.toJy_ = pow(10.0, 0.4 * (filter.zp_ + 31.4));
    filter.centralWavelength_ = trapz<double>(mult<double>(data[1], data[0]), data[0][1] - data[0][0]) / filter.area_;
    filter.min_ = -1;
    filter.max_ = -1;
    for (size_t i = 1; i < (filter.bandpass_.size() - 1); i++) {
        if (filter.bandpass_[i] != 0 and filter.min_ == -1) {
            filter.min_ = filter.wavelength_[i-1];
        }
        if (filter.bandpass_[filter.bandpass_.size() - 1 - i] != 0 and filter.max_ == -1) {
            filter.max_ = filter.wavelength_[filter.bandpass_.size() - i];
        }
        if (filter.min_ != -1 && filter.max_ != -1) break;
    }

    filters_.push_back(filter);
}


void Filters::rescale(const vector<double> &wavelength) {
    for (size_t i = 0; i < filters_.size(); ++i) {
        filters_[i].wavelength_ = wavelength;
        filters_[i].bandpass_ = interp<double>(wavelength,filters_[i].inputWavelength_,filters_[i].inputBandpass_);
    }
}


void Filters::rescale(double start, double end, double step) {
    vector<double> wavelength = range<double>(start, end, step);
    for (size_t i = 0; i < filters_.size(); ++i) {
        filters_[i].wavelength_ = wavelength;
        filters_[i].bandpass_ = interp<double>(wavelength,filters_[i].inputWavelength_,filters_[i].inputBandpass_);
    }
}

void Filters::rescale(double step) {
    double start, end;
    for (size_t i = 0; i < filters_.size(); ++i) {
        start = filters_[i].inputWavelength_.front();
        start -= fmod(start, step);
        end = filters_[i].inputWavelength_.back();
        end -= fmod(end, step);
        end += step;

        filters_[i].wavelength_ = range<double>(start, end, step);
        filters_[i].bandpass_ = interp<double>(filters_[i].wavelength_,filters_[i].inputWavelength_,filters_[i].inputBandpass_);
    }
}

double Filters::flux(const vector<double>& SED, const string& filterName) {
    if (filterID_.find(filterName) == filterID_.end()) {
        return 0;

    } else {
        int ID = filterID_.at(filterName);
        vector<double> filteredSED = mult<double>(SED, filters_[ID].bandpass_);
        double integFlux = trapz<double>(filteredSED, filters_[ID].wavelength_[1] - filters_[ID].wavelength_[0]);
        return integFlux / filters_[ID].area_;
    }
}
