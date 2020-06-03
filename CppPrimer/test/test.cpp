#include <iostream>

class Main //Main是类，是对象定义
{
    public:
    Main()=default;         //Main()是构造函数
    Main(int n):main(n){}
    private:
    int main;  //main是成员
    public:
    int get_main(){return main;}; //get_main()是成员函数
};

int main() //main()是函数
{
    Main main(3); //main是对象，是类Main的实例
    std::cout<<main.get_main()<<std::endl;
    return 0;
}