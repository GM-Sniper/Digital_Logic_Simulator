#include "Circuits.h"
#include "Gates.h"
#include "Stimuli.h"
using namespace std;

// Define the constructor
CircuitComponent::CircuitComponent(const string& name, const Gates& type, const string& output, const vector<Stimuli>& inputs)
    : name(name), type(type), output(output), inputs(inputs) {}

// Define the getters
string CircuitComponent::getName() const {
    return name;
}

const Gates& CircuitComponent::getType() const {
    return type;
}

string CircuitComponent::getOutput() const {
    return output;
}

vector<Stimuli> CircuitComponent::getInputs() const {
    return inputs;
}

// Define the setters
void CircuitComponent::setName(const string& newName) {
    name = newName;
}

void CircuitComponent::setType(const Gates& newType) {
    type = newType;
}

void CircuitComponent::setOutput(const string& newOutput) {
    output = newOutput;
}

void CircuitComponent::setInputs(const vector<Stimuli>& newInputs) {
    inputs = newInputs;
}
