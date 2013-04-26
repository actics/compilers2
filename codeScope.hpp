#pragma once

#include <string>
#include <vector>
#include <map>

template <typename LlvmValue>
class CodeScope {
private:
    std::vector<std::map<std::string, LlvmValue> > scope;

public:
    virtual ~CodeScope() {}

    void pushScope();
    void popScope();
    LlvmValue getObject(std::string name);
    void addObject(std::string name, LlvmValue object);
};

