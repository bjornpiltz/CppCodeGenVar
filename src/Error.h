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
    // Make the source path portable:
    std::string shortFileName(fileName);
    auto lastSlash = shortFileName.find("CppCodeGenVar");
    if (lastSlash != std::string::npos)
        shortFileName = "<src>/" + shortFileName.substr(lastSlash);
    for (auto i = 0; i < shortFileName.size(); ++i)
        if (shortFileName[i] == '\\')
            shortFileName[i] = '/';

    std::ostringstream stream;
    stream << "EXCEPTION: " << msg << "\n" << shortFileName << " line " << lineNumber;
    throw SymbolError(stream.str());
}

}// namespace internal
}// namespace codegenvar 

#define ERROR(msg) codegenvar::internal::MyException(msg, __FILE__, __LINE__) 
#define CONDITION(cond, msg) do{if(!(cond))codegenvar::internal::MyException(msg, __FILE__, __LINE__);}while(false)
#define ASSERT(cond) do{if(!(cond))codegenvar::internal::MyException(#cond, __FILE__, __LINE__);}while(false)