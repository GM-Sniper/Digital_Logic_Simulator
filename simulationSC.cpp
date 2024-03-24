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
#include "Circuit Classes/Stimuli.h"
#include "Circuit Classes/Gates.h"

using namespace std;
struct wire // Struct for wires is used to instantiate wires that have common attributes like name, delay,and boolean type.
{
    string name;
    bool type;
    int delay;
    stack<int> initial;
    wire(string n, int t, int d = 0) : name(n), type(t), delay(d) {}
};

vector<Gates> parseLibraryFile(const string &filename)
{
    // Vector to store parsed Gates components
    vector<Gates> components;

    // Open the file
    ifstream file(filename);
    // Check if the file is successfully opened
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return components; // Return an empty vector if the file cannot be opened
    }

    string line;
    // Read each line from the file
    while (getline(file, line))
    {
        // Create a string stream to parse the line
        istringstream iss(line);
        string name, outputExpression;
        int numInputs, delayPs;
        char comma;
        // Read the gate name, number of inputs, output expression, and delay from the line
        if (!(iss >> name >> numInputs >> comma >> outputExpression >> delayPs))
        {
            cerr << "Error parsing line: " << line << endl;
            continue; // Skip to the next line if parsing fails
        }
        // Remove the trailing comma from the gate name if necessary
        if (name.at(name.length() - 1) == ',')
            name.erase(name.length() - 1, 1);
        while (name.at(0) == ' ')
        {
            name.erase(0, 1);
        }
        // Remove the trailing comma from the output expression if necessary
        if (outputExpression.at(outputExpression.length() - 1) == ',')
            outputExpression.erase(outputExpression.length() - 1, 1);
        while (outputExpression.at(0) == ' ')
        {
            outputExpression.erase(0, 1);
        }
        // Create a Gates object with the parsed information and add it to the components vector
        components.push_back({name, numInputs, outputExpression, delayPs});
    }
    // Close the file
    file.close();
    // Return the vector containing parsed Gates components
    return components;
}

vector<Stimuli> parseStimuliFile(const string &filename) // Reads from .stim files to read the time delay used for each input
{
    vector<Stimuli> stimuli;

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        return stimuli;
    }

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string token;
        if (line.empty() || line[0] == ' ') // Skip lines that are empty or start with a whitespace character
            continue;
        // Parse time stamp
        if (!getline(iss, token, ','))
        {
            cerr << "Error: Missing timestamp in line: " << line << endl;
            continue; // Skip to the next line
        }
        int timeStamp;
        try
        {
            timeStamp = stoi(token);
        }
        catch (...)
        {
            cerr << "Error: Invalid timestamp in line: " << line << endl;
            continue; // Skip to the next line
        }

        // Parse input
        if (!getline(iss, token, ','))
        {
            cerr << "Error: Missing input in line: " << line << endl;
            continue; // Skip to the next line
        }
        string input = token;

        // Parse logic value
        if (!getline(iss, token, ','))
        {
            cerr << "Error: Missing logic value in line: " << line << endl;
            continue; // Skip to the next line
        }
        bool logicValue;
        try
        {
            logicValue = stoi(token);
        }
        catch (...)
        {
            cerr << "Error: Invalid logic value in line: " << line << endl;
            continue; // Skip to the next line
        }

        // Validate logic value
        if (logicValue != 0 && logicValue != 1)
        {
            cerr << "Error: Invalid logic value (must be 0 or 1) in line: " << line << endl;
            continue; // Skip to the next line
        }
        while (input.at(0) == ' ')
        {
            input.erase(0, 1);
        }
        // Add stimulus to the vector
        stimuli.push_back({timeStamp, input, logicValue});
    }

    file.close();
    return stimuli;
}

