#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <string>

using namespace std;

namespace yncapi {
    /**
     * @brief Cut a string into substrings using a separator character
     * @param s Source string
     * @param c Separator
     * @return Vector of substrings
     */
    const vector<string> slice(const string& s, char c);    
    
    /**
     * @brief Replace the "toFind" string in "src" string by the "replaceBy" string each time it's necessary
     * @param src Source string
     * @param toFind String to replace
     * @param replaceBy Replacement string
     */
    void replace(string& src, const string& toFind, const string& replaceBy);
}

#endif