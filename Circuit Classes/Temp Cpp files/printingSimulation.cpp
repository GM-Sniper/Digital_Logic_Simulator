#include <iostream>
#include <vector>
using namespace std;

string dashedPattern(int value) {
    if (value == 1) {
        return "-------------------";  
    } else if (value == 0) {
        return "___________________________";  
    } else {
        return "Invalid input";
    }
}

int main() {
    vector<int> inputValues = {1, 0, 1, 0, 1};
    
    cout << "Input values: ";
    for (int value : inputValues) {
        cout << value << " ";
    }

    cout << "\nDashed pattern: ";
    for (int value : inputValues) 
        {
        cout << dashedPattern(value) << "";

        
        }
    return 0;
}
