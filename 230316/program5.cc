#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>

int main()
{
    // Test negative cases (i.e. where it return false)
    static_assert( !has_iterator<int>::value );
    static_assert( !has_iterator<std::ostream>::value );

    // Test containers
    static_assert( has_iterator<std::string>::value );
    static_assert( has_iterator<std::vector<int>>::value );
    static_assert( has_iterator<std::map<std::string, int>>::value );
    static_assert( has_iterator<std::set<float>>::value );

    // test C-arrays
    static_assert( has_iterator<char(&)[4]>::value );
    static_assert( has_iterator<int(&)[12]>::value );
}
