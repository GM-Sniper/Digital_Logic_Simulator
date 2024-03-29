#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <stack>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include "../Circuit Classes/Stimuli.h"
#include "../Circuit Classes/Gates.h"
using namespace std;
struct wire // Struct for wires is used to instantiate wires that have common attributes like name, delay,and boolean type.
{
    string name;
    bool type;
    int delay;
    int actualdelay = 0;
    stack<int> initial;
    wire(string n, int t, int d = 0) : name(n), type(t), delay(d) {}
};

string removeSpaces(string spaces)
{
    string noSpaces;
    for (int i = 0; i < spaces.length(); i++)
    {
        if (spaces[i] != ' ')
        {
            noSpaces += spaces[i];
        }
    }
    return noSpaces;
}
string removeCommas(string commas)
{
    string noCommas;
    for (int i = 0; i < commas.length(); i++)
    {
        if (commas[i] != ',')
        {
            noCommas = noCommas + commas[i];
        }
    }
    return noCommas;
}

vector<Gates> parseLibraryFile(const string &filename)
{
    vector<Gates> components;

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        exit(10);
    }

    string line;
    int lineCount = 1;
    while (getline(file, line))
    {
        // Split the line by commas
        size_t pos = 0;
        vector<string> parts;
        while ((pos = line.find(',')) != string::npos)
        {
            parts.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        parts.push_back(line); // Add the last part

        // Validate the number of parts
        if (parts.size() != 4)
        {
            cerr << "Error: Invalid format in line " << lineCount << " in the library file" << endl;
            exit(1);
        }

        // Extract components from parts
        string name = parts[0];
        int numInputs, delayPs;
        string outputExpression;
        try
        {
            numInputs = stoi(parts[1]);
            outputExpression = parts[2];
            delayPs = stoi(parts[3]);
        }
        catch (const exception &e)
        {
            cerr << "Error parsing line: " << line << endl;
            exit(2);
        }
        if (delayPs < 0 || numInputs < 0)
        {
            cerr << "Error in numbers in the library. Number is negative in line: " << line << endl;
            exit(2);
        }
        name = removeSpaces(name);
        outputExpression = removeSpaces(outputExpression);
        name = removeCommas(name);
        outputExpression = removeCommas(outputExpression);

        components.push_back({name, numInputs, outputExpression, delayPs});
        lineCount++;
    }

    file.close();
    return components;
}

vector<Stimuli> parseStimuliFile(const string &filename) // Reads from .stim files to read the time delay used for each input
{
    vector<Stimuli> stimuli;

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(10);
        return stimuli;
    }

    string line;
    int lineCount = 1;
    while (getline(file, line))
    {
        if (line.empty() || line[0] == ' ') // Skip lines that are empty or start with a whitespace character
            continue;

        // Split the line by commas
        size_t pos = 0;
        vector<string> parts;
        while ((pos = line.find(',')) != string::npos)
        {
            parts.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        parts.push_back(line); // Add the last part

        // Validate the number of parts
        if (parts.size() != 3)
        {
            cerr << "Error: Invalid format in line: " << lineCount << endl;
            exit(3);
        }

        // Extract components from parts
        int timeStamp;
        bool logicValue;
        string input;
        try
        {
            timeStamp = stoi(parts[0]);
            input = parts[1];
            logicValue = stoi(parts[2]);
        }
        catch (const exception &e)
        {
            cerr << "Error parsing line: " << lineCount << endl;
            exit(4);
        }

        // Validate logic value
        if (logicValue != 0 && logicValue != 1)
        {
            cerr << "Error: Invalid logic value (must be 0 or 1) in line: " << lineCount << endl;
            continue; // Skip to the next line
        }
        // Add stimulus to the vector
        input = removeSpaces(input);
        if (timeStamp < 0)
        {
            cout << "Time delay can not be negative in the stimuli.\n";
            exit(150);
        }
        if (timeStamp == 0 && logicValue == 0)
            continue;
        else
            stimuli.push_back({timeStamp, input, logicValue});

        lineCount++;
    }

    file.close();
    return stimuli;
}

bool checkGates(const string &str, const vector<Gates> &gates)
{
    for (const auto &gate : gates)
    {
        if (gate.getGateName() == str)
        {
            return true;
        }
    }
    return false;
}

bool parseCircuitFile(const string &filename, vector<pair<string, vector<wire>>> &ioComponents, vector<Stimuli> &stimuli, vector<string> &inputsT, vector<Gates> &gates)
{
    string input;

    // Open the circuit file
    ifstream file(filename);
    // Check if the file is successfully opened
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return 0; // Return if the file cannot be opened
    }

    string line;
    // Read each line from the file
    while (getline(file, line))
    {
        // Skip empty lines and lines starting with whitespace
        if (line.empty() || line[0] == ' ')
            continue;

        // Parse INPUTS section
        if (line == "INPUTS:")
        {
            // Read input lines until COMPONENTS section is reached
            while (getline(file, line) && line != "COMPONENTS:")
            {
                istringstream iss(line);
                // Read comma-separated inputs
                while (getline(iss, input, ','))
                {
                    int position = -1;
                    for (int i = 0; i < stimuli.size(); i++)
                    {
                        if (input == stimuli[i].getInput())
                        {
                            position = i;
                        }
                    }
                    input = removeCommas(input);
                    input = removeSpaces(input);
                    inputsT.push_back(input);
                }
            }
        }

        // Parse COMPONENTS section
        if (line == "COMPONENTS:")
        {
            while (getline(file, line))
            {
                vector<wire> ioVector; // Vector of wires for this gate (output and inputs)
                // Parse each component line into a vector pair
                // Example: G0, NOT, w1, C
                istringstream iss(line);
                string name, output;
                string type;
                // Skip empty lines or lines starting with whitespace
                if (line.empty() || line[0] == ' ')
                    continue;
                // Extract name, type, and output from the line
                if (iss >> name >> type >> output)
                {
                    // Clean up type and output strings
                    type = removeSpaces(type);
                    type = removeCommas(type);
                    output = removeSpaces(output);
                    output = removeCommas(output);
                    name = removeSpaces(name);
                    name = removeCommas(name);

                    // Add output to the vector of wires
                    ioVector.push_back(wire(output, 0));

                    // Checks for similar inputs with differnt delays
                    vector<bool> checks(stimuli.size(), false);
                    // Read inputs for the gate
                    while (iss >> input)
                    {
                        input = removeSpaces(input);
                        input = removeCommas(input);
                        int position = -1;
                        // Check if the input is provided as stimuli
                        for (int i = 0; i < stimuli.size(); i++)
                        {
                            if (input == stimuli[i].getInput() && checks[i] == false)
                            {
                                position = i;
                                checks[i] = true;
                                break;
                            }
                        }
                        // Add the input to the vector of wires
                        if (position == -1)
                        {
                            ioVector.push_back(wire(input, 0));
                        }
                        else
                        {
                            if (stimuli[position].getTimeStamp() == 0)
                            { // Add input with timestamp if it's provided as stimuli
                                ioVector.push_back(wire(input, stimuli[position].getLogicValue(), stimuli[position].getTimeStamp()));
                                stimuli.erase(stimuli.begin() + position);
                            }
                            else
                            {
                                ioVector.push_back(wire(input, 0, stimuli[position].getTimeStamp()));
                            }
                        }
                    }
                }
                // Add the gate and its corresponding vector of wires to the ioComponents vector
                if (checkGates(type, gates))
                {
                    ioComponents.push_back(make_pair(type, ioVector));
                }
                else
                {
                    cerr << "This gate is not in the library file, please check.\n";
                    exit(15);
                    return false;
                }
            }
        }
    }
    // Close the file
    file.close();
    return true;
}

bool getWire(const vector<pair<string, vector<wire>>> &vec, const string &wireName) // a vector that returns the boolean type of the needed wire
{
    // Iterate over each pair in the vector using a const reference
    for (auto it = vec.begin(); it != vec.end(); it++)
    { // Iterate over the vector of wires for each gate
        for (int i = 0; i < it->second.size(); i++)
        { // Check if the wire name matches
            if (it->second[i].name == wireName)
            { // Return the type of the wire if found
                return it->second[i].type;
            }
        }
    }
    // Return false if wire is not found
    return 0;
}
int getDelay(const vector<pair<string, vector<wire>>> &vec, const string &wireName)
{
    // Iterate over each pair in the vector using iterators
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        // Iterate over the vector of wires for each gate
        for (int i = 0; i < it->second.size(); i++)
        {
            // Check if the wire name matches
            if (it->second[i].name == wireName)
            {
                // Return the delay if the wire is found
                return it->second[i].actualdelay;
            }
        }
    }
    // Return 0 if the wire is not found
    return 0;
}
int getmax(vector<wire> V, const vector<pair<string, vector<wire>>> &vec)
{
    int temp = 0;
    for (int i = 1; i < V.size(); i++)
    {
        if (temp < getDelay(vec, V[i].name))
        {

            temp = getDelay(vec, V[i].name);
        }
    }
    return temp;
}
int getmin(vector<wire> V, const vector<pair<string, vector<wire>>> &vec)
{
    int temp = 10000;
    for (int i = 1; i < V.size(); i++)
    {
        if (temp > getDelay(vec, V[i].name))
        {
            temp = getDelay(vec, V[i].name);
        }
    }
    return temp;
}
bool isoperator(char c)
{
    return (c == '(' || c == ')' || c == '&' || c == '~' || c == '!' || c == '|' || c == '^');
}

int precedence(char c)
{
    switch (c)
    {
    case '(':
    case ')':
        return 0;
    case '!':
    case '~':
        return 3;
    case '&':
        return 2;
    case '|':
        return 1;
    }
    return -1;
}
int extractinput(string expression, int &pos)
{
    string operand = "";
    cout << expression[pos] << endl;
    while (pos < expression.length() && !isoperator(expression[pos + 1]))
    {
        operand += expression[++pos]; // incrementing to get the first char after I
        cout << operand << endl;
    }
    return stoi(operand);
}
bool checkinputs(vector<Stimuli> stimuli, vector<pair<string, vector<wire>>> ioComponents)
{
    for (auto it = ioComponents.begin(); it != ioComponents.end(); it++)
    {
        // for (int i = 0; i < it->second.size(); i++)
        // {
        for (int j = 0; j < stimuli.size(); j++)
        {
            if (stimuli[j].getInput() == it->second[0].name)
            {
                return false;
                break;
            }
        }
        // }
    }
    return true;
}
void computinglogic2(vector<Gates> library, vector<pair<string, vector<wire>>> ioComponents, vector<Stimuli> &stimuli, vector<int> timeScale, vector<string> &cirInputs, vector<Stimuli> &F_output)
{
    string expression;
    int currenttimescale = 0;
    int scaleindex = 0;
    vector<bool> checks(stimuli.size(), false);
    if (!checkinputs(stimuli, ioComponents))
    {
        cerr<<"An input can not be an Output "<<endl;
        exit(5);
    }
    while (scaleindex <= timeScale.size())
    {
        auto it = ioComponents.begin();
        for (it = ioComponents.begin(); it != ioComponents.end(); it++)
        {
            // cout<<"jsdhfkjshdfjkshdkjfhsdkjfhksjhdfkjshjkfhskjdfhkdsjhfkjhdkjfhskdjfhskjdfhskjdfhskjhfkjs"<<it->second[1].name<<it->second[1].type<<endl;
            int position = -1;
            int j = 0;
            bool found = false;
            string Xwire;
            stack<bool> operands;
            stack<char> operators;
            for (int i = 1; i < it->second.size(); i++)
            {
                auto ix = find(cirInputs.begin(), cirInputs.end(), it->second[i].name);
                if (ix != cirInputs.end())
                    found = true;
                else
                {
                    found = false;
                    Xwire = it->second[i].name;
                    break;
                }
            }
            if (!found)
            {
                cerr << Xwire << " is not declared as an input " << endl;
                exit(12);
            }
            for (int i = 0; i < library.size(); i++)
            {
                if (it->first == library[i].getGateName())
                {
                    position = i;
                    expression = library[i].getOutputExpression();
                    cout << expression << endl;
                }
            }
            while (j < expression.length())
            {
                char c = expression[j];
                if (!isoperator(c))
                {
                    int x = extractinput(expression, j);
                    operands.push(getWire(ioComponents, it->second[x].name));
                }
                if (c == '(')
                {
                    operators.push(c);
                }
                else if (c == ')')
                {
                    while (operators.top() != '(')
                    {
                        bool b;
                        bool a = operands.top();
                        operands.pop();
                        switch (operators.top())
                        {
                        case '~':
                        case '!':
                            operands.push(!a);
                            break;
                        case '&':
                            b = operands.top();
                            operands.pop();
                            operands.push(a && b);
                            cout << operands.top();
                            break;
                        case '|':
                            b = operands.top();
                            operands.pop();
                            operands.push(a || b);
                            break;
                        }
                        operators.pop();
                    }
                    operators.pop();
                }
                else if (isoperator(c))
                {
                    bool b;
                    while (!operators.empty() && precedence(c) <= precedence(operators.top()))
                    {
                        bool a = operands.top();
                        operands.pop();
                        switch (operators.top())
                        {
                        case '~':
                        case '!':
                            operands.push(!a);
                            break;
                        case '&':
                            b = operands.top();
                            operands.pop();
                            operands.push(a && b);
                            break;
                        case '|':
                            b = operands.top();
                            operands.pop();
                            operands.push(a || b);
                            break;
                        }
                        operators.pop();
                    }
                    operators.push(c);
                }
                j++;
            }
            while (!operators.empty())
            {
                bool b;
                bool a = operands.top();
                operands.pop();
                switch (operators.top())
                {
                case '~':
                case '!':
                    operands.push(!a);
                    break;
                case '&':
                    b = operands.top();
                    operands.pop();
                    operands.push(a && b);
                    break;
                case '|':
                    b = operands.top();
                    operands.pop();
                    operands.push(a || b);
                    cout << operands.top();
                    break;
                }
                operators.pop();
            }
            if (!operands.empty())
            {
                // cout << it->second[0].name << endl;
                it->second[0].type = operands.top();
                // cout << "operands" << operands.top();
                // cout << "lololololollolololololololoololololololololol  " << it->second[0].name << "   " << it->second[0].type << endl;
                // Check if the output wire has been initialized
                if (it->second[0].initial.empty())
                {
                    // If not, push the current state to the initial stack
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }
                // if (it->second[0].name == "w1")
                // {
                //     cout << "yesssssss"
                //          << "    " << getWire(ioComponents, it->second[0].name) << endl;
                // }
                else if (it->second[0].initial.top() != it->second[0].type)
                {
                    it->second[0].actualdelay = getmax(it->second, ioComponents) + library[position].getDelayTime();
                    // if (it->second[0].name == "w1")
                    // {
                    //     cout << "yesssssss" << it->second[0].actualdelay << endl;
                    // }
                    // cout << "////////////////////////" << it->second[0].name << "  " << it->second[0].type << endl;
                    F_output.push_back({getDelay(ioComponents, it->second[0].name), it->second[0].name, it->second[0].type});
                    // Push the current state to the initial stack
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }
                cirInputs.push_back(it->second[0].name);
            }
        }
        if (scaleindex == 0)
        {
            currenttimescale = timeScale.at(scaleindex);
            scaleindex++;
        }
        else
        {
            // If it's not the first index
            if (scaleindex != timeScale.size())
            {
                // Update the current time scale by adding the difference between the current and previous time scales
                currenttimescale += (timeScale.at(scaleindex) - timeScale.at(scaleindex - 1));
            }
            scaleindex++;
        }
        for (auto ix = ioComponents.begin(); ix != ioComponents.end(); ix++)
        {
            // Loop through all wires in the current component
            for (int i = 0; i < ix->second.size(); i++)
            {
                // Reference to the current wire
                wire &currentWire = ix->second[i];
                // Loop through all stimuli
                for (int j = 0; j < stimuli.size(); j++)
                {
                    // Check if the current wire matches the input of any stimuli
                    if (currentWire.name == stimuli[j].getInput() && checks[j] == false)
                    {
                        // cout << "hiiiiiiiiiiiiiiiii" << currentWire.name << endl;
                        currentWire.delay = stimuli[j].getTimeStamp();
                        // Check if the wire has a delay
                        // if (currentWire.delay != 0)
                        // {
                        // If the current time scale matches the delay and it's not the last scale index
                        if (currenttimescale == currentWire.delay && scaleindex != timeScale.size() + 1)
                        {
                            currentWire.actualdelay = currentWire.delay;
                            // Toggle the type of the wire
                            if (currentWire.type == 1)
                            {
                                currentWire.type = 0;
                            }
                            else
                            {
                                currentWire.type = 1;
                            }
                            // Store the output with delay, name, and type
                            F_output.push_back({currentWire.delay, currentWire.name, currentWire.type});
                            checks[j] = true;
                            break;
                        }
                        // }
                    }
                }
            }
        }
    }
}
int findGCD(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int scale(const vector<int> &elements)
{
    int result = elements[0];
    for (int i = 1; i < elements.size(); i++)
    {
        result = findGCD(result, elements[i]);
    }
    return result;
}

int findMax(const vector<int> &vec)
{

    auto max_iterator = max_element(vec.begin(), vec.end());
    return *max_iterator;
}

bool compareStimuli(const Stimuli &a, const Stimuli &b)
{
    return a.getTimeStamp() < b.getTimeStamp();
}

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        std::cerr << "Error: Please provide a library file, circuit file, and stimuli file as arguments." << std::endl;
        return 1; // Exit with an error code
    }
    const char *libraryPath = argv[1];
    const char *circuitPath = argv[2];
    const char *stimuliPath = argv[3];
    const char *outputpath = argv[4];
    const char *commonoutputpath = argv[5];
    vector<Gates> libComponents = parseLibraryFile(libraryPath);
    vector<Stimuli> stimuli = parseStimuliFile(stimuliPath);
    vector<pair<string, vector<wire>>> mp;
    vector<int> timeScale;
    vector<string> cirInputs;
    vector<Stimuli> output;
    for (int i = 0; i < stimuli.size(); i++)
    {
        timeScale.push_back(stimuli[i].getTimeStamp());
    }
    int OScale = scale(timeScale);

    if (parseCircuitFile(circuitPath, mp, stimuli, cirInputs, libComponents))
    {
        for (int i = 0; i < libComponents.size(); i++)
        {
            cout << libComponents[i].getGateName() << libComponents[i].getNumOfInputs() << libComponents[i].getOutputExpression() << libComponents[i].getDelayTime() << endl;
        }
        computinglogic2(libComponents, mp, stimuli, timeScale, cirInputs, output);
    }
    else
    {
        cout << "Error: There is a gate in the circuit that has not been initialized in the library file.\n";
    }
    ofstream outfile(outputpath);
    ofstream out(commonoutputpath);
    if (!out.is_open())
    {
        cout << "Error opening file 'out'" << endl;
        return 1; // Exit with an error code
    }
    if (!outfile.is_open())
    {
        cout << "Error opening file 'outfile'" << endl;
        return 1; // Exit with an error code
    }
    std::sort(output.begin(), output.end(), compareStimuli);

    for (int i = 0; i < output.size(); i++)
    {
        cout << output[i].getTimeStamp() << " " << output[i].getInput() << " " << output[i].getLogicValue() << endl;
        if (output[i].getTimeStamp() != output[i + 1].getTimeStamp() || output[i].getInput() != output[i + 1].getInput() || output[i].getLogicValue() != output[i + 1].getLogicValue())
        {
            outfile << output[i].getTimeStamp() << " " << output[i].getInput() << " " << output[i].getLogicValue() << endl;
            out << output[i].getTimeStamp() << " " << output[i].getInput() << " " << output[i].getLogicValue() << endl;
        }
    }
    outfile.close();
    out.close();
    system("plot.py");
}
