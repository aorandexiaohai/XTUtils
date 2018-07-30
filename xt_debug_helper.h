#pragma once
#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_HELPER(expression) expression
#define DEBUG_PAUSE  FunctionOverGuard<> fog_25AB818D_2DF9_4419_9B9A_4D3BB3E7A391{[](){std::cout << "debug pause"<<std::endl; std::cin.clear(); std::cin.get();}};
#else
#define DEBUG_HELPER(expression)
#define DEBUG_PAUSE 
#endif

#define FORCE_PAUSE  FunctionOverGuard<> fog_25AB818D_2DF9_4419_9B7A_4D3BB3E7A391{[](){std::cout << "force pause"<<std::endl; std::cin.clear(); std::cin.get();}};