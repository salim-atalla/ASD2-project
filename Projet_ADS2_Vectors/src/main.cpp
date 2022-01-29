#include "fint.h"
#include <iostream>

using namespace std;






int main()
{


    fint f1 (90);
    fint f2 (5);

    unsigned int n = 2;

    cout << "f1 = " << f1 << " = " << f1.to_int() << endl;
    cout << "f2 = " << f2 << " = " << f2.to_int() << endl;



    fint f3 = f1 / f2;

    cout << "a = " << f3 << " = " << f3.to_int() << endl;

    return 0;
}