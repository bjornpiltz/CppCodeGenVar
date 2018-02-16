#pragma once
#include <symengine/logic.h>
#include <memory>

namespace codegenvar {

namespace internal {

/*
 * The class ConditionalTree keeps track of which branches of code have
 * been executed. Take the following simple code example:

   if(x<y)
       return 0.;
   if(x>0)
       return x;
   return -x;
 
 * This code can be modeled as a tree with two nodes and three leaves:
 *
 *   condition       
 *   +--------+  branch[true]
 *   |   x<y  | -------------->  0.0  
 *   +--------+    
 *        | branch[false]
 *        V
 *   condition         
 *   +--------+  branch[true]  
 *   |   x>0  | -------------->  x 
 *   +--------+    
 *        | branch[false]
 *        V
 *       -x   
 */
struct ConditionalTree
{
    typedef std::pair<SymEngine::RCP<const SymEngine::Boolean>, bool> ConditionalBranch;
    typedef std::vector<ConditionalBranch> Iterator;

    struct Child
    {
        std::unique_ptr<ConditionalTree> node;

        bool visited = false;
        bool isNull()const
        {
            return !visited && !node;
        }
        bool isFullyEvaluated()const
        {
            return visited || (node && node->isFullyEvaluated());
        }
    }branch[2];
    
    bool isFullyEvaluated()const;
    bool isEvaluated(const Iterator& address)const;
    void visit(const Iterator& address);
};

} // namespace internal

} // namespace codegenvar
