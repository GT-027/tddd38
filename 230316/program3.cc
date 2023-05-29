#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <ostream>
#include <map>

// Implement the classes here

class Action
{
public:
  
    virtual ~Action() = default;
    virtual void evaluate(std::stack<int>& stack) const = 0;
    virtual Action* clone() const = 0;
};

class Literal : public Action
{
public:
    Literal(int value)
        : value{value}
    {}

    void evaluate(std::stack<int>& stack) const override
    {
        stack.push(value);
    }
    Action* clone() const override
    {
        return new Literal{value};
    }
private:
    int value;
};

class Print : public Action
{
public:
    Print(std::ostream & output)
        : output{output}
    {}

    void evaluate(std::stack<int>& stack) const override
    {
        output << stack.top() << std::endl;
        stack.pop();
    }
    Action* clone() const override
    {
        return new Print{output};
    }

private:
    std::ostream& output;
};

template<typename T>
class Operator : public Action
{
public:
    Operator(T const& function)
        : function{function}
    {}

    void evaluate(std::stack<int>& stack) const override
    {
        int lhs {stack.top()};
        stack.pop();
        int rhs {stack.top()};
        stack.pop();
        stack.push(function(lhs, rhs));
    }
    Action* clone() const override
    {
        return new Operator<T>{function};
    }

private:
    T function;
};


// Implement the make_literal(), make_print() and make_operator() functions here

Action* make_literal(int value)
{
    return new Literal{value};
}

Action* make_print(std::ostream & os)
{
    return new Print{os};
}

template<typename T>
Action* make_operator(T const& function)
{
    return new Operator<T>{function};
}

void evaluate_expression(std::vector<Action*> const& expression)
{
    std::stack<int> stack { };
    for (Action* action : expression)
    {
        action->evaluate(stack);
    }
}


/* Associations that should be be stored in parse_table:
*/
std::map<std::string, Action*> const parse_table {
    {"."   , make_print(std::cout) },
    {"+"   , make_operator([](int x, int y) { return x + y; }) },
    {"*"   , make_operator([](int x, int y) { return x * y; }) },
    {"-"   , make_operator([](int x, int y) { return x - y; }) },
    {"/"   , make_operator([](int x, int y) { return x / y; }) },
    {"min" , make_operator([](int x, int y) { return std::min(x, y); }) },
    {"max" , make_operator([](int x, int y) { return std::max(x, y); }) }
};
    // Note: It should be very easy to for example add something like:
    // "^"   -> make_operator([](int x, int y) { return std::pow(x, y); }


Action* parse_token(std::string const& token)
{
    if (auto it = parse_table.find(token); it != parse_table.end())
    {
        return it->second->clone();
    }
    else
    {
        int value { std::stoi(token)};
        return make_literal(value);
    }
}

std::vector<Action*> parse_expression(std::istream& is)
{
    std::vector<Action*> expression { };
    
    std::string token;
    while (is >> token)
    {
        expression.push_back(parse_token(token));
    }
    
    return expression;
}

int main()
{ 
    std::string line;
    std::getline(std::cin, line);

    std::istringstream iss { line };

    auto expression = parse_expression(iss);
    evaluate_expression(expression);

    for (Action* action : expression)
    {
        delete action;
    }

    for (auto [key, action] : parse_table)
    {
        delete action;
    }
}

/* Example runs:

$ ./a.out
1 2 + . <enter>
3

$ ./a.out
1 . 2 3 + . <enter>
1
5

$ ./a.out
11 12 13 max min .
11

$ ./a.out
1 2 + 10 * .
30

 */
