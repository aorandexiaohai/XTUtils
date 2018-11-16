#pragma once
#include "xt_std_head.h"
struct AsyncCout : public std::stringstream {
	static std::mutex cout_mutex;
	~AsyncCout() {
		std::lock_guard<std::mutex> l{ cout_mutex };
		std::cout << rdbuf();
		std::cout.flush();
	}
};
#define ACOUT AsyncCout{}