#ifndef GATES_H
#define GATES_H

#include <string>
using namespace std;

class Gates {
private:
    string gateName=" ";
    int numOfInputs=0;
    string outputExpression=" ";
    int timeDelay=0; // time is in picoseconds (x10^-12)

public:
    // Constructor
    Gates(string& name, int numInputs, string& outputExpression, int delayPs);
    Gates();
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

    //function in gate that determines the type of gate array of inputs-> splitting to split the inputs and outputss, 
};
#include "Gates.cpp"
#endif // GATES_H

