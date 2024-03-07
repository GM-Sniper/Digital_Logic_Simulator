#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream inputFile("Tests/testCircuit1.cir"); // Open file for reading
    if (!inputFile) {
        cout << "Error opening file" << endl;
    
        return 1;
    }

    string line;
    while (getline(inputFile, line)) { // Read each line from the file
        cout << line << endl; // Output the line
    }

    inputFile.close(); // Close the file

    return 0;
}
