#pragma once

#include "xt_std_head.h"

template <typename F>
static auto asynchronize(F f)
{
	return [f](auto ... xs) {
		return [=]() {
			return std::async(std::launch::async, f, xs...);
		};
	};
}

template <typename F>
static auto fut_unwrap(F f)
{
	return [f](auto ... xs) {
		return f(xs.get()...);
	};
}

template <typename F>
static auto async_adapter(F f)
{
	return [f](auto ... xs) {
		return [=]() {
			return async(std::launch::async,
				fut_unwrap(f), xs()...);
		};
	};
}


//example
//#include "xt_tiny_parallel.h"
//#include "xt_asyc_cout.h"
//
//int main()
//{
//	//demo 1: string join/twice
//	{
//		auto pcreate(asynchronize([](const char *s)
//		{
//			ACOUT << "this_id:" << std::this_thread::get_id() << "    1s CREATE " << s << '\n';
//			std::this_thread::sleep_for(std::chrono::seconds{ 1 });
//			return s;
//		}));
//		auto pconcat(async_adapter([](const std::string &a, const std::string &b)
//		{
//			ACOUT << "this_id:" << std::this_thread::get_id() << "    1s CONCAT " << a << " " << b << '\n';
//			std::this_thread::sleep_for(std::chrono::seconds{ 1 });
//			return a + b;
//		}));
//		auto ptwice(async_adapter([](const std::string &s)
//		{
//			ACOUT << "this_id:" << std::this_thread::get_id() << "    1s TWICE " << s << '\n';
//			std::this_thread::sleep_for(std::chrono::seconds{ 1 });
//			return s + s;
//		}));
//
//		auto result(
//			pconcat(
//				ptwice(
//					pconcat(
//						pcreate("foo "),
//						pcreate("bar "))),
//				pconcat(
//					pcreate("this "),
//					pcreate("that "))));
//		ACOUT << result().get() << '\n';
//	}
//	//demo 2: set a - set b
//	{
//		//generate set random
//		auto create_set(asynchronize([]()
//		{
//			std::set<int> tmps;
//			static int x = 0;
//			if (x++ == 0)
//			{
//				for (int index = 0; index <  999 ; index++)
//				{
//					tmps.insert(std::rand() % 1000000);
//				}
//			}
//			else
//			{
//				for (int index = 0; index < 1999; index++)
//				{
//					tmps.insert(std::rand() % 1000000);
//				}
//			}
//			return tmps;
//		}));
//		auto diff_set(async_adapter( [](const std::set<int>& a, const std::set<int>& b) {
//			std::set<int> result;
//			std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::inserter(result, result.end()));
//			return result;
//		} ));
//
//		auto result(
//			diff_set(create_set(), create_set())
//		);
//		auto set_result = result().get();
//		ACOUT << set_result.size() << std::endl;
//	}
//	std::cin.get();
//}
