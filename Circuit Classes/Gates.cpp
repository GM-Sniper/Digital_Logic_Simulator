#include "Gates.h"

// Constructor
Gates::Gates(string& name,int numInputs, string& outputExpression, int delayPs)
    : gateName(name), numOfInputs(numInputs), outputExpression(outputExpression), timeDelay(delayPs) {}
Gates::Gates(){}
// Getters
string Gates::getGateName() const { return gateName; }
int Gates::getNumOfInputs() const { return numOfInputs; }
string Gates::getOutputExpression() const { return outputExpression; }
int Gates::getDelayTime() const { return timeDelay; }

// Setters
void Gates::setGateName(const string& newName) { gateName = newName; }
void Gates::setNumOfInputs(int newNumInputs) { numOfInputs = newNumInputs; }
void Gates::setOutputExpression(const string& newOutputExpression) { outputExpression = newOutputExpression; }
void Gates::setDelayTime(int newDelayPs) { timeDelay = newDelayPs; }
