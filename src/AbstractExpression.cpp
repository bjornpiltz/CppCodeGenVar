#include "internal/AbstractExpression.h"
#include "internal/Error.h"

namespace codegenvar{
namespace internal{

ConstPtr AbstractExpression::child(int num)const
{
    ERROR("Class has no children.");
}

const std::vector<ConstPtr>& AbstractExpression::children()const
{
    ERROR("Class has no children.");
}

std::set<std::string> AbstractExpression::getVariableNames()const
{
    return {};
}

ConstPtr AbstractExpression::resolved(const Symbol::Map&)const
{
    return ConstPtr{clone()};
}

}// namespace internal
}// namespace codegenvar
