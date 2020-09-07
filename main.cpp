#include <iostream>

#include "pstl.cpp"

/*-----------------------------------------------------------------------------
The main function.
-----------------------------------------------------------------------------*/
int main(void)
{
    Polynomial a({6.53, 1.57, 9.21, 0, 43.42, 16.72, 9.91, 0, 0, 1.16}, "a");
    Polynomial b({88.42, 1.97, 0, 6.53, 23.21, 6.666, 1.94}, "b");
    Polynomial c = a - b;
    Polynomial d = a + b;
    Polynomial e = a / 3.32;
    Polynomial f = b / 6.59;
    Polynomial g = a * b;
    a.print();
    b.print();
    c.print();
    d.print();
    e.print();
    f.print();
    g.print();

    return 0;
}

