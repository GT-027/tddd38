#include <string>
#include <vector>
#include <stdexcept> // exceptions
#include <cassert>
#include <ostream>
#include <sstream>
#include <iomanip>

/* implement your solution here */

template<typename T>
class Formatter
{
public:
    static void format(std::ostream& os, T const& object)
    {
        os << object;
    }
};

template<>
class Formatter<std::string>
{
public:
    static void format(std::ostream& os, std::string const& object)
    {
        os << '<' << object << '>';
    }
};

template<typename T>
class Formatter<std::vector<T>>
{
public:
    static void format(std::ostream& os, std::vector<T> const& vector)
    {
        os << "{";
        for (T const& v : vector)
        {
            os << " ";
            Formatter<T>::format(os, v);
            
        }
        os << " }";
    }
};

namespace details
{
    void format(std::ostream& os, std::string string)
    {
        auto index = string.find("{}");
        if (index != std::string::npos)
        {
            throw std::runtime_error{"Excessive format specifier"};
        }
        os << string;
    }
    template<typename T, typename... Args>
    void format(std::ostream& os, std::string string, T const& arg, Args const&... args)
    {
        auto index = string.find("{}");
        if (index == std::string::npos)
        {
            throw std::runtime_error{"Extra argument passed to format"};
        }
        std::string prefix {string};
        // "vector: {}:{}:{}"
        os << prefix.erase(index);
        Formatter<T>::format(os, arg);
        details::format(os, string.substr(index + 2), args...);
    }
}


template<typename ...Args>
std::string format(std::string const& string, Args&&...args)
{
    std::ostringstream oss{};
    details::format(oss, string, args...);
    return oss.str();


}

int main()
{
    std::string str1 { format("Hello world!") };
    assert(str1 == "Hello world!");
    
    std::string str2 { format("int: {}", 5) };
    assert(str2 == "int: 5");

    std::string my_string { "my string" };
    
    std::string str3 { format("string: {}", my_string) };
    assert(str3 == "string: <my string>");

    std::string str4 { format("{}:{}:{}", 1, 2, 3) };
    assert(str4 == "1:2:3");

    std::vector<std::string> v { "abc", "def", "ghi" };
    
    std::string str5 { format("vector: {}", v) };
    assert(str5 == "vector: { <abc> <def> <ghi> }");
    
    try
    {
        format("{}");
        assert(false);
    }
    catch (...) { }

    try
    {
        format("my string", 5);
        assert(false);
    }
    catch (...) { }
}