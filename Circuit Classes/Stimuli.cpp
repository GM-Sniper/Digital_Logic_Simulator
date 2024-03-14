#include "Stimuli.h"
using namespace std;

// Constructor
Stimuli::Stimuli(int timeStamp, string inputs, bool logicValue)
    : timeStamp(timeStamp), input(inputs), logicValue(logicValue) {}

// Getters
int Stimuli::getTimeStamp() const {
    return timeStamp;
}

string Stimuli::getInput() const {
    return input;
}

bool Stimuli::getLogicValue() const {
    return logicValue;
}

// Setters
void Stimuli::setTimeStamp(int timeStamp) {
    this->timeStamp = timeStamp;
}

void Stimuli::setInput(string input) {
    this->input = input;
}

void Stimuli::setLogicValue(bool logicValue) {
    this->logicValue = logicValue;
}
