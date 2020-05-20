#include <string>
#include <iostream>
using std::istream;
using std::ostream;
using std::string;

class Person
{
    friend istream &read(istream&, Person&);
    friend ostream &print(ostream&, Person&);
    private:
        string name;
        string address;
    public:
        string Name();
        string Address();
        Person():Person("",""){}
        Person(string n, string a):name(n), address(a){}
        Person(istream &is){is>>this->name>>this->address;}
        explicit Person(string n):Person(n,""){}
        ostream &print(ostream &os);
};

string Person::Name()
{
    return this->name;
}

string Person::Address()
{
    return this->address;
}

istream &read(istream &is, Person &p)
{
    is >> p.name >> p.address;
    return is;
}

ostream &print(ostream &os, Person &p)
{
    os << "Name: " << p.name << "\nAddress: " << p.address;
    return os;
}

ostream &Person::print(ostream &os)
{
    os << "Name: " << this->name << "\nAddress: " << this->address;
    return os;
}