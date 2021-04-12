// 1、auto
{
    int x = 0;
    auto r1 = x;    //auto=>int
    auto &r1 = x;   //auto=>int
    auto *p1 = &x;  //auto=>int
    auto p2 = &x;   //auto=>int*
    //const
    {
        const auto n = x;   //auto=>int
        auto f = n;         //auto=>int(const 被丢弃)
        const auto &r1 = x; //auto=>int
        auto &r2= r1;       //auto=>const int
    }
    // auto 的限制
    // 1、不能用于函数参数（auto要求变量是初始化的）
    // 2、不能用于静态变量
    // 3、不能用于定义数组
    // 4、不能用于模板参数（typename的类型参数）
}

// 2、decltype
{
    // auto 和 decltype 关键字都可以自动推导出变量的类型，区别
    //     auto 根据=右边的初始值 value 推导出变量的类型
    //     decltype 根据 exp 表达式推导出变量的类型
    int a = 0;
    decltype(a) b = 1;      //=>int
    decltype(1.1) c = 2.2;  //=>double
    decltype(c+1) d;        //=>double

    // 区别:
    int n = 10;
    int &r1 = n;
    auto r2 = r1;           //auto=>int (没有引用)
    decltype(r1) r3 = n;    //int& (保留引用)
}

// 3、C++返回值类型后置（跟踪返回值类型）
{
    {
        template <typename R, typename T, typename U>
        R add(T t, U u)
        {
            return t+u;
        }
        int a = 1; float b = 2.0;
        auto c = add<decltype(a + b)>(a, b);
        // 使用十分不方便,外部其实并不知道参数之间应该如何运算,只有 add 函数才知道返回值应当如何推导
    }
    // 返回值是前置语法+decltype
    {
        template <typename T, typename U>
        decltype((*(T*)0) + (*(U*)0)) add(T t, U u)
        {
            return t + u;
        }
        // 使用 decltype 完成了返回值的推导，但写法过于晦涩
        // 在返回值类型推导上的使用难度并降低代码的可读性。
    }
    // ==》》 C++11 中增加了返回类型后置（trailing-return-type，又称跟踪返回类型）语法
    //  decltype 和 auto 结合起来完成返回值类型的推导。
    template<typename T, typename U>
    auto add(T t, U u) -> decltype(t+u)
    {
        return t+u;
    }
    // 另一个例子:
    int& foo(int& i);
    double foo(double& d);
    template<typename T>
    // auto func(T& val)  c++14才支持，c++11要返回值类型后置
    // error: 'auto' return without trailing return type; deduced return types are a C++14 extension
    auto func(T& val) -> decltype(foo(val))
    {
        return foo(val);
    }
}

// 4、using定义别名（替代typedef）
{
    // using 的别名语法覆盖了 typedef 的全部功能
    {
        // 重定义unsigned int
        typedef unsigned int uint_t;
        using uint_t = unsigned int;
        // 重定义std::map
        typedef std::map<std::string, int> map_int_t;
        using map_int_t = std::map<std::string, int>;
    }
    // 函数也是
    {
        typedef void(*func_t)(int,int);
        using func_t = void(*)(int,int);
    }
    // 模板
    {
        /* C++98/03 */
        template <typename T>
        struct func_t
        {
            typedef void (*type)(T, T);
        };

        func_t<int>::type xx_1;
        /* C++11 */
        template <typename T>
        using func_t = void (*)(T, T);
        func_t<int> xx_2;
    }
}

// 5、模板的默认模板参数
{
    // error in C++98/03: default template arguments
    // C++11支持默认模板参数
    template <typename R = int, typename U>
    R func(U val)
    {
        return val;
    }
    int main()
    {
        func(97);               // R=int, U=int
        func<char>(97);         // R=char, U=int
        func<double, int>(97);  // R=double, U=int
        return 0;
    }
}

// 6、列表初始化
{
    class Foo
    {
    public:
        Foo(int) {}
    private:
        Foo(const Foo &);
    };
    int main(void)
    {
        Foo a1(123);
        Foo a2 = 123;  //error: 'Foo::Foo(const Foo &)' is private
        Foo a3 = { 123 };   //同a1
        Foo a4 { 123 };     //同a1

        int a5 = { 3 };
        int a6 { 3 };
        return 0;
    }
    
}


