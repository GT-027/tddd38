#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <numeric>

namespace std
{
std::istream & operator>>(std::istream & is, std::vector<int>& v)
{
    std::string line{};
    std::getline(is, line);
    std::stringstream ss{line};


    v = std::vector<int>{
        std::istream_iterator<int>{ss},
        std::istream_iterator<int>{}};

    return is;
}

}



int main()
{
    std::ifstream ifs{"data.txt"};
    std::vector<std::vector<int>> data{
        std::istream_iterator<std::vector<int>>{ifs},
        std::istream_iterator<std::vector<int>>{}};

    

    int N{};
    std::cin >> N;

    std::vector<double> averages{};
    std::transform(data.begin(), data.end(), std::back_inserter(averages),
        [](std::vector<int>& row) -> double
        {
            return std::accumulate(row.begin(), row.end(), 0.0)/row.size();
        });

    std::partial_sort(averages.begin(), std::next(averages.begin(), N), averages.end(), std::greater{});


    std::copy_n(averages.begin(), N, std::ostream_iterator<double>(std::cout, "\n"));


    //std::cout << std::endl;


    return 0;
}