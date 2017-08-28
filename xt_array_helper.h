#pragma once
template<class T>
struct IsArray {
  static const bool value = std::is_array<T>::value;
  typedef typename std::conditional<value, int, void>::type FakeCheckType;
};

//test case.
//IsArray<int[2]>::value;
//IsArray<int[2]>::FakeCheckType;
//IsArray<int*>::value;
//IsArray<int*>::FakeCheckType;
//IsArray<int>::value;
//IsArray<int>::FakeCheckType;