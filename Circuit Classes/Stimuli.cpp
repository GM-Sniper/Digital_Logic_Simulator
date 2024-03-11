#include "Stimuli.h"

// Constructor
Stimuli::Stimuli(int timeStamp, char input, bool logicValue)
    : timeStamp(timeStamp), input(input), logicValue(logicValue) {}

// Getters
int Stimuli::getTimeStamp() const {
    return timeStamp;
}

char Stimuli::getInput() const {
    return input;
}

bool Stimuli::getLogicValue() const {
    return logicValue;
}

// Setters
void Stimuli::setTimeStamp(int timeStamp) {
    this->timeStamp = timeStamp;
}

void Stimuli::setInput(char input) {
    this->input = input;
}

void Stimuli::setLogicValue(bool logicValue) {
    this->logicValue = logicValue;
}
