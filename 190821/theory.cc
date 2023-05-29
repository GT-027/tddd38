#include <utility>
#include <iostream>


template<typename... Args>
auto sum(Args... args) {
    return (args + ...); // (a) fold -expression that adds all the args together.
}

struct A // (c)
{
    void foo() &&{
        std::cout << "called" << std::endl;
    }
};

template <int N> // non-type template parameter. (d)
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<> // (d)
struct Factorial<0> {
    static constexpr int value = 1;
};

// e

class Base {
public:
    int a;
};

class Derived : public Base {
public:
    int b;
};


int main()
{
    A a;
    std::move(a).foo(); // (c) this is an example of a valid call. we reate an rvalue reference to a which allows us to call A::foo()
    int result = sum(1, 2, 3, 4, 5);
    std::cout << "result: " << result << std::endl;
    constexpr int fact5 = Factorial<5>::value;
    constexpr int fact10 = Factorial<10>::value;
    std::cout << fact5 << " " << fact10 << std::endl;

    Derived d;
    d.a = 1;
    d.b = 2;

    Base b = d;

    std::cout << b.a << std::endl;
    // std::cout << b.b << std::endl; // error
    return 0;
}


/* (b) The override keyword is used to explicitly indicate that a function in 
a derived class is intended to override a virtual function in the base class.
If the functin signature in the derived class does not match the signature in the base
class exactly the function may not be overriden as intended.
Using the override keyword ensure that the function in the derived class has the same 
signature as the function in the base class. If the function in the derived class does not match the signature in the 
base class a compilation error will occur.
*/

/* (d) in C++, templates can have two types of parameters: type template parameters and non-type template parameters.
The main difference between type template parameters and non-type template parameters is that type template parameters 
specifuc types, wheras non-type template parameters specify values. 

template<typename T> //type template parameter
void print(T value) {
    std::cout << value << std::endl;
}
*/

/* (e) object slicing occurs when a derived class object is copied or assigned to a base class object, resulting in the
 loss of derived class-specific information and behaviour. The base class is typically smaller than the derived class object,
 so copying or assigning a derived class object to a base class object will discard any additional data that that is specific to the
 derived class. see example

 To avoid object slicing, it is recommended to use pointers or references to the base class instead of copying or assigning 
 derived class objects to base class objects. 




*/


