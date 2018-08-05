#pragma once
#include "xt_pointer.h"


template<class Type, bool>
struct MakeDefaultObjInner;
template<class Type>
struct MakeDefaultObjInner<Type, true>
{
  operator Type() {
    return Type{};
  }
};
//help set the class member to default value in 'VarMacro'
template<class Type>
Type MakeDefaultObj()
{
  return MakeDefaultObjInner<Type,
    std::is_default_constructible<Type>::value &&
    (
      std::is_move_constructible<Type>::value
      ||
      std::is_copy_constructible<Type>::value
      )>{};
}

template<class T, unsigned int size>
std::array<T, size> MakeDefaultObjArr()
{
  std::array<T, size> result;
  for (int index = 0; index < size; index++)
  {
    result[index] = MakeDefaultObj<T>();
  }
  return result;
}

#define VarMacro(type, var_name)\
protected:\
type var_name##_ = MakeDefaultObj<type>();

#define VarMacroPure(type, var_name)\
protected:\
type var_name##_;



//ensure zero initialize manually.
#define VarMacroArr(type, var_name, size)\
protected:\
type var_name##_[size];\

#define SetInnerMacro(type, var_name)\
public:\
void set_##var_name(const type& var_name){var_name##_ = var_name;}

#define SetInnerMacroArr(type, var_name)\
public:\
void set_##var_name(const type& var_name, int index){var_name##_[index] = var_name;}

#define GetInnerCopyMacro(type, var_name)\
public:\
type get_##var_name()const{return var_name##_;}\

#define GetInnerRefMacro(type, var_name)\
public:\
type&  get_##var_name(){return var_name##_;}\

#define GetInnerRefMacroArr(type, var_name)\
public:\
type&  get_##var_name(int index){return var_name##_[index];}

#define GetInnerConstRefMacroArr(type, var_name)\
public:\
const type&  get_##var_name(int index) const {return var_name##_[index];}

#define GetInnerConstRefMacro(type, var_name)\
public:\
const type&  get_##var_name()const{return var_name##_;}\

//if it is not a fixed array, give the compiler error.
#define GetInnerSizeMacro(type, var_name)\
public:\
inline size_t get_##var_name##_size(){\
  (void)(sizeof(IsArray<decltype(var_name##_)>::FakeCheckType));\
  return sizeof(var_name##_)/sizeof(type);\
}


//if type is not a shared ptr, give a compile error.
#define PtrGetInnerMacro(type, var_name)\
public:\
WPTRMacro(IsSharedPtr<type>::ResultType)  get_##var_name() const{\
  (void)sizeof(IsSharedPtr<type>::FakeCheckType);\
  return var_name##_;\
} \
PTRMacro(IsSharedPtr<type>::ResultType)  get_##var_name##_lock() const{\
  return var_name##_;\
} 

#define PtrGetInnerMacroArr(type, var_name)\
public:\
WPTRMacro(IsSharedPtr<type>::ResultType)  get_##var_name(int index) const{\
  (void)sizeof(IsSharedPtr<type>::FakeCheckType);\
  return var_name##_[index];\
}

#define GetInnerMacroArr(type, var_name)\
public:\
GetInnerRefMacroArr(type, var_name)\
GetInnerConstRefMacroArr(type, var_name)

#define GetInnerMacro(type, var_name)\
GetInnerRefMacro(type, var_name)\
GetInnerConstRefMacro(type, var_name)

//define a protected variable and get/set fucntion
#define SetMacro(type, var_name)\
VarMacro(type, var_name)\
SetInnerMacro(type, var_name)

//define a protected variable and get/set fucntion
#define SetGetMacro(type, var_name)\
VarMacro(type, var_name)\
GetInnerMacro(type, var_name)\
SetInnerMacro(type, var_name)



#define PtrSetGetMacro(type, var_name)\
VarMacro(type, var_name)\
PtrGetInnerMacro(type, var_name)\
SetInnerMacro(type, var_name)

//define a protected variable and get ref function
#define GetRefMacro(type, var_name)\
VarMacro(type, var_name)\
GetInnerMacro(type, var_name)

//define a protected variable and get function
#define GetMacro(type, var_name)\
VarMacro(type, var_name)\
GetInnerCopyMacro(type, var_name)


#define PtrGetMacro(type, var_name)\
VarMacro(type, var_name) \
PtrGetInnerMacro(type, var_name)

#define PtrGetMacroArr(type, var_name, size)\
VarMacroArr(type, var_name,size)\
GetInnerSizeMacro(type, var_name)\
PtrGetInnerMacroArr(type, var_name)


#define SetGetMacroArr(type, var_name, size)\
VarMacroArr(type, var_name, size)\
GetInnerSizeMacro(type, var_name)\
SetInnerMacroArr(type, var_name)\
GetInnerMacroArr(type, var_name)\

//define a protected variable and get ref function
#define GetConstRefMacro(type, var_name)\
VarMacro(type, var_name)\
GetInnerConstRefMacro(type, var_name)