void parseCircuitFile(const string &filename, vector<pair<string, vector<wire>>> &ioComponents, vector<Stimuli> stimuli)
{
    string input;
    vector<string> inputs2;

    // Open the circuit file
    ifstream file(filename);
    // Check if the file is successfully opened
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return; // Return if the file cannot be opened
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
                    inputs2.push_back(input);
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
                    while (type.at(type.length() - 1) == ',')
                    {
                        type.erase(type.length() - 1, 1);
                    }
                    while (type.at(0) == ' ')
                    {
                        type.erase(0, 1);
                    }
                    while (output.at(output.length() - 1) == ',')
                    {
                        output.erase(output.length() - 1, 1);
                    }
                    while (output.at(0) == ' ')
                    {
                        output.erase(0, 1);
                    }
                    // Add output to the vector of wires
                    ioVector.push_back(wire(output, 0));

                    // Read inputs for the gate
                    while (iss >> input)
                    {
                        while (input.at(input.length() - 1) == ',')
                        {
                            input.erase(input.length() - 1, 1);
                        }
                        while (input.at(0) == ' ')
                        {
                            input.erase(0, 1);
                        }
                        int position = -1;
                        // Check if the input is provided as stimuli
                        for (int i = 0; i < stimuli.size(); i++)
                        {
                            if (input == stimuli[i].getInput())
                            {
                                position = i;
                            }
                        }
                        // Add the input to the vector of wires
                        if (position == -1)
                        {
                            ioVector.push_back(wire(input, 0));
                        }
                        else
                        {
                            // Add input with timestamp if it's provided as stimuli
                            ioVector.push_back(wire(input, 0, stimuli[position].getTimeStamp()));
                        }
                    }
                }
                // Add the gate and its corresponding vector of wires to the ioComponents vector
                ioComponents.push_back(make_pair(type, ioVector));
            }
        }
    }
    // Close the file
    file.close();
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
                return it->second[i].delay;
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

bool computingLogic(vector<pair<string, vector<wire>>> ioComponents, vector<Gates> libComponents, vector<Stimuli> stimuli, vector<int> timeScale, vector<Stimuli> &F_output) // send the delay vector to the function
{
    int currentTimeScale = 0;
    bool output = false;
    int scaleIndex = 0;
    while (scaleIndex <= timeScale.size())
    {
        for (auto it = ioComponents.begin(); it != ioComponents.end(); it++)
        {

            int position = -1;
            for (int i = 0; i < libComponents.size(); i++)
            {
                if (it->first == libComponents[i].getGateName())
                {
                    position = i;
                    break;
                }
            }
            if (it->first == "NOT")
            {
                // Print the gate type for debugging purposes
                cout << "Not Gate " << endl;

                // Print input wire and its boolean state
                cout << "Input : " << it->second[1].name << "  Boolean state : " << getWire(ioComponents, it->second[1].name) << endl;

                // Apply NOT operation to input wire and update output wire
                it->second[0].type = !(it->second[1].type);

                // Print output wire and its boolean state
                cout << "Output : \n"
                     << it->second[0].name << "  Boolean state : " << getWire(ioComponents, it->second[0].name) << endl;

                // Check if the output wire has been initialized
                if (it->second[0].initial.empty())
                {
                    // If not, push the current state to the initial stack
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }

                else if (it->second[0].initial.top() != it->second[0].type)
                {
                    // If the current state is different from the initial state, calculate delay and update
                    it->second[0].delay = (getDelay(ioComponents, it->second[1].name)) + libComponents[position].getDelayTime();
                    F_output.push_back({getDelay(ioComponents, it->second[0].name), it->second[0].name, it->second[0].type});

                    // Push the current state to the initial stack
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }
                // else
                // {
                //     // Clear the initial state stack if the state remains unchanged
                //     while (it->second[0].initial.size() != 0)
                //     {
                //         it->second[0].initial.pop();
                //     }
                // }
                /// S & N &V
                // if (input.substr(0, 3) == "AN
                // Print a separator for clarity
                cout << "===============================" << endl;
            }
            else if (it->first.substr(0, 3) == "AND")
            {
                // Print the gate type for debugging purposes
                cout << "AND Gate " << endl;

                // Print input wires and their boolean states
                // Apply AND operation to input wires and update output wire
                // it->second[0].type = (getWire(ioComponents, it->second[1].name) & getWire(ioComponents, it->second[2].name));

                for (int i = 1; i < it->second.size(); i++)
                {
                    if (i == 1)
                    {
                        it->second[0].type = getWire(ioComponents, it->second[i].name) & getWire(ioComponents, it->second[++i].name);
                        cout << "Inputs : \n"
                             << "1) " << it->second[1].name << " Boolean state : " << getWire(ioComponents, it->second[1].name) << endl;
                        cout << "2) " << it->second[2].name << " Boolean state : " << getWire(ioComponents, it->second[2].name) << endl;
                    }
                    else
                    {
                        it->second[0].type = it->second[0].type & getWire(ioComponents, it->second[i].name);
                        cout << i << ") " << it->second[i].name << " Boolean state : " << getWire(ioComponents, it->second[i].name) << endl;
                    }
                }

                // Print output wire and its boolean state
                cout << "Output : \n"
                     << it->second[0].name << "  Boolean state : " << getWire(ioComponents, it->second[0].name) << endl;
                // Check if the output wire has been initialized
                if (it->second[0].initial.empty())
                {
                    // If not, push the current state to the initial stack
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }
                else if (it->second[0].initial.top() != it->second[0].type)
                {
                    // If the current state is different from the initial state, calculate delay and update

                    it->second[0].delay = getmax(it->second, ioComponents) + libComponents[position].getDelayTime();

                    F_output.push_back({getDelay(ioComponents, it->second[0].name), it->second[0].name, it->second[0].type});

                    // Push the current state to the initial stack
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }

                // Print a separator for clarity
                cout << "===============================" << endl;
            }
            else if (it->first.substr(0, 2) == "OR")
            {
                // Display gate type
                cout << "OR Gate " << endl;

                // Display inputs and their boolean states

                // Apply OR operation to input wires and update output wire
                // it->second[0].type = (getWire(ioComponents, it->second[1].name) | getWire(ioComponents, it->second[2].name));

                for (int i = 1; i < it->second.size(); i++)
                {
                    if (i == 1)
                    {
                        it->second[0].type = !getWire(ioComponents, it->second[i].name) & !getWire(ioComponents, it->second[++i].name);
                        cout << "Inputs : \n"
                             << "1) " << it->second[1].name << " Boolean state : " << getWire(ioComponents, it->second[1].name) << endl;
                        cout << "2) " << it->second[2].name << " Boolean state : " << getWire(ioComponents, it->second[2].name) << endl;
                    }
                    else
                    {
                        it->second[0].type = !(it->second[0].type) & !getWire(ioComponents, it->second[i].name);
                        cout << i << ") " << it->second[i].name << " Boolean state : " << getWire(ioComponents, it->second[i].name) << endl;
                    }
                }

                // Display output and its boolean state
                cout << "Output : \n"
                     << it->second[0].name << "  Boolean state : " << getWire(ioComponents, it->second[0].name) << endl;

                // Check if the output wire has been initialized
                if (it->second[0].initial.empty())
                {
                    // If not, push the boolean state to the stack
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }
                else if (it->second[0].initial.top() != it->second[0].type)
                {
                    // If the top of the stack is different from the current boolean state, update delay and push to stack

                    it->second[0].delay = getmin(it->second, ioComponents) + libComponents[position].getDelayTime();

                    F_output.push_back({getDelay(ioComponents, it->second[0].name), it->second[0].name, it->second[0].type});

                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }

                // Separator for readability
                cout << "===============================" << endl;
            }
            else if (it->first.substr(0, 4) == "NAND")
            {
                // Display gate type
                cout << "NAND Gate " << endl;

                // Display inputs and their boolean states
                for (int i = 1; i < it->second.size(); i++)
                {
                    if (i == 1)
                    {
                        it->second[0].type = !getWire(ioComponents, it->second[i].name) | !getWire(ioComponents, it->second[++i].name);
                        cout << "Inputs : \n"
                             << "1) " << it->second[1].name << " Boolean state : " << getWire(ioComponents, it->second[1].name) << endl;
                        cout << "2) " << it->second[2].name << " Boolean state : " << getWire(ioComponents, it->second[2].name) << endl;
                    }
                    else
                    {
                        it->second[0].type = (it->second[0].type) | !getWire(ioComponents, it->second[i].name);
                        cout << i << ") " << it->second[i].name << " Boolean state : " << getWire(ioComponents, it->second[i].name) << endl;
                    }
                }

                // Apply NAND operation to input wires and update output wire
                it->second[0].type = !(getWire(ioComponents, it->second[1].name) & getWire(ioComponents, it->second[2].name));

                // Display output and its boolean state
                cout << "Output : \n"
                     << it->second[0].name << "  Boolean state : " << getWire(ioComponents, it->second[0].name) << endl;

                // Check if the output wire has been initialized
                if (it->second[0].initial.empty())
                {
                    // If not, push the boolean state to the stack
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }
                else if (it->second[0].initial.top() != it->second[0].type)
                {
                    // If the top of the stack is different from the current boolean state, update delay and push to stack

                    it->second[0].delay = getmax(it->second, ioComponents) + libComponents[position].getDelayTime();

                    F_output.push_back({getDelay(ioComponents, it->second[0].name), it->second[0].name, it->second[0].type});
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }

                // Separator for readability
                cout << "===============================" << endl;
            }
            else if (it->first.substr(0, 3) == "NOR")
            {
                // Display gate type
                cout << "NOR Gate " << endl;

                for (int i = 1; i < it->second.size(); i++)
                {
                    if (i == 1)
                    {
                        it->second[0].type = !getWire(ioComponents, it->second[i].name) & !getWire(ioComponents, it->second[++i].name);
                        cout << "Inputs : \n"
                             << "1) " << it->second[1].name << " Boolean state : " << getWire(ioComponents, it->second[1].name) << endl;
                        cout << "2) " << it->second[2].name << " Boolean state : " << getWire(ioComponents, it->second[2].name) << endl;
                    }
                    else
                    {
                        it->second[0].type = (it->second[0].type) & !getWire(ioComponents, it->second[i].name);
                        cout << i << ") " << it->second[i].name << " Boolean state : " << getWire(ioComponents, it->second[i].name) << endl;
                    }
                }

                // Display output and its boolean state
                cout << "Output : \n"
                     << it->second[0].name << "  Boolean state : " << getWire(ioComponents, it->second[0].name) << endl;

                // Check if the output wire has been initialized
                if (it->second[0].initial.empty())
                {
                    // If not, push the boolean state to the stack
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }
                else if (it->second[0].initial.top() != it->second[0].type)
                {
                    // If the top of the stack is different from the current boolean state, update delay and push to stack

                    it->second[0].delay = getmax(it->second, ioComponents) + libComponents[position].getDelayTime();

                    F_output.push_back({getDelay(ioComponents, it->second[0].name), it->second[0].name, it->second[0].type});
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }

                // Separator for readability
                cout << "===============================" << endl;
            }
            else if (it->first.substr(0, 3) == "XOR")
            {
                // Display gate type
                cout << "XOR Gate " << endl;

                // Display inputs and their boolean states
                for (int i = 1; i < it->second.size(); i++)
                {
                    if (i == 1)
                    {
                        it->second[0].type = getWire(ioComponents, it->second[i].name) ^ getWire(ioComponents, it->second[++i].name);
                        cout << "Inputs : \n"
                             << "1) " << it->second[1].name << " Boolean state : " << getWire(ioComponents, it->second[1].name) << endl;
                        cout << "2) " << it->second[2].name << " Boolean state : " << getWire(ioComponents, it->second[2].name) << endl;
                    }
                    else
                    {
                        it->second[0].type = (it->second[0].type) ^ getWire(ioComponents, it->second[i].name);
                        cout << i << ") " << it->second[i].name << " Boolean state : " << getWire(ioComponents, it->second[i].name) << endl;
                    }
                }

                // Display output and its boolean state
                cout << "Output : \n"
                     << it->second[0].name << "  Boolean state : " << getWire(ioComponents, it->second[0].name) << endl;

                // Check if the output wire has been initialized
                if (it->second[0].initial.empty())
                {
                    // If not, push the boolean state to the stack
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }
                else if (it->second[0].initial.top() != it->second[0].type)
                {
                    // If the top of the stack is different from the current boolean state, update delay and push to stack
                    it->second[0].delay = getmax(it->second, ioComponents) + libComponents[position].getDelayTime();
                    F_output.push_back({getDelay(ioComponents, it->second[0].name), it->second[0].name, it->second[0].type});
                    it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
                }

                // Separator for readability
                cout << "===============================" << endl;
            }
            output = it->second[0].type;
        }
        if (scaleIndex == 0)
        {
            // If it's the first index, set the current time scale to the first value in the time scale vector
            currentTimeScale = timeScale.at(scaleIndex);
            scaleIndex++;
        }
        else
        {
            // If it's not the first index
            if (scaleIndex != timeScale.size())
            {
                // Update the current time scale by adding the difference between the current and previous time scales
                currentTimeScale += (timeScale.at(scaleIndex) - timeScale.at(scaleIndex - 1));
            }
            scaleIndex++;
        }

        // Loop through all components in the input/output components map
        for (auto it = ioComponents.begin(); it != ioComponents.end(); it++)
        {
            // Loop through all wires in the current component
            for (int i = 0; i < it->second.size(); i++)
            {
                // Reference to the current wire
                wire &currentWire = it->second[i];

                // Loop through all stimuli
                for (int i = 0; i < stimuli.size(); i++)
                {
                    // Check if the current wire matches the input of any stimuli
                    if (currentWire.name == stimuli[i].getInput())
                    {
                        // Check if the wire has a delay
                        if (currentWire.delay != 0)
                        {
                            // If the current time scale matches the delay and it's not the last scale index
                            if (currentTimeScale == currentWire.delay && scaleIndex != timeScale.size() + 1)
                            {
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
                            }
                        }
                    }
                }

                // Check if the wire's type is 1 (true)
                if (currentWire.type == 1)
                    output = true;
            }
        }
        // increment the time scale by the difference between two consicutive elements in the vector
        // if i=1, currentTimeScale  = vector.at(i)-0
        // else currentTimeScale = vector.at(i)-vector.at(i-1)
    }
    return output;
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

int main()
{
    vector<Gates> libComponents = parseLibraryFile("new/lib2.lib");
    vector<Stimuli> stimuli = parseStimuliFile("new/2.stim");
    vector<int> timeScale;
    vector<Stimuli> output;
    for (int i = 0; i < stimuli.size(); i++)
    {
        timeScale.push_back(stimuli[i].getTimeStamp());
    }
    int OScale = scale(timeScale);
    vector<pair<string, vector<wire>>> mp;
    int i = 0;
    parseCircuitFile("new/2.cir", mp, stimuli);
    ofstream outfile("new/3.sim");
    if (!outfile.is_open())
    {
        cerr << "Error opening output file" << endl;
        return 1;
    }
    for (auto it = mp.begin(); it != mp.end(); it++)
    {
        cout << i << " " << it->first << " ";
        for (int i = 0; i < it->second.size(); i++)
        {
            cout << it->second[i].name << " ";
        }
        cout << endl;
        i++;
    }
    cout << computingLogic(mp, libComponents, stimuli, timeScale, output);

    std::sort(output.begin(), output.end(), compareStimuli);

    for (int i = 0; i < output.size(); i++)
    {
        if (output[i].getTimeStamp() != output[i + 1].getTimeStamp() || output[i].getInput() != output[i + 1].getInput() || output[i].getLogicValue() != output[i + 1].getLogicValue() )
        {
            outfile << output[i].getTimeStamp() << " " << output[i].getInput() << " " << output[i].getLogicValue() << endl;
        }
    }

    outfile.close();
    system("plot.py");
}
