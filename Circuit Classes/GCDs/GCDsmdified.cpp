#include <iostream>
#include <vector>

using namespace std;

int findGCD(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int findGCD(const vector<int>& elements) {
    int result = elements[0];
    for (int i = 1; i < elements.size(); i++) {
        result = findGCD(result, elements[i]);
    }
    return result;
}

int main() {
    vector<int> numbers = {10, 6, 15};

    int gcd = findGCD(numbers);

    cout << "Greatest Common Divisor: " << gcd << endl;

    return 0;
}