// 7、lambda匿名函数
{
    // 语法格式：
    //     [外部变量访问方式说明符] (参数) mutable noexcept/throw() -> 返回值类型
    //     {
    //     函数体;
    //     };
        // 表 1 [外部变量]的定义方式
        // 外部变量格式	功能
        // []	空方括号表示当前 lambda 匿名函数中不导入任何外部变量。
        // [=]	只有一个 = 等号，表示以值传递的方式导入所有外部变量；
        // [&]	只有一个 & 符号，表示以引用传递的方式导入所有外部变量；
        // [val1,val2,...]	表示以值传递的方式导入 val1、val2 等指定的外部变量，同时多个变量之间没有先后次序；
        // [&val1,&val2,...]	表示以引用传递的方式导入 val1、val2等指定的外部变量，多个变量之间没有前后次序；
        // [val,&val2,...]	以上 2 种方式还可以混合使用，变量之间没有前后次序。
        // [=,&val1,...]	表示除 val1 以引用传递的方式导入外，其它外部变量都以值传递的方式导入。
        // [this]	表示以值传递的方式导入当前的 this 指针。

        //  (参数) 接收外部传递的多个参数，可以省略()

        // 默认情况下，对于以值传递方式引入的外部变量
        //如果想修改它们，就必须使用 mutable 关键字。
        {
            int num[4] = {1, 3, 2, 4};
            sort(num, num+4, [=](int x, int y)->bool{return x < y;});
            for(int n : num)
                cout<<n<<" ";
        }
        // 给匿名函数设置别名
        auto show = [=](int a, int b) -> void{ cout<<a<<" "<<b<<endl; };
        show(1,2);

        // 访问权限
        {
            static int all_num = 5; //全局变量
            int num_1 = 1;          //局部变量
            auto lambda1 = [=]{  //值传递
                all_num = 6;    //全局变量可以访问甚至修改

                // num_1 = 2;      //会报错：函数体内只能使用外部变量，而无法对它们进行修改

                // lambda1 = [=]{改成lambda1 = [&]{                 可以num_1 = 2
                // lambda1 = [=]{改成lambda1 = [=] () mutable {     可以num_1 = 2
                
            };
            lambda1();
        }
        // 异常
        {
            auto except1 = []()noexcept     { throw 100; }; //except1 匿名函数指定了函数体中不发生任何异常，但函数体中却发生了整形异常
            auto except2 = []()throw(char)  { throw 10;  }; //except2 匿名函数指定函数体可能会发生字符异常，但函数体中却发生了整形异常
            try{
                except1();
                except2();
            }catch(int){
                cout << "捕获到了整形异常"<< endl;
            }
            // 由于指定异常类型和真正发生的异常类型不匹配，导致 try-catch 无法捕获，最终程序运行崩溃。
        }
}

// 8、for循环（基于范围的循环）
{
    for(char c : "hello")
        cout<<c<<" ";
    for(int i : {1,2,3,4,5})
        cout<<i<<",";
}

// 9、constexpr：验证是否为常量表达式
// 以定义数组为例，数组的长度就必须是一个常量表达式
{
    int url[10];//正确
    int length = 10;
    int url[length];//错误，length是变量
    constexpr int num = 1 + 2 + 3;
    int url[num] = {1,2,3,4,5,6};   //OK
    // constexpr修饰函数，函数返回值为参量
}

// 10、右值引用
{
    // C++11 标准新引入了另一种引用方式，称为右值引用，用 "&&" 表示
    {
        // 无名右值引用:
        //     static_cast<T&&>(t)
        //     std::move()
        // 具名右值引用:
        //     T&&
    }
    // ============C++左值引用和右值引用:
    // 引用类型	    可以引用的值类型	                        使用场景
    //              非常量左值	常量左值	非常量右值	常量右值
    // 非常量左值引用	Y	        N	        N	        N	无
    // 常量左值引用	    Y	        Y	        Y	        Y	常用于类中构建拷贝构造函数
    // 非常量右值引用	N	        N	        Y	        N	移动语义、完美转发
    // 常量右值引用	    N	        N	        Y	        Y	无实际用途

    {
        struct X {}; 
        X a; 
        X&& b = static_cast<X&&>(a);    //static_cast<X&&>(a)是“无名右值引用“，是右值 
        X&& c = std::move(a);           //std::move(a)是“无名右值引用“，是右值
        // b 和 c 是“具名右值引用”，是左值 

        X& d = a; 
        X& e = b; 
        //左值引用d和e只能绑定左值（包括传统左值：变量a以及新型左值：右值引用b） 
        const X& f = c; 
        const X& g = X(); 
        X&& h = X(); 
        //const左值引用f和g可以绑定左值（右值引用c），也可以绑定右值（临时对象X()） 
        
        //右值引用b，c和h只能绑定右值（包括新型右值：无名右值引用std::move(a)以及传统右值：临时对象X()） 
    }
}

