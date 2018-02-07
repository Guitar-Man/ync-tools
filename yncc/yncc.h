#ifndef YNCC_H
#define YNCC_H

#include "../yncapi/yncapi.h"
#include <string>

#define _YNCC_MAJOR_VERSION  0
#define _YNCC_MINOR_VERSION  24
#define _YNCC_PATCH_VERSION  1
#define _YNCC_PRE_RELEASE    ""

static const unsigned int YNCC_MAJOR_VERSION = _YNCC_MAJOR_VERSION;
static const unsigned int YNCC_MINOR_VERSION = _YNCC_MINOR_VERSION;
static const unsigned int YNCC_PATCH_VERSION = _YNCC_PATCH_VERSION;
static const char*        YNCC_PRE_RELEASE   = _YNCC_PRE_RELEASE;

namespace yncc {
    void handle(yncapi::TReturnCode code);
    bool parseCmdLine(const std::string &cmdline);
}

#endif
