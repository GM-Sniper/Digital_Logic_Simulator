#include "Gates.h"

using namespace std;
// Constructor
Gates::Gates(const string& name, int numInputs, const string& outputExpression, int delayPs)
    : name(gateName), numInputs(numOfInputs), outputExpression(outputExpression), delayPs(timeDelay) {}

// Getters
string Gates:: getGateName() const { return gateName; }
int Gates::getNumOfInputs() const { return numOfInputs; }
string Gates::getOutputExpression() const { return outputExpression; }
int Gates::getDelayTime() const { return timeDelay; }

// Setters
void Gates::setGateName(const string& newName) { gateName = newName; }
void Gates::setNumOfInputs(int newNumInputs) { numOfInputs = newNumInputs; }
void Gates::setOutputExpression(const string& newOutputExpression) { outputExpression = newOutputExpression; }
void Gates::setDelayTime(int newDelayPs) { delayTime = newDelayPs; }
