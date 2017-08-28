#pragma once
#include "xt_std_head.h"

#if _MSC_VER == 1900
template<typename Type>
using PTR = std::shared_ptr<Type>;
template<typename Type>
using WPTR = std::weak_ptr<Type>;

#define PTRMacro(Type) PTR<Type>
#define WPTRMacro(Type) WPTR<Type>

#else if _MSC_VER == 1700
#define PTR(Type) std::shared_ptr<Type>
#define WPTR(Type) std::weak_ptr<Type>
#define PTRMacro(Type)  PTR(Type)
#define WPTRMacro(Type) WPTR(Type)
#endif//_MSC_VER

//as we all know, there is a 'nullptr' in c++11
//but we can not assign it to a weak_ptr
//weak_nullptr is a solution.
//it solved the problem successfully by implicit conversion.
struct weak_nullptr_t {
  template<class T>
  operator WPTRMacro(T) ()
  {
    return WPTRMacro(T) ();
  }
};
static weak_nullptr_t weak_nullptr;


template<class T>
struct IsSharedPtr {
  static const bool value = false;
  typedef void ResultType;
  typedef void FakeCheckType;
};
template<class T>
struct IsSharedPtr<PTRMacro(T)> {
  static const bool value = true;
  typedef T ResultType;
  typedef int FakeCheckType;
};


#define PTR_NEW(type) std::make_shared<type>()

//test case:
//auto x1 = PTR_NEW(int);
//IsSharedPtr<int>::value;
//IsSharedPtr<int>::ResultType;
//IsSharedPtr<int>::FakeCheckType;
//IsSharedPtr<PTRMacro(int)>::value;
//IsSharedPtr<PTRMacro(int)>::ResultType;
//IsSharedPtr<PTRMacro(int)>::FakeCheckType;
//WPTRMacro(int) x2 = weak_nullptr;
