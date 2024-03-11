#ifndef GATES_H
#define GATES_H

#include <string>
using namespace std;

class Gates {
private:
    string gateName;
    int numOfInputs;
    string outputExpression;
    int timeDelay; // time is in picoseconds (x10^-12)

public:
    // Constructor
    Gates(string& name, int numInputs, string& outputExpression, int delayPs);

    // Getters
    string getGateName() const;
    int getNumOfInputs() const;
    string getOutputExpression() const;
    int getDelayTime() const;

    // Setters
    void setGateName(const string& newName);
    void setNumOfInputs(int newNumInputs);
    void setOutputExpression(const string& newOutputExpression);
    void setDelayTime(int newDelayPs);
};
#include "Gates.cpp"
#endif // GATES_H

