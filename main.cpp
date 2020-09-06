#include <iostream>

#include "pstl.cpp"

/*-----------------------------------------------------------------------------
The main function.
-----------------------------------------------------------------------------*/
int main(void)
{
    // Polynomial a("a", {5.1, -6.6, 0, 1.19, -92.33, 1.4});
    Polynomial a("a", {3, -6, 1});
    Polynomial b("b", {2, -7});
    Polynomial c = a + b * a;
    Polynomial d;
    a.print();
    b.print();
    c.print();
    d.print();

    return 0;
}

