#include "tools.h"

const vector<string> yncapi::slice(const string& s, const char& c) {
    string buff{""};
    vector<string> v;

    for(auto n:s) {
        if(n != c) buff+=n;
        else
            if(n == c && buff != "") {
                v.push_back(buff);
                buff = "";
            }
    }

    if(buff != "") v.push_back(buff);

    return v;
}

void yncapi::replace(string& src, const string& toFind, const string& replaceBy) {
    string tmp;

    if(src.size() && toFind.size() && toFind.size() <= src.size()) {
        size_t pos = src.find(toFind);

        while(pos != string::npos) {
            src.replace(pos, toFind.length(), replaceBy);
            pos = src.find(toFind, pos + 1);
        }
    }
}
