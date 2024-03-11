#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Circuit Classes/Circuits.h"
#include "Circuit Classes/Stimuli.h"
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

vector<CircuitComponent> parseCircuitFile(const string& filename) {
    vector<CircuitComponent> components;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return components;
    }

    string line;
    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        // Parse INPUTS section
        if (line == "INPUTS:") {
            while (getline(file, line) && line != "COMPONENTS:") {
                // Parse input signals
                // Example: A, B, C
                // for example after it reads inputs, it goes to the next line and takes A and adds the stimuli for it w kda
                istringstream iss(line);
                string input;
                while (getline(iss, input, ',')) {
                    // Create Stimuli object for each input and add it to a vector
                    // This means for example if the input is A, to get the data parsed from the stim of that file onto it, this will
                    // require to do the stimuli parsing first and add the vector of the stimuli as a parameter.
                    // a better implememntation is to do it in the class itself but we are testing first so lets see how it goes

                
                    Stimuli stimuli(0, input[0], false); // Set initial time stamp to 0 and logic value to false
                    // You might want to validate and process the input further as needed
                }
            }
        }

        // Parse COMPONENTS section
        if (line == "COMPONENTS:") {
            while (getline(file, line)) {
                // Parse each component line
                // Example: G0, NOT, w1, C
                istringstream iss(line);
                string name, output;
                Gates type; // add a default contrusctor for Gates function
                vector<Stimuli> inputs;
                if (iss >> name >> type >> output) {
                    // Parse input signals for the component
                    string input;
                    while (iss >> input) {
                        // Create Stimuli object for each input and add it to a vector
                        Stimuli stimuli(0, input[0], false); // Set initial time stamp to 0 and logic value to false
                        // You might want to validate and process the input further as needed
                        inputs.push_back(stimuli);
                    }
                    // Create Gates object for the component type
                    Gates gate(name, 0, "", 0); // You might need to provide appropriate values here
                    // Create CircuitComponent object and add it to the vector
                    components.push_back(CircuitComponent(name, gate, output, inputs));
                }
            }
        }
    }

    return components;
}

int main() {
    vector<Gates> components = parseLibraryFile("Tests/libFile.lib");
    
   /* for(const auto& component : components) {
        cout << "Component: " << component.getGateName() << " Num Inputs: " << component.getNumOfInputs()
                  << " Output Expression: " << component.getOutputExpression() << " Delay (ps): " << component.getDelayTime() << endl;
    }*/
    
}
