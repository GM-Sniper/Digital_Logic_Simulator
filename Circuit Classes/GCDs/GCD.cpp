#include<iostream>

using namespace std;

int main()
{
    int A, B, C, R, GCD;

    A = 10;
    B = 6;
    C = 15;

    R = A % B;

    while (R != 0)
    {
        A = B;
        B = R;
        R = A % B;
    }

    R = B % C;

    while (R != 0)
    {
        B = C;
        C = R;
        R = B % C;
    }

    GCD = C;

    cout << "Greatest Common Divisor: " << GCD;

    return 0;
}
