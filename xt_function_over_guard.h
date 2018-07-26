///author wuzhenhai
#pragma once
#include "xt_std_head.h"
#include "xt_general.h"
struct function_nullptr_t {
	template<class T>
	operator std::function<T()>() {
		return std::function<T()>{};
	}
};
static function_nullptr_t function_nullptr;

class NonNormalEndFunctionException : public std::exception {
public:
	char const* what() const override 
	{
		return "NonNormalEndFunctionException";
	}
};

//can only be moved, can not be copied.
template<class Result = void >
class FunctionOverGuard
{
public:
	FunctionOverGuard(std::function<Result()> fun=function_nullptr, bool state=true) : function_end_(fun),state_(state){}
	~FunctionOverGuard()
	{
		if(state_ && function_end_)
			function_end_();
	}
	FunctionOverGuard(const FunctionOverGuard& func) = delete;
	FunctionOverGuard& operator=(const FunctionOverGuard& func) = delete;
	FunctionOverGuard(FunctionOverGuard&& func)
	{
		state_ = func.state_;
		function_end_ = std::move(func.function_end_);
	}
	FunctionOverGuard& operator=(FunctionOverGuard&& func) {
		if (function_end_) throw NonNormalEndFunctionException{};
		state_ = func.state_;
		function_end_ = std::move(func.function_end_);
		return *this;
	}
protected:
	SetMacro(std::function<Result()>, function_end)
	SetGetMacro(bool, state)
};

//can only be moved, can not be copied.
template<class Result = void>
class ScopedFunction {
public:
  ScopedFunction(
    std::function<Result()> function_begin = function_nullptr,
    std::function<Result()> function_end = function_nullptr,
    bool state = true) :function_begin_{ function_begin }, function_end_{ function_end }, state_{state}
  {
    if (state && function_begin_) {
      function_begin_();
    }
  }
  ~ScopedFunction() {
    if (state_ && function_end_) {
      function_end_();
    }
  }

  ScopedFunction(const ScopedFunction& func) = delete;
  ScopedFunction& operator=(const ScopedFunction& func) = delete;

  ScopedFunction(ScopedFunction&& func)
  {
	  state_ = func.state_;
	  function_begin_ = std::move(func.function_begin_);
	  function_end_ = std::move(func.function_end_);
  }
  ScopedFunction& operator=(ScopedFunction&& func) {
	  if (function_begin_ || function_end_) throw NonNormalEndFunctionException{};
	  state_ = func.state_;
	  function_begin_ = std::move(func.function_begin_);
	  function_end_ = std::move(func.function_end_);
	  return *this;
  }
protected:
	SetGetMacro(std::function<Result()>, function_begin)
		SetGetMacro(std::function<Result()>, function_end)
		SetGetMacro(bool, state)
};