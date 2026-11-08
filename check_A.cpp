#include "MyVector.h"
#include "MyList.h"
#include "MyDLList.h"

struct A
{
    A(int c) : c_(c) {}
    int c_;
private:
    A() = default;
};

int main()
{
    MyList<A> l;
    l.push_back(A(1));
    l.insert(A(2), 0);

    MyDLList<A> d;
    d.push_back(A(1));
    d.insert(A(2), 0);

    MyVector<A> v;
    v.push_back(A(1));
}
