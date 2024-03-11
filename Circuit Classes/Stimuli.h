#ifndef STIMULI_H
#define STIMULI_H

#include <string>

class Stimuli {
public:
    Stimuli(int timeStamp, char input, bool logicValue);
    
    // Getters
    int getTimeStamp() const;
    char getInput() const;
    bool getLogicValue() const;

    // Setters
    void setTimeStamp(int timeStamp);
    void setInput(char input);
    void setLogicValue(bool logicValue);

private:
    int timeStamp;   // Time stamp in picoseconds
    char input;      // Input identifier
    bool logicValue; // Logic value (0 or 1)
};


#endif // STIMULI_H
