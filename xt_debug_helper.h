#pragma once
#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_HELPER(expression) expression
#define DEBUG_PAUSE  FunctionOverGuard<> fog_25AB818D_2DF9_4419_9B9A_4D3BB3E7A391{[](){std::cout << "\ndebug pause\n"; std::cin.clear(); std::cin.get();}};

#define DEBUG_CODE_OUTPUT(expression)\
std::cout<<"source code:\n"\
<<#expression<<"\n"\
<<"begin:\n\n";\
expression \
std::cout << "\nfinished\n";

#else
#define DEBUG_HELPER(expression)
#define DEBUG_PAUSE 
#define DEBUG_CODE_OUTPUT(expression) expression
#endif

#define FORCE_PAUSE  FunctionOverGuard<> fog_25AB818D_2DF9_4419_9B7A_4D3BB3E7A391{[](){std::cout << "\nforce pause\n"; std::cin.clear(); std::cin.get();}};