#pragma once
#include <cstdlib>
//[0,1)
inline double XTRandomNumber()
{
	return (std::rand() % 100) / 100.0;
}