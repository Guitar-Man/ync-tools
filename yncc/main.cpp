#include "yncc.h"

string getCmdLine() {
    string cmdLine;

    cout << "> ";
    getline(cin, cmdLine);

    return cmdLine;
}

int main(int argc, char **argv) {
    cout << "YNC Console " << YNCC_MAJOR_VERSION << "."
         << YNCC_MINOR_VERSION << "."
         << YNCC_PATCH_VERSION
         << YNCC_PRE_RELEASE   << " ("

         <<     "API "     << YNCAPI_MAJOR_VERSION << "."
         << YNCAPI_MINOR_VERSION << "."
         << YNCAPI_PATCH_VERSION
         << YNCAPI_PRE_RELEASE   << ")" << endl;

    yncapi::Device::initialize();

    while(yncc::parseCmdLine(getCmdLine()));

    cout << "Exiting..." << endl;

    return 0;
}
