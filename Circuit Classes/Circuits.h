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
    CircuitComponent(const string& name, const Gates& type, const string& output, const vector<Stimuli>& inputs);

    // Getters
    string getName() const;
    const Gates& getType() const;
    string getOutput() const;
    vector<Stimuli> getInputs() const;

    // Setters
    void setName(const string& newName);
    void setType(const Gates& newType);
    void setOutput(const string& newOutput);
    void setInputs(const vector<Stimuli>& newInputs);

private:
    string name;
    Gates type;
    string output;
    vector<Stimuli> inputs;
};
#include "Circuits.cpp"
#endif // CIRCUIT_COMPONENT_H
