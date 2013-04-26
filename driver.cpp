#include "driver.hpp"

int Driver::parse(FILE * code_file)
{
    yyin = code_file;
    yy_flex_debug = trace_scanning;
    IRBuilder<> builder = IRBuilder<>(getGlobalContext());
    CodeGeneratorBase::setBuilder(&builder);
    yy::Parser parser(*this);
    parser.set_debug_level(trace_parsing);

    return parser.parse();
}


std::string Driver::getAstString() 
{
    std::ostringstream stream;
    stream << "(" << std::endl;

    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        stream << (*it)->toString() << std::endl;
    }

    stream << ")" <<std::endl;
    return stream.str();
}


void Driver::deleteAst() 
{
    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        delete *it;
    }
}

void Driver::error(const std::string& mess)
{
    fprintf(stderr, "%s\n", mess.c_str());
}

