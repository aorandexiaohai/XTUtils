#pragma once
#include "xt_quick_cast.h"
namespace XT
{
    class AnyValue{
    private:
        std::string inner_value;
    public:
        AnyValue(const std::string& inner) : inner_value(inner){}
        template<class T>
        operator T()
        {
            return QCast<T>(inner_value);
        }        
    };
    template<class T>
    AnyValue MakeAnyValue(const T& t)
    {
        return QCast<std::string>(t);
    }
}