#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream inputFile("Tests/libFile.lib"); // Open file for reading
    if (!inputFile) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) { // Read each line from the file
        cout << "Line: " << line << endl; // Output the line
    }

    inputFile.close(); // Close the file

    return 0;
}
