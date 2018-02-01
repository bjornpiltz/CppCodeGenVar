#pragma once
#include <codegenvar/Symbol.h>
#include <memory>

namespace codegenvar {
    
struct TBA
{
    void operator=(const Symbol& other);
private:
    std::unique_ptr<int> p;//Todo make non copyable
};

namespace internal {struct BooleanEvaluatorPrivate;}

struct BooleanEvaluator
{
    BooleanEvaluator();
    bool done(std::map<std::string, TBA>&)const;
    
private:
    friend struct internal::BooleanEvaluatorPrivate;
    friend class Symbol;
    static std::shared_ptr<internal::BooleanEvaluatorPrivate> p;
    static std::weak_ptr<internal::BooleanEvaluatorPrivate> get();
};

} // namespace codegenvar
