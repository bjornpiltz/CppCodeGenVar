#pragma once
#include "AbstractExpression.h"
#include "Number.h"
#include <memory>
#include <set>
#include <vector>

namespace codegenvar{
namespace internal {

class Expression : public AbstractExpression
{
public:
    Expression(Type type);
    Expression(Type type, const std::vector<ConstPtr>& exps);
    
    ConstPtr resolved(const Symbol::Map&)const override;
    
    Type type()const override;
    int childCount()const override;
    ConstPtr child(int num=0)const override;
    const std::vector<ConstPtr>& children()const override;    
    void append(ConstPtr expr)override;
    void prepend(ConstPtr expr)override;
    std::set<std::string> getVariableNames()const override;
    Expression* clone()const override;
    
protected:
    Type _type;
    std::vector<ConstPtr> exps;
};

}// namespace internal
}// namespace codegenvar
