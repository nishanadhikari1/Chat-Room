#include <iostream>
#include <stdexcept>

using namespace std;

template <class T>
class Stack
{
private:
    T st[100];
    int top;

public:
    Stack();
    void push(T var);
    T pop();
};

template <class T>
Stack<T>::Stack()
{
    top = -1;
}

template <class T>
void Stack<T>::push(T var)
{
    if (top >= 99)
    {
        throw overflow_error("Stack overflow!");
    }
    st[++top] = var;
}

template <class T>
T Stack<T>::pop()
{
    if (top < 0)
    {
        throw underflow_error("Stack underflow!");
    }
    return st[top--];
}

int main()
{
    Stack<float> s1;
    try
    {
        s1.push(111.1F);
        s1.push(222.2F);
        s1.push(333.3F);

        cout << "1 : " << s1.pop() << endl;
        cout << "2 : " << s1.pop() << endl;
        cout << "3 : " << s1.pop() << endl;
    }
    catch (const exception &e)
    {
        cerr << "Exception: " << e.what() << endl;
    }

    Stack<long> s2;
    try
    {
        s2.push(123123123L);
        s2.push(234234234L);
        s2.push(345345345L);

        cout << "1 : " << s2.pop() << endl;
        cout << "2 : " << s2.pop() << endl;
        cout << "3 : " << s2.pop() << endl;
        cout << "4 : " << s2.pop() << endl;
    }
    catch (const exception &e)
    {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
