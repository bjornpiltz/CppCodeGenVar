#pragma once
#include <exception>
#include <sstream>

namespace codegenvar {
namespace internal {

class SymbolError : public std::exception
{
public: 
    SymbolError(const std::string& msg)
        : msg(msg)
    {
    }
    virtual const char* what() const noexcept
    {
      return msg.c_str();
    }
    std::string msg;
};


[[noreturn]] inline void MyException(const std::string msg,
                        const char* fileName,
                        const std::size_t lineNumber)
{
   std::ostringstream stream;
   stream << "EXCEPTION: " << msg << ", file " << fileName << " line " << lineNumber;
   throw SymbolError(stream.str());
}

}// namespace internal
}// namespace codegenvar 

#define ERROR(msg) MyException(msg, __FILE__, __LINE__) 
#define CONDITION(cond, msg) if(!(cond))codegenvar::internal::MyException(msg, __FILE__, __LINE__)