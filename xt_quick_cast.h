#pragma once
#include "xt_std_head.h"
#include "xt_class_helper.h"
#include "xt_string.h"
/*
if U&& can be convertible to T:
result is created by the convertible way.
else:
use std::stringstream way.
(like: boost::lexical_cast, but QCast will not throw an exception, but will return an default-constructor value)

So:
'boost::lexical_cast<std::string>("")' will throw an exception;
while:
'QCast<std::string>("")' is a completely legitimate call.
because:
"" can be convertible to 'std::string'

*/

template<class T, class U>
T QCastInner(std::true_type, U&& u)
{
	return u;
}
template<class T, class U>
T QCastInner(std::false_type, const U& u)
{
	std::stringstream ss;
	T Result{};
	if (!(ss << u) || !(ss >> Result) || !(ss >> std::ws).eof())
	{
	}
	return Result;
}

template<class T, class U>
T QCastInner(std::true_type, const U& u, bool& bres)
{
	bres = true;
	return QCastInner<T>(std::true_type(), u);
}
template<class T, class U>
T QCastInner(std::false_type, const U& u, bool& bres)
{
	std::stringstream ss;
	T Result{};
	if (!(ss << u) || !(ss >> Result) || !(ss >> std::ws).eof())
	{
		bres = false;
	}
	else
	{
		bres = true;
	}
	return Result;
}


template<class T, class U>
T QCast(U&& u)
{
	using BaseU = std::remove_cv_t<U>;
	using BaseT = std::remove_cv_t<T>;
	if constexpr (std::is_same_v<BaseT, bool> && std::is_convertible_v<const BaseU&, std::string>)
	{
		auto value = XT::String::ToUpper(u);
		if(value == "0" || value == "" || value =="FALSE")
		{
			return false;
		}
		else
		{
			return true;
		}

	}
	else
	{
		using TmpType =typename  std::is_convertible<const U&, T>::type;
		return QCastInner<T>(TmpType{}, std::forward<U>(u));
	}
}

template<class T, class U>
bool QCastRes(U&& u)
{
	bool bres = false;
	QCast<T>(std::forward<U>(u), bres);
	return bres;
}

template<class T, class U>
T QCast(const U& u, bool& bres)
{
	using TmpType = typename std::is_convertible<const U&, T>::type;
	return QCastInner<T>(TmpType{}, u, bres);
}

static void QCast(std::vector<std::string>& con_str, const std::string& str, const std::string& spliter = "")
{
	con_str.clear();
	if (spliter == "")
	{
		con_str.push_back(str);
	}
	std::string::size_type init_position = 0;
	std::string::size_type next_position = 0;
	do
	{
		next_position = str.find(spliter, init_position);
		if (next_position == std::string::npos)
		{
			con_str.push_back(std::string(str.begin() + init_position, str.end()));
			break;
		}
		else
		{
			con_str.push_back(std::string(str.begin() + init_position, str.begin() + next_position));
			init_position = next_position + spliter.length();
		}
	} while (true);
}
static void QCast(std::string& str, const std::vector<std::string>& con_str, const std::string& spliter = "")
{
	str.clear();
	for (auto it = con_str.begin(); it != con_str.end(); ++it)
	{
		str += *it + (it != (con_str.end() - 1) && con_str.size() != 1 ? spliter : std::string(""));
	}
}

template<class T1, class T2, class T3>
void QCast(std::vector<T1>& vec_t1, const T2& t2, const T3& t3)
{
	std::vector<T1>().swap(vec_t1);
	std::string t2_str;
	std::string t3_str;
	{
		bool bres;
		t2_str = QCast<std::string>(t2, bres);
		if (!bres)
			return;
	}
	{
		bool bres;
		t3_str = QCast<std::string>(t3, bres);
		if (!bres)
			return;
	}
	std::vector<std::string> vec_str;
	QCast(vec_str, t2_str, t3_str);
	for (auto& sub : vec_str)
	{
		vec_t1.push_back(QCast<T1>(sub));
	}
}
template<class T1, class T2, class T3>
std::vector<T1> QCastToVec(T2&& t2, T3&& t3)
{
	std::vector<T1> vec_t1;
	QCast(vec_t1, std::forward<T2>(t2), std::forward<T3>(t3));
	return vec_t1;
}
template<class T1, class T2, class T3>
void QCast(T1& t1, const std::vector<T2>& vec_t2, const T3& t3)
{
	std::string t3_str;
	{
		bool bres;
		t3_str = QCast<std::string>(t3, bres);
		if (!bres)
			return;
	}
	std::string temp;
	std::vector<std::string> vec_str;
	for (auto& sub : vec_t2)
	{
		bool bvalid = false;
		std::string res = QCast<std::string>(sub, bvalid);
		if (bvalid)
		{
			vec_str.push_back(res);
		}
	}
	QCast(temp, vec_str, t3_str);
	t1 = QCast<T1>(temp);
}

template<class T1, class T2, class T3>
T1 QCastFromVec(const std::vector<T2>& vec_t2, const T3& t3)
{
	T1 t1;
	QCast(t1, vec_t2, t3);
	return t1;
}

static std::vector<std::string> MKSTRVec()
{
	return{};
}

template<class T, typename... Args>
std::vector<std::string> MKSTRVec(T&& t, Args&&... args) {
	auto&& vec = MKSTRVec(std::forward<Args>(args)...);
	std::vector<std::string> result;
	result.push_back(MKSTR(std::forward<T>(t)));
	result.insert(result.end(), vec.begin(), vec.end());
	return result;
}

template<typename... Args>
std::string MKMAPSTR(Args&&... args)
{
	auto&& vec = MKSTRVec(std::forward<Args>(args)...);
	std::string result;
	for (int index = 0; index < vec.size(); index++)
	{
		if (index % 2 == 0)
		{
			result += "{" + vec[index] + ":";
		}
		else
		{
			result += vec[index] + "},";
		}
	}
	return result;
}




template<typename T, typename... Args>
std::string MKSTR(T&& t, Args&&... args)
{
	return QCast<std::string>(std::forward<T>(t)) + MKSTR(std::forward<Args>(args)...);

}
static std::string MKSTR()
{
	return "";
}

template<class Type>
std::string FixedPrecision(Type&& t, int prescription = 1)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(prescription) << std::forward<Type>(t);
	return ss.str();
}
template<class Type>
std::string FixedWidth(Type&& value, int width, char placeholder = ' ')
{
	std::stringstream ss;
	ss << std::setw(width) << std::setfill(placeholder) << value;
	std::string result;
	ss >> result;
	return result;
}



class InvalidValueException : public std::runtime_error
{
public:
	InvalidValueException(const std::string& str = "") :std::runtime_error{ str }{}
};

//if qcast failed, throw an exception.
template<class T, class U>
T TCast(U&& u)
{
	bool bres = false;
	auto res = QCast<T>(std::forward<U>(u), bres);
	if (!bres) throw InvalidValueException("TCast");
	return res;
}


template<class T>
std::string QCastToStr(T&& t)
{
	return std::is_floating_point<REMOVE_QUALIFIERS(t)>::value ? FixedPrecision(t) : QCast<std::string>(t);
}

template<class T> T false_filter(const T& value)
{
	if (!value) throw InvalidValueException("false_filter");
	return value;
}

#define FALSE_FILTER_SPECIFIC_MESSAGE(expr, meg) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(false_filter(expr), meg)
#define FALSE_FILTER(expr)  VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(false_filter(expr), MKSTR(#expr, " is false!"))

static std::string empty_str_filter(const std::string& str) {
	if (str.empty()) throw InvalidValueException("empty_str_filter");
	return str;
}

#define EMPTY_STR_FILTER_SPECIFIC_MESSAGE(expr, meg) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(empty_str_filter(expr), meg)
#define EMPTY_STR_FILTER(expr) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(empty_str_filter(expr), MKSTR(#expr, " is not a empty string!"))

template<class T>
T positive_num_filter(T number)
{
	if (number <= 0) throw InvalidValueException("positive_num_filter");
	return number;
}
#define POSITIVE_NUM_FILTER_SPECIFIC_MESSAGE(expr, meg) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(positive_num_filter(expr), meg)
#define POSITIVE_NUM_FILTER(expr) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(positive_num_filter(expr), MKSTR(#expr, " is not a positive value!"))
//[begin, end)
template<class T>
T range_num_filter(T number, T begin, T end)
{
	if (number >= begin && number < end) return number;
	throw InvalidValueException("range_num_filter");
}
#define RANGE_NUM_FILTER_SPECIFIC_MESSAGE(number, begin, end, meg) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(range_num_filter(number, begin, end),meg)
#define RANGE_NUM_FILTER(number, begin, end) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(range_num_filter(number, begin, end), MKSTR(#number," is not in[",#begin,",",#end,")"))
template<class T>
T not_negative_num_filter(T number)
{
	if (number < 0) throw InvalidValueException("not_negative_num_filter");
	return number;
}
#define NOT_NEGATIVE_NUM_FILTER_SPECIFIC_MESSAGE(expr, meg) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(not_negative_num_filter(expr), meg)
#define NOT_NEGATIVE_NUM_FILTER(expr) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(not_negative_num_filter(expr), MKSTR(#expr, " is a negative value!"))



#define  TCAST_TO_FLOAT_SPECIFIC_MESSAGE(expr, meg) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(TCast<float>(expr), meg)
#define  TCAST_TO_FLOAT(expr) VALUE_CHECK_EXCEPTION_SPECIFIC_MESSAGE(TCast<float>(expr), MKSTR(#expr, " cast to float failed!"))


#define FALSE_FILTER_LOG(expr)\
[&]()->bool{\
	auto&& result = expr;\
	if (!result) {\
		LOGE(#expr);\
		return false;\
	}\
	return true;\
}()

#define IF_FALSE_LOG(expr) if(!FALSE_FILTER_LOG(expr))

const std::string TupleSpliter = ",";
template<class TupleType, std::size_t N>
struct TupSerializationInner {
	static std::string TupSerialization(const TupleType& t, const std::string& spliter = TupleSpliter)
	{
		return TupSerializationInner<TupleType, N - 1>::TupSerialization(t) + spliter + QCast<std::string>(std::get<N - 1>(t));
	}
};

template<class TupleType>
struct TupSerializationInner<TupleType, 1> {
	static std::string TupSerialization(const TupleType& t, const std::string& spliter = TupleSpliter)
	{
		return QCast<std::string>(std::get<0>(t));
	}
};

template<class TupleType>
struct TupSerializationInner<TupleType, 0> {
	static std::string TupSerialization(const TupleType& t, const std::string& spliter = TupleSpliter)
	{
		return "";
	}
};

template<class... Args>
std::string TupSerialization(const std::tuple<Args...>& t, const std::string& spliter = TupleSpliter)
{
	return TupSerializationInner<decltype(t), sizeof...(Args)>::TupSerialization(t, spliter);
}







template <typename ... Ts/*, std::size_t N*/, std::size_t...Is, typename vecT>
std::tuple<Ts...>
TupDeserializationFromVecInner(const std::vector<vecT>& s,
	std::index_sequence<Is...>)
{
	return std::tuple<Ts...>(QCast<Ts>(s[Is])...);
}

template <typename ... Ts, std::size_t N = sizeof...(Ts), typename vecT>
std::tuple<Ts...> TupDeserializationFromVec(const std::vector<vecT>& s)
{
	assert(N == s.size());
	return TupDeserializationFromVecInner<Ts...>(s, std::index_sequence_for<Ts...>());
}

template<typename... Ts, typename vecT>
bool VecToTupDeserialization(const std::vector<vecT>& v, std::tuple<Ts...>& result)
{
	result = TupDeserializationFromVec<Ts...>(v);
	return true;
}
template<typename... Ts>
bool TupDeserialization(const std::string& value, std::tuple<Ts...>& result, const std::string & spliter = TupleSpliter)
{
	std::vector<std::string> vec;
	QCast(vec, value, spliter);
	return  VecToTupDeserialization(vec, result);
}