#include "codeScope.hpp"

template <typename LlvmValue>
void CodeScope<LlvmValue>::pushScope()
{
    scope.push_back(std::map<std::string, LlvmValue>());
}


template <typename LlvmValue>
void CodeScope<LlvmValue>::popScope() 
{
    auto delete_scope = scope.back();
    delete_scope.clear();
    scope.pop_back();
}


template <typename LlvmValue>
void CodeScope<LlvmValue>::addObject(std::string name, LlvmValue obj) 
{
    scope.back().insert( std::pair<std::string, LlvmValue>(name, obj) );
}


template <typename LlvmValue>
LlvmValue CodeScope<LlvmValue>::getObject(std::string name)
{
    for (auto it = scope.rend(); it != scope.rbegin(); it++) {
        auto obj_it = it->find(name);
        if (obj_it == it->end()) {
            continue;
        }
        return obj_it->second;
    }
    return NULL;
}

