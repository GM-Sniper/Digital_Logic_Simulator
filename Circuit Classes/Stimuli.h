#ifndef STIMULI_H
#define STIMULI_H

#include <string>

using namespace std;

class Stimuli {
public:
    Stimuli(int timeStamp, string input, bool logicValue);
    
    // Getters
    int getTimeStamp() const;
    string getInput() const;
    bool getLogicValue() const;

    // Setters
    void setTimeStamp(int timeStamp);
    void setInput(string inputs);
    void setLogicValue(bool logicValue);

    //Overloading operations
    bool operator==(const Stimuli& other) const
    {
        return input== other.input;
    }

private:
    int timeStamp;   // Time stamp in picoseconds
    string input;      // Input identifier
    bool logicValue; // Logic value (0 or 1)
    
};

#include "Stimuli.cpp"
#endif // STIMULI_H