#if _MSC_VER == 1900
#define DEFAULT_CONSTRUCTOR(Type)\
public:\
Type() = default;

#define DEFAULT_COPY_CONSTRUCTOR(Type)\
public:\
Type(const Type&) = default;

#define DEFAULT_COPY_ASSIGNMENT(Type)\
public:\
Type& operator=(const Type&) = default;

#define DEFAULT_MOVE_CONSTRUCTOR(Type)\
public:\
Type(Type&&) = default;

#define DEFAULT_MOVE_ASSIGNMENT(Type)\
public:\
Type& operator=(Type&&) = default;

#define DEFAULT_OPERATION(Type)\
DEFAULT_COPY_CONSTRUCTOR(Type)\
DEFAULT_COPY_ASSIGNMENT(Type)\
DEFAULT_MOVE_CONSTRUCTOR(Type)\
DEFAULT_MOVE_ASSIGNMENT(Type)

#define DEFAULT_ALL(Type)\
DEFAULT_CONSTRUCTOR(Type)\
DEFAULT_OPERATION(Type)

#define DELETE_COPY_CONSTRUCTOR(Type)\
public:\
Type(const Type&) = delete;

#define DELETE_COPY_ASSIGNMENT(Type)\
public:\
Type& operator=(const Type&) = delete;

#define DELETE_MOVE_CONSTRUCTOR(Type)\
public:\
Type(Type&&) = delete;

#define DELETE_MOVE_ASSIGNMENT(Type)\
public:\
Type& operator=(Type&&) = delete;

#define DELETE_OPERATION(Type)\
DELETE_COPY_CONSTRUCTOR(Type)\
DELETE_COPY_ASSIGNMENT(Type)\
DELETE_MOVE_CONSTRUCTOR(Type)\
DELETE_MOVE_ASSIGNMENT(Type)
#endif

template<typename Function> struct xt_function_traits_helper;
template<typename R, typename T1>
struct xt_function_traits_helper<R(T1)>
{
  typedef R result_type;
  typedef T1 arg1_type;
};
#define IdentityType(type) xt_function_traits_helper<void type>::arg1_type



template<class T>
using remove_qualifers_t = std::remove_const_t<std::remove_reference_t<T>>;
#define REMOVE_QUALIFERS_T(Type) remove_qualifers_t<Type>

#define REMOVE_QUALIFIERS(exp) REMOVE_QUALIFERS_T(decltype(exp))


#define _CLASS_BEGIN {
#define _CLASS_END };
#if _MSC_VER == 1900
//can be converted to bool
template<class T>
bool CheckValidateInner(T&& t, std::true_type)
{
  return static_cast<bool>(t);
}

//it should not happen.
template<class T>
bool CheckValidateInner(T&& t, std::false_type)
{
#if defined(DEBUG) || defined(_DEBUG)
  std::abort();
#endif
  return false;
}

template<typename T, typename... Args>
bool CheckValidate(T&& t, Args&&... args)
{
  return CheckValidateInner(std::forward<T>(t), std::is_constructible<bool, T&&>::type()) && CheckValidate(std::forward<Args>(args)...);
}
static bool CheckValidate()
{
  return true;
}


//debug test.
#if defined(DEBUG) || defined(_DEBUG)
  #define CheckValidateSerious(...)\
  {\
    if(!CheckValidate(__VA_ARGS__))\
    {\
      std::abort();\
    }\
  }
#else
  #define CheckValidateSerious(...)
#endif

#endif






template< class To, class From>
To CheckCast(From* from)
{
  assert(dynamic_cast<To>(from));
  return static_cast<To>(from);
}


#if _MSC_VER == 1900
template<class Base, class Derived>
using is_base_of_t = typename std::is_base_of<Base, Derived>::type;
#define IS_BASE_OF_T(Base, Derived) is_base_of_t<Base, Derived>
#else if _MSC_VER == 1700
#define IS_BASE_OF_T(Base, Derived) std::is_base_of<Base, Derived>::type
#endif


template<class Self>
struct MemberFunctionParse;

template<class Result, class Self>
struct MemberFunctionParse< Result(Self::*)()>
{
  typedef Result ResultType;
};

#if _MSC_VER == 1900
#define GenerateSelfType()\
private:\
auto SelfTypeNeverUseManually()->REMOVE_QUALIFERS_T(decltype(*this));\
using SelfTypeNeverUseManuallyClass = MemberFunctionParse<decltype(&SelfTypeNeverUseManually)>::ResultType;
#else if _MSC_VER == 1700
#define GenerateSelfType(Type) typedef Type SelfTypeNeverUseManuallyClass;
#endif


template<class T>
struct ZeroSizeOf
{
  enum{ value = 0 };
};

template<class T, unsigned int val=0>
struct ConstValueOfClass
{
	enum { value = val};
};

#define NOT_ZeroSizeOf(class_type) \
template<> \
struct ZeroSizeOf<class_type>\
{\
  enum { value = sizeof(class_type) };\
};