// 11、移动构造函数 （右值引用相关）
// 右值引用主要用于实现移动（move）语义和完美转发
{
    class demo{
    public:
        demo():num(new int(0)){
            cout<<"construct!"<<endl;
        }
        //拷贝构造函数
        demo(const demo &d):num(new int(*d.num)){
            cout<<"copy construct!"<<endl;
        }
        ~demo(){
            cout<<"class destruct!"<<endl;
        }
    private:
        int *num;
    };
    demo get_demo(){
        return demo();
    }
    int main(){
        demo a = get_demo();
        return 0;
    }
    // 编译器默认有优化拷贝临时对象的拷贝构造。
    // 执行结果：
        // construct!
        // class destruct! 
    // 可以C++变异的时候指定-fno-elide-constructors，即不忽略构造。
    // 执行结果：
        // construct!            <-- 执行 demo()
        // copy construct!              <-- 执行 return demo()拷贝构造
        // class destruct!       <-- 销毁 demo() 产生的匿名对象
        // copy construct!                  <-- 执行 a = get_demo()拷贝构造
        // class destruct!              <-- 销毁 get_demo() 返回的临时对象
        // class destruct!                  <-- 销毁 a

    // C++11移动语义是什么
    // 拷贝构造（深拷贝）如果申请了大量的堆空间影响效率，引入了右值引用的语法，借助它可以实现移动语义。
    // 所谓移动语义，指的就是以移动而非深拷贝的方式初始化含有指针成员的类对象。
    // 简单的理解，移动语义指的就是将其他对象（通常是临时对象）拥有的内存资源“移为已用”。

    // 临时对象，往往只用于传递数据（没有其它的用处），并且会很快会被销毁。
    // 因此在使用临时对象初始化新对象时，我们可以将其包含的指针成员指向的内存资源直接移给新对象所有，无需再新拷贝一份，这大大提高了初始化的执行效率。
    // 添加：
    //添加移动构造函数
    demo(demo &&d):num(d.num){
        d.num = NULL;
        cout<<"move construct!"<<endl;
    }
    // 此构造函数使用右值引用形式的参数，又称为移动构造函数。
    // 并且在此构造函数中，num 指针变量采用的是浅拷贝的复制方式，同时在函数内部重置了 d.num，有效避免了“同一块对空间被释放多次”情况的发生。
    // 执行结果：
        // construct!
        // move construct!
        // class destruct!
        // move construct!
        // class destruct!
        // class destruct!

    // 非 const 右值引用只能操作右值，程序执行结果中产生的临时对象（例如函数返回值、lambda 表达式等）既无名称也无法获取其存储地址，所以属于右值。
    // 当类中同时包含拷贝构造函数和移动构造函数时，如果使用临时对象初始化当前类的对象，编译器会优先调用移动构造函数来完成此操作。
    // 只有当类中没有合适的移动构造函数时，编译器才会退而求其次，调用拷贝构造函数。

    // 在实际开发中，通常在类中自定义移动构造函数的同时，会再为其自定义一个适当的拷贝构造函数，
    // 由此当用户利用右值初始化类对象时，会调用移动构造函数；
    // 使用左值（非右值）初始化类对象时，会调用拷贝构造函数。

    // C++11 标准中为了满足用户使用左值初始化同类对象时也通过移动构造函数完成的需求，
    // 新引入了 std::move() 函数，它可以将左值强制转换成对应的右值，由此便可以使用移动构造函数。
}

// 12、move()函数：将左值强制转换为右值（实现移动语义）
{
    class movedemo{
    public:
        movedemo():num(new int(0)){
            cout<<"construct!"<<endl;
        }
        //拷贝构造函数
        movedemo(const movedemo &d):num(new int(*d.num)){
            cout<<"copy construct!"<<endl;
        }
        //移动构造函数
        movedemo(movedemo &&d):num(d.num){
            d.num = NULL;
            cout<<"move construct!"<<endl;
        }
    public:     //这里应该是 private，使用 public 是为了更方便说明问题
        int *num;
    };
    int main(){
        movedemo demo;
        cout << "demo2:\n";
        movedemo demo2 = demo;
        //cout << *demo2.num << endl;   //可以执行
        cout << "demo3:\n";
        movedemo demo3 = std::move(demo);
        //此时 demo.num = NULL，因此下面代码会报运行时错误
        //cout << *demo.num << endl;
        return 0;
    }
}

