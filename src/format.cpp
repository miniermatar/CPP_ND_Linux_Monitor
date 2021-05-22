#include <string>
#include <chrono>
#include "format.h"
#include <iomanip>

using std::string;

// fromat function for time
string Format::ElapsedTime(long seconds) {
    std::chrono::seconds sec(seconds);
    auto hh=std::chrono::duration_cast<std::chrono::hours>(sec);
    auto mm=std::chrono::duration_cast<std::chrono::minutes>(sec-hh);
    auto ss=sec-hh-mm;
    std::ostringstream timestr;
    timestr << std::setw(2) << std::setfill('0') << hh.count() << ":" << std::setw(2) << std::setfill('0') << mm.count() << ":" << std::setw(2) << std::setfill('0') << ss.count();
    return timestr.str();
}

