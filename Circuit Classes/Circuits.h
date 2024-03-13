#ifndef CIRCUIT_COMPONENT_H
#define CIRCUIT_COMPONENT_H

#include <string>
#include <vector>
#include <memory>
#include "Gates.h"

// Forward declaration of Gates class
class Stimuli;


using namespace std;

class CircuitComponent {
public:
    CircuitComponent(const string& name, const string& type, const string& output, const vector<string>& inputs);

    // Getters
    string getName() const;
    const string& getType() const;
    string getOutput() const;
    vector<string> getInputs() const;

    // Setters
    void setName(const string& newName);
    void setType(const string& newType);
    void setOutput(const string& newOutput);
    void setInputs(const vector<string>& newInputs);

private:
    string name;
    string type;
    string output;
    vector<string> inputs;
};
#include "Circuits.cpp"
#endif // CIRCUIT_COMPONENT_H
