# 基于c++ 11 实现的线程池
包括
**lamda表达式**

```
[外部变量访问方式说明符](参数) mutable nonexcept/throw()->返回值类型
{
    函数体
};
```

**move()**
将某个左值强制转化为右值
左值引用与右值引用
```
int a = 10;
int &b = a; // 定义一个左值引用变量
b = 20;   // 利用左值引用改变引用内存的值
```
左值引用在汇编层面与普通指针相同，定义引用变量必循初始化，需要告诉编译器是谁的引用
```
int &var = 10;
```
上述代码无法编译通过，因为无法对立即数10取地址，内存中不存储10。
```
const int &var = 10;
```
可以使用常引用来引用常量10，因为此刻相当于内存中产生了临时变量10，可以取地址操作，相当于下述操作
```
const int temp = 10;
const int &var = temp;
```
因此左值引用必须右边的值能取地址。如无法取地址，可用常引用。但常引用无法修改值。


广泛认同的说法：
可以取地址，有名字的，非临时的为左值
不能取地址，没有名字的，临时的为右值
**立即数，函数返回的值都是右值。非匿名对象，函数返回的引用，const对象等都是左值**

从本质上理解，就是程序员只保证本行有效的，就是右值。而用户创建的，通过作用规则可知其生存周期的就是左值。

右值引用用法
```
类型 &&引用名 = 右值表达式
```
**右值引用的作用**：充分利用右值的临时对象的构造来减少对象的构造和析构操作达到高效率

**unique_lock** 

使得在作用域范围内始终持有锁变量
**std::fuction<>**
函数指针，可以延迟函数的执行

**std::bind**
将可调用对象与其参数一起绑定，绑定后的结果可以使用std::function保存
**std::forward**
将参数进行绑定
