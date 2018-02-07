#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <string>

using namespace std;

namespace yncapi {
    /**
     * @brief 
     * @param s
     * @param c
     */
    const vector<string> slice(const string& s, const char& c);    
    
    /**
     * @brief 
     * @param src
     * @param toFind
     * @param replaceBy
     */
    void replace(string& src, const string& toFind, const string& replaceBy);
}

#endif