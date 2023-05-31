#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

std::map<std::string, std::string> define_macros(std::ifstream & ifs)
{
    std::map<std::string, std::string> macros;
    std::string line{};
    while(std::getline(ifs,line))
    {
        auto it = line.find(':');
        std::string key = line.substr(0, it);
        std::string value = line.substr(it + 1);
        macros[key] = value;
    }
    return macros;
}

std::string expand(std::string const& line, std::map<std::string, std::string> const& macros)
{
    std::istringstream iss{line};
    std::ostringstream oss{};
    std::transform(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}, std::ostream_iterator<std::string>{oss},
        [&macros](std::string const& word)
        {
            auto it = macros.find(word);
            if (it != macros.end())
            {
                return expand(it->second, macros);
            }
            return word + " ";
        });
    return oss.str();
}

int main()
{
    std::ifstream ifs{"macros.txt"};
    std::map<std::string, std::string> macros {define_macros(ifs)};
    std::string line{};



    while(std::getline(std::cin, line))
    {
        std::cout << expand(line, macros) << std::endl;
    }

    return 0;
}