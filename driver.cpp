#include <iostream>
#include "Driver.hpp"

Driver::Driver() :
    trace_scanning(false),
    trace_parsing(false)
{
}

Driver::~Driver()
{
}

int Driver::parse(const std::string& filename_)
{
    filename = filename_;
    scan_begin();
    yy::Parser parser(*this);
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();
    scan_end();
    return res;
}

void Driver::error(const std::string& m)
{
    std::cerr << m << std::endl;
}
