#pragma once
#include "xt_log_helper.h"
#include "xt_class_helper.h"
#define XTEXCEPTION_HELPER(expression)\
try\
{\
	expression;\
}\
catch(...)\
{\
	XTLOGE("an exception happen!");\
	XTLOGE(#expression);\
	throw;\
}


#if defined(DEBUG) || defined(_DEBUG)
#define XTEXCEPTION_DEBUG_HELPER(expression) XTEXCEPTION_HELPER(expression)
#else
#define XTEXCEPTION_DEBUG_HELPER(expression) expression
#endif


#define XTVALUE_CHECK_EXCEPTION(expr)\
[&]()->decltype(expr){\
	try\
	{\
		return expr;\
	}\
	catch (...)\
	{\
		XTLOGE("exception here!"); \
		throw;\
	}\
}()