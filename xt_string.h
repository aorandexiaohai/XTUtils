#pragma once
#include <cassert>
#include <map>
#include <algorithm>
#include <vector>
namespace XT
{
class String
{
public:
    static  std::string AddPrefix(std::string str, std::string prefix, bool b_check_exist)
    {
        if (prefix == "")
            return str;
        auto AddPrefixInteral = [=](std::string &str_hs, std::string prefix_hs) {
            if (prefix_hs == "")
                assert(false);
            bool b_need_prefix = false;
            if (str_hs.size() >= prefix_hs.size())
            {
                std::string temp(str_hs.begin(), str_hs.begin() + prefix_hs.size());
                if (temp != prefix_hs)
                    b_need_prefix = true;
            }
            else
                b_need_prefix = true;
            if (b_need_prefix || !b_check_exist)
                str_hs = prefix_hs + str_hs;
        };
        AddPrefixInteral(str, prefix);
        return str;
    }

    static  std::string Replace(std::string str, const std::string &old_replacement, const std::string &new_replacement)
    {
        if ((str == "") || (old_replacement == ""))
            return str;
        std::vector<size_t> index_vec;
        size_t begin_offset = 0;
        size_t finded_index = std::string::npos;
        int step = old_replacement.size();
        while ((finded_index = str.find(old_replacement, begin_offset)) != std::string::npos)
        {
            index_vec.push_back(finded_index);
            begin_offset = finded_index + step;
        }
        std::string new_str = "";
        for (size_t i = 0; i < str.size();)
        {
            if (std::find(index_vec.begin(), index_vec.end(), i) != index_vec.end())
            {
                new_str += new_replacement;
                i += step;
            }
            else
            {
                new_str += str[i];
                i++;
            }
        }
        return new_str;
    }

    static  std::string ToUpper(std::string str)
    {
        static std::map<char, char> lower_upper_char_map;
        static bool bis_init = false;
        if (!bis_init)
        {
            char c1, c2;
            for (c1 = 'a', c2 = 'A'; c1 <= 'z'; c1++, c2++)
            {
                lower_upper_char_map[c1] = c2;
            }
            bis_init = true;
        }
        for (size_t i = 0; i < str.size(); i++)
        {
            if (str[i] >= 'a' && str[i] <= 'z')
                str[i] = lower_upper_char_map[str[i]];
        }
        return str;
    }
    static  std::string ToLower(std::string str)
    {
        static std::map<char, char> upper_lower_char_map;
        static bool bis_init = false;
        if (!bis_init)
        {
            char c1, c2;
            for (c1 = 'a', c2 = 'A'; c1 <= 'z'; c1++, c2++)
            {
                upper_lower_char_map[c2] = c1;
            }
            bis_init = true;
        }
        for (size_t i = 0; i < str.size(); i++)
        {
            if (str[i] >= 'A' && str[i] <= 'Z')
                str[i] = upper_lower_char_map[str[i]];
        }
        return str;
    }

    static  std::string Trim(std::string str, char c)
    {
        if (str == "")
            return str;
        auto it_first = std::find_if(str.begin(), str.end(), [=](char c_hs) { return c_hs != c; });
        auto it_last = std::find_if(str.rbegin(), str.rend(), [=](char c_hs) { return c_hs != c; });
        if (it_first != str.end())
            return std::string(it_first, it_last.base());
        return "";
    }

    static  std::string NonRegexSpecialCharacter(std::string str)
    {
        /*
		* . ? + $ ^ [ ] ( ) { } | \
		*/
        std::string prefix = "\\";
        std::vector<std::string> str_vec = {"\\" /*first!!*/, "*", ".", "?", "+", "$", "^", "[", "]", "(", ")", "{", "}", "|"};
        for (auto &&sub : str_vec)
        {
            str = String::Replace(str, sub, prefix + sub);
        }
        return str;
    }
};
} // namespace XT