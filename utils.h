#ifndef SLAP_UTILS_UTILS_H_
#define SLAP_UTILS_UTILS_H_

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <dirent.h>
#include <iterator>
#include <algorithm>

using namespace std;


void split(const string&, char, vector<string>&);
vector<string> split(const string&, char);

void dirlist(const string&, vector<string>&);
vector<string> dirlist(const string&);

bool compareStrings(string, string);

void copyFile(string, string);

template<typename T>
string joinStrings(const vector<T> &vec, char delim) {
    ostringstream oss;

    if (vec.size() > 1) {
        copy(vec.begin(), vec.end() - 1, ostream_iterator<T>(oss, &delim));
        oss << vec.back();

    } else {
        oss << vec[0];
    }

    return oss.str();
}

#endif
