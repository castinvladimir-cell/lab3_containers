#include <iostream>
#include <type_traits>
#include "MyVector.h"
#include "MyList.h"
#include "MyDLList.h"

template <class T>
void print(T& cont) { // вывод через итератор
    for (auto it = cont.begin(); it != cont.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

template <class T>
std::remove_reference_t<T>&& my_move(T&& x) noexcept { // свой std::move
    return static_cast<std::remove_reference_t<T>&&>(x);
}

template <class T>
void test(T& a) {
    for (int i = 0; i < 10; i++) {
        a.push_back(i);
    }
    print(a);
    std::cout << " Размер " << a.size() << std::endl;

    // удаляю 3, 5 и 7 элемент. с конца чтоб индексы не съехали
    a.erase(6);
    a.erase(4);
    a.erase(2);
    std::cout << " Удаление " << std::endl;
    print(a);

    a.insert(10, 0);
    std::cout << " В начало " << std::endl;
    print(a);

    a.insert(20, a.size() / 2);
    std::cout << " В середину " << std::endl;
    print(a);

    a.push_back(30);
    std::cout << " В конец " << std::endl;
    print(a);

    // доп проверки
    std::cout << " a[4] = " << a[4] << std::endl;
    a[4] = 21;
    print(a);

    T b(a); // копия
    b.push_back(99);
    std::cout << " Копия " << std::endl;
    print(b);
    print(a);

    T c;
    c = a;
    std::cout << " c = a " << std::endl;
    print(c);

    T m(my_move(b)); // перемещение, b после этого пустой
    std::cout << " Перемещение " << std::endl;
    print(m);
    std::cout << " Размер b " << b.size() << std::endl;

    c = my_move(m);
    std::cout << " c = my_move(m) " << std::endl;
    print(c);
    std::cout << " Размер m " << m.size() << std::endl;

    try {
        a.erase(100);
    } catch (std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }
}

int main()
{
    MyVector<int> v;
    std::cout << "----Вектор-----" << std::endl;
    test(v);

    MyList<int> l;
    std::cout << "----Список----" << std::endl;
    test(l);

    MyDLList<int> dl;
    std::cout << "------ДлСписок-----" << std::endl;
    test(dl);
}