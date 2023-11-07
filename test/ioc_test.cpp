#include "common/IocContainer2.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;

struct ICar {
    virtual ~ICar() {}
    virtual void test() const {}
};

struct Car {
    void test() const { std::cout << "Car test" << std::endl; }
};

struct Bus {
    void test() const { std::cout << "Bus test" << std::endl; }
};

struct Base {
    virtual void func() {}
    virtual ~Base() {}
};

struct DerivedB : Base {
    virtual void func() { std::cout << "derived b" << std::endl; }
};

struct DerivedA : Base {
    virtual void func() { std::cout << "derived a" << std::endl; }
};

struct A {
    A(Base *ptr) : m_ptr(ptr) {}

    virtual void func() { m_ptr->func(); }

  private:
    Base *m_ptr;
};

int main()
{
    IocContainer ioc;
    ioc.registerType<A, DerivedA>("A");
    ioc.registerType<A, DerivedB>("B");

    auto pa = ioc.resolveShared<A>("A");
    auto pb = ioc.resolveShared<A>("B");

    pa->func();
    pb->func();
    cout << "-------------------" << endl;

    ioc.registerType<Bus>("bus");
    ioc.registerType<Car>("car");
    auto pbus = ioc.resolveShared<Bus>("bus");
    auto pcar = ioc.resolveShared<Car>("car");
    pbus->test();
    pcar->test();

    return 0;
}
