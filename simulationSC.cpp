#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Circuit Classes/Gates.h"

using namespace std;

vector<Gates> parseLibraryFile(const string& filename) {
    vector<Gates> components;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return components;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string name, outputExpression;
        int numInputs, delayPs;
        char comma;
         // To read the comma
        if (!(iss >> name >> numInputs >> comma >> outputExpression >> delayPs)) {
            cerr << "Error parsing line: " << line << endl;
            continue;
        }
        name.erase(name.length()-1,1);
        outputExpression.erase(outputExpression.length()-1,1);
        
        
        components.push_back({name, numInputs, outputExpression, delayPs});
    }
    return components;
}


int main() {
    vector<Gates> components = parseLibraryFile("Tests/libFile.lib");
    for(const auto& component : components) {
        cout << "Component: " << component.getGateName() << " Num Inputs: " << component.getNumOfInputs()
                  << " Output Expression: " << component.getOutputExpression() << " Delay (ps): " << component.getDelayTime() << endl;
    }
}
