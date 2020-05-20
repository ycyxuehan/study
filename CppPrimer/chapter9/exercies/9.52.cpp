#include <iostream>
#include <stack>
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::stack;
using std::string;
using std::to_string;
using std::vector;

int calc(string oper, int a, int b)
{
    if (oper == "+")
    {
        return a + b;
    }
    if (oper == "-")
    {
        return a - b;
    }
    if (oper == "*")
    {
        return a * b;
    }
    if (oper == "/")
    {
        if (b == 0)
        {
            return 0;
        }
        return a / b;
    }
}

bool priority(string oper1, string oper2)
{
    if (oper1 == "*" || oper1 == "/")
        return true;
    if (oper2 == "*" || oper2 == "/")
        return false;
    return true;
}
vector<string> reverse(const vector<string> & vs)
{
    vector<string> res;
    for(auto crbeg = vs.crbegin(); crbeg != vs.crend(); ++ crbeg)
    {
        res.push_back(*crbeg);
    }
    return res;

}
void reverse(stack<string> &ss)
{
    vector<string> vs;
    while (!ss.empty())
    {
        vs.push_back(ss.top());
        ss.pop();
    }
    for (auto crbeg = vs.crbegin(); crbeg != vs.crend(); ++crbeg)
    {
        ss.push(*crbeg);
    }
}

int do_expr(stack<string> &expr)
{
    if (expr.empty())
    {
        return 0;
    }
    string oper1, oper2;
    string number1, number2;
    while (!expr.empty())
    {
        auto curr = expr.top();
        if (number1.empty())
        {
            number1 = curr;
            expr.pop();
            continue;
        }
        if (oper1.empty())
        {
            oper1 = curr;
            expr.pop();
            continue;
        }
        if (number2.empty())
        {
            number2 = curr;
            expr.pop();
            continue;
        }
        if (oper2.empty())
        {
            oper2 = curr;
            expr.pop();
            continue;
        }
        if(!oper1.empty() && !oper2.empty())
        {
            if (priority(oper1, oper2))
            {
                auto v = calc(oper1, atoi(number1.c_str()), atoi(number2.c_str()));
                number1 = to_string(v);
                oper1 = oper2;
                number2 = curr;
                oper2 = "";
            }
            else
            {
                auto v = calc(oper2, atoi(number2.c_str()), atoi(curr.c_str()));
                number2 = to_string(v);
                oper2= "";
            }
            expr.pop();
        }
    }
    auto v = calc(oper1, atoi(number1.c_str()), atoi(number2.c_str()));
    // expr.push(to_string(v));
    return v;
}

int expr(stack<string, vector<string>> &cs)
{
    stack<string> prev;
    stack<string> expr;
    bool begin_expr = false;
    bool end_expr = false;
    //
    while (!cs.empty() && !end_expr)
    {
        auto curr = cs.top();
        if (curr == "(")
        {
            begin_expr = true;
        }
        if (begin_expr && curr == ")")
        {
            end_expr = true;
        }
        if (begin_expr)
        {
            if (curr != "(" && !end_expr)
            {
                expr.push(curr);
            }
        }
        else
        {
            prev.push(curr);
        }
        cs.pop();
        if(end_expr)
        {
            reverse(expr);
            auto v = do_expr(expr);
            cs.push(to_string(v));
            while (!prev.empty())
            {
                cs.push(prev.top());
                prev.pop();
            }
            begin_expr = false;
            end_expr = false;
        }
    }
    auto v = do_expr(prev);
    cs.push(to_string(v));
    return v;
}


void dislay(const vector<string> &d)
{
    cout << "expr: ";
    for (auto cbeg = d.cbegin(); cbeg != d.cend(); ++cbeg)
    {
        cout << *cbeg;
    }
    cout << ", result: ";
}

int main()
{
    vector<string> vs1 = {"1", "+", "(", "2", "+", "3", "*", "4", ")", "*", "5"};
    vector<string> vs2 = {"1", "+", "(", "2", "+", "3", "-", "4", ")", "*", "5"};
    vector<string> vs3 = {"1", "*", "(", "2", "+", "3", "*", "4", ")", "+", "5"};
    auto rvs1 = reverse(vs1);
    dislay(vs1);
    stack<string, vector<string>> ss1(rvs1);
    cout<<expr(ss1) <<endl;

    auto rvs2 = reverse(vs2);
    dislay(vs2);
    stack<string, vector<string>> ss2(rvs2);
    cout<<expr(ss2) <<endl;

    auto rvs3 = reverse(vs3);
    dislay(vs3);
    stack<string, vector<string>> ss3(rvs3);
    cout<<expr(ss3) <<endl;

}