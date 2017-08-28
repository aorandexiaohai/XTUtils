#pragma once
#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_HELPER(expression) expression
#else
#define DEBUG_HELPER(expression)
#endif