// 13、C++11完美转发
{
    // 完美转发，它指的是函数模板可以将自己的参数“完美”地转发给内部调用的其它函数。
    // 所谓完美，即不仅能准确地转发参数的值，还能保证被转发参数的左、右值属性不变
    
    //重载被调用函数，查看完美转发的效果
    void other_func(int & t) {
        cout << "lvalue\n";
    }
    void other_func(const int & t) {
        cout << "rvalue\n";
    }
    //实现完美转发的函数模板
    template <typename T>
    void function(T&& t) {
        other_func(forward<T>(t));
    }
    int main()
    {
        function(5);
        int  x = 1;
        function(x);
        return 0;
    }
    // 结果输出：
        // rvalue
        // lvalue
    // 在实现完美转发时，只要函数模板的参数类型为 T&&，则 C++ 可以自行准确地判定出实际传入的实参是左值还是右值。
    // forword() 函数模板用于修饰被调用函数中需要维持参数左、右值属性的参数
    // C++98/03就需要2个函数：
        {
            //重载函数模板，分别接收左值和右值
            //接收右值参数
            template <typename T>
            void function(const T& t) {
                other_func(t);
            }
            //接收左值参数
            template <typename T>
            void function(T& t) {
                other_func(t);
            }
        }
}
// 14、nullptr：初始化空指针
{
    // C++ 中将 NULL 定义为字面常量 0
    void isnull(void *c){cout << "void* c" << endl;}
    void isnull(int n){cout << "int n" << endl;}
    isnull(NULL);   //会调用int n，而不是void*c
    // 需要对 NULL（或者 0）进行强制类型转换才会调用void* c
    isnull( (void*)NULL ); 
    isnull( (void*)0 );

    // nullptr 是 nullptr_t 类型的右值常量，专用于初始化空类型指针
}
// 15、shared_ptr智能指针
{
    // C++ 智能指针底层是采用引用计数的方式实现的
    // 多个 shared_ptr 智能指针可以共同使用同一块堆内存。
    // 并且，由于该类型智能指针在实现上采用的是引用计数机制，即便有一个 shared_ptr 指针放弃了堆内存的“使用权”（引用计数减 1），
    // 也不会影响其他指向同一堆内存的 shared_ptr 指针（只有引用计数为 0 时，堆内存才会被自动释放）。
    // 空智能指针：
    std::shared_ptr<int> p1;             //不传入任何实参
    std::shared_ptr<int> p2(nullptr);    //传入空指针 nullptr
    // 智能指针：
    std::shared_ptr<int> p3(new int(10));
    // std::make_shared<T> 模板函数，其可以用于初始化 shared_ptr 智能指针
    std::shared_ptr<int> p3 = std::make_shared<int>(10);
    //调用拷贝构造函数
    std::shared_ptr<int> p4(p3);//或者 std::shared_ptr<int> p4 = p3;
    //调用移动构造函数
    std::shared_ptr<int> p5(std::move(p4)); //或者 std::shared_ptr<int> p5 = std::move(p4);
    // shared_ptr<T>模板类常用成员方法：
    reset() //当函数没有实参时，该函数会使当前 shared_ptr 所指堆内存的引用计数减 1，同时将当前对象重置为一个空指针；
            //当为函数传递一个新申请的堆内存时，则调用该函数的 shared_ptr 对象会获得该存储空间的所有权，并且引用计数的初始值为 1
    use_count() //返回同当前 shared_ptr 对象（包括它）指向相同的所有 shared_ptr 对象的数量
    unique()    //判断当前 shared_ptr 对象指向的堆内存，是否不再有其它 shared_ptr 对象再指向它。
}
// 16、unique_ptr智能指针
{
    // 每个 unique_ptr 指针都独自拥有对其所指堆内存空间的所有权。
    // unique_ptr 和 shared_ptr 指针最大的不同之处在于，unique_ptr 指针指向的堆内存无法同其它 unique_ptr 共享
    // 1) 通过以下  2 种方式，可以创建出空的 unique_ptr 指针：
    std::unique_ptr<int> p1();
    std::unique_ptr<int> p2(nullptr);
    // 2) 创建 unique_ptr 指针的同时，也可以明确其指向。例如：
    std::unique_ptr<int> p3(new int);
    // 只提供了移动构造函数。例如：
    std::unique_ptr<int> p4(p3);            //错误，堆内存不共享
    std::unique_ptr<int> p4(std::move(p3)); //正确，调用移动构造函数
                // 对于调用移动构造函数的 p3 和 p4 来说，p4 将获取 p3 所指堆空间的所有权，而 p3 将变成空指针（nullptr）。
}
// 17、weak_ptr智能指针
{
    // 通常不单独使用（没有实际用处），只能和 shared_ptr 类型指针搭配使用
    // weak_ptr 类型指针视为 shared_ptr 指针的一种辅助工具，借助 weak_ptr 类型指针， 我们可以获取 shared_ptr 指针的一些状态信息
    // weak_ptr 类型指针并不会影响所指堆内存空间的引用计数（指向和释放都不会影响shared_ptr的计数）
    std::shared_ptr<int> sp1(new int(10));
    std::shared_ptr<int> sp2(sp1);
    std::weak_ptr<int> wp(sp2);
    
    cout << wp.use_count() << endl;//输出和 wp 同指向的 shared_ptr 类型指针的数量
    sp2.reset();        //释放 sp2
    cout << wp.use_count() << endl;
    
    cout << *(wp.lock()) << endl;//借助 lock() 函数，返回一个和 wp 同指向的 shared_ptr 类型指针，获取其存储的数据

    // 程序执行结果为：
    //     2
    //     1
    //     10
}





