
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
/* Example output:

Note: <ENTER> and <ctrl+D> refers to user input.

$ ./a.out
I study at LiU <ENTER>
I study at Linköping University
I take COURSE at LiU <ENTER>
I take TDDD38 at Linköping University
<ctrl+D>

$ ./a.out
The course FULL_COURSE taught me how to fully utilize C++ <ENTER>
The course TDDD38 at Department of Computer and Information Science / Linköping University taught me how to fully utilize C++
<ctrl+D>

*/

std::map<std::string, std::string> define_macros(std::ifstream& ifs)
{
    std::map<std::string, std::string> macros;
    std::string line{};
    
    while(std::getline(ifs, line))
    {
        
        auto it = line.find(':');
        // std::string first(line.begin(), std::next(line.begin(), it));
        // std::string second(std::next(line.begin(), it + 1), line.end());
        std::string first = line.substr(0, it);
        std::string second = line.substr(it + 1);
        macros.insert({first, second});
    }
    return macros;
}

std::string expand(std::string const& line, std::map<std::string, std::string> const& macros)
{
    std::istringstream iss{line};
    std::ostringstream oss{};

    std::transform(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}, std::ostream_iterator<std::string>{oss},
        [&macros](std::string const& word) {
            auto it = macros.find(word);
            if (  it != macros.end())
            {
                return expand(macros[word], macros);
            }
            return word + " ";
        });
    


    return oss.str();

}

int main()
{

    std::ifstream ifs{"macros.txt"};
    std::map<std::string, std::string> macros = define_macros(ifs);

    std::string line{};
    std::string expanded_line{};
    


    while (std::getline(std::cin, line))
    {        
        std::cout << expand(line, macros) << std::endl;
    }
    

    


    for (auto [key, value] : macros)
    {
        std::cout << key << ":" << value << std::endl;
    }

    return 0;
}