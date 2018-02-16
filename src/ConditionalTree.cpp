#include "ConditionalTree.h"
#include "Error.h"

namespace codegenvar {

namespace internal {

bool ConditionalTree::isFullyEvaluated()const
{
    return branch[true].isFullyEvaluated() && branch[false].isFullyEvaluated();
}

bool ConditionalTree::isEvaluated(const Iterator& address)const
{
    ASSERT(!address.empty());
    const auto& child = branch[address.front().second];
    if (address.size()==1)
        return child.isFullyEvaluated();
    return child.node && child.node->isEvaluated(Iterator(address.begin()+1, address.end()));
}

void ConditionalTree::visit(const Iterator& address)
{
    ASSERT(!address.empty());
    auto& child = branch[address.front().second];
    if (address.size()==1)
    {
        CONDITION(!child.node, "It seems the function tested is not deterministic.");
        child.visited = true;
        return;
    }
    if(!child.node)
        child.node = std::unique_ptr<ConditionalTree>(new ConditionalTree);

    child.node->visit(Iterator(address.begin()+1, address.end()));
}

} // namespace internal

} // namespace codegenvar
