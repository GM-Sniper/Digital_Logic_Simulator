#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>
#include "Circuit Classes/Stimuli.h"
#include "Circuit Classes/Gates.h"

using namespace std;
struct wire // Struct for wires is used to instantiate wires that have common attributes like name, delay,and boolean type.
{
    string name;
    bool type;
    int delay;
    int initial;
    int DGCD;
    wire(string n, int t, int d = 0) : name(n), type(t), delay(d), initial(0), DGCD(0) {}
    void settype(int t)
    {
        type = t;
    }
    void setinitial(int i)
    {
        initial = i;
    }
    int getinitial()
    {
        return initial;
    }
    void setDGCD(int d)
    {
        DGCD = d;
    }
};

vector<int> OGCD;

vector<Gates> parseLibraryFile(const string &filename)
{
    vector<Gates> components;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return components;
    }

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string name, outputExpression;
        int numInputs, delayPs;
        char comma;
        // To read the comma
        if (!(iss >> name >> numInputs >> comma >> outputExpression >> delayPs))
        {
            cerr << "Error parsing line: " << line << endl;
            continue;
        }
        if (name.at(name.length() - 1) == ',')
            name.erase(name.length() - 1, 1); // Removing the included comma in the gate name if necessery

        if (outputExpression.at(outputExpression.length() - 1) == ',')
            outputExpression.erase(outputExpression.length() - 1, 1); // Removing the included comma in the outputExpression if necessery

        components.push_back({name, numInputs, outputExpression, delayPs});
    }
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
        if (input.at(0) == ' ')
        {
            input.erase(0, 1);
        }
        // Add stimulus to the vector
        stimuli.push_back({timeStamp, input, logicValue});
    }

    file.close();
    return stimuli;
}

void parseCircuitFile(const string &filename, vector<pair<string, vector<wire>>> &mp, vector<Stimuli> stimuli) // Reads from .cir file to store the gates used in circuits, the outputs, and inputs.
{
    string input;
    vector<string> inputs2;

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        // Skip empty lines and spaces
        if (line.empty() || line[0] == ' ') // Skip lines that are empty or start with a whitespace character
            continue;

        // Parse INPUTS section
        if (line == "INPUTS:")
        {
            while (getline(file, line) && line != "COMPONENTS:")
            {
                istringstream iss(line);
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

                vector<wire> vec; // vector of wires (the first element is always the output and all other elements are the inputs used for this gate)
                // Parse each component line into a vector pair
                // Example: G0, NOT, w1, C
                istringstream iss(line);
                string name, output;
                string type;
                // vector<wire> outputs;
                if (line.empty() || line[0] == ' ') // Skip lines that are empty or start with a whitespace character
                    continue;
                if (iss >> name >> type >> output)
                {
                    if (type.at(type.length() - 1) == ',')
                    {
                        type.erase(type.length() - 1, 1);
                    }
                    if (type.at(0) == ' ')
                    {
                        type.erase(0, 1);
                    }
                    if (output.at(output.length() - 1) == ',')
                    {
                        output.erase(output.length() - 1, 1);
                    }
                    if (output.at(0) == ' ')
                    {
                        output.erase(0, 1);
                    }

                    vec.push_back(wire(output, 0)); // Here we are adding the output to the vector

                    while (iss >> input)
                    {
                        if (input.at(input.length() - 1) == ',')
                        {
                            input.erase(input.length() - 1, 1);
                        }
                        if (input.at(0) == ' ')
                        {
                            input.erase(0, 1);
                        }
                        int position = -1;
                        for (int i = 0; i < stimuli.size(); i++)
                        {
                            cout << stimuli[i].getInput() << "//" << input << endl;
                            if (input == stimuli[i].getInput())
                            {
                                position = i;
                            }
                        }
                        if (position == -1)
                        {
                            vec.push_back(wire(input, 0)); // adding the inputs to the vector
                        }
                        else
                        {
                            // cout<<"------------------------"<<stimuli[position].getTimeStamp()<<endl;
                            vec.push_back(wire(input, 0, stimuli[position].getTimeStamp()));
                        }
                    }
                }
                mp.push_back(make_pair(type, vec));
            }
        }
    }

    return;
}
bool getWire(vector<pair<string, vector<wire>>> vec, string wire_name) // a vector that returns the boolean type of the needed wire
{
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        for (int i = 0; i < it->second.size(); i++)
        {
            if (it->second[i].name == wire_name)
            {
                return it->second[i].type;
            }
        }
    }
    return 0;
}

int getDelay(vector<pair<string, vector<wire>>> vec, string wire_name)
{
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        for (int i = 0; i < it->second.size(); i++)
        {
            if (it->second[i].name == wire_name)
            {
                cout << it->second[i].name << " " << it->second[i].delay << endl;
                return it->second[i].delay;
            }
        }
    }
    return 0;
}
bool computingLogic(vector<pair<string, vector<wire>>> vec, vector<Gates> libComponents, vector<Stimuli> stimuli, ofstream &outfile, int T_scale, int maxdelay)
{
    int temp_scale = T_scale;
    bool output = false;
    int j = 0;
    // change in one of the inputs

    while (T_scale <maxdelay)
    {
        for (auto it = vec.begin(); it != vec.end(); it++)
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
                cout << "Not Gate " << endl;
                cout << "Input : " << it->second[1].name << "  Boolean state : " << getWire(vec, it->second[1].name) << endl;
                it->second[0].type = !(it->second[1].type);
                cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(vec, it->second[0].name) << endl;
                if (T_scale == temp_scale)
                {
                    it->second[0].initial = getWire(vec, it->second[0].name);
                }
                it->second[0].delay = (getDelay(vec, it->second[1].name)) + libComponents[position].getDelayTime();
                // cout<<"jsdhfkjshdjkfhsjkdhfkjs   "<<getDelay(vec, it->second[1].name)<<endl;
                // cout<<"-"<<it->second[1].name<<"-"<<endl;
                // cout << it->second[0].name << "  " << it->second[0].delay << endl;
                // cout << "kjdfhsjdhfksdhfjhsd" << it->second[0].initial << "   " << it->second[0].name << endl;
                if (it->second[0].initial != it->second[0].type)
                {
                    outfile << getDelay(vec,it->second[0].name) << " " << it->second[0].name << " " << it->second[0].type << endl;
                }
                cout << "===============================" << endl;
            }
            else if (it->first == "AND2")
            {
                cout << "AND2 Gate " << endl;
                cout << "Inputs : \n"
                     << "First : " << it->second[1].name << " Boolean state : " << getWire(vec, it->second[1].name) << endl;
                cout << "Second : " << it->second[2].name << " Boolean state : " << getWire(vec, it->second[2].name) << endl;
                it->second[0].type = (getWire(vec, it->second[1].name) & getWire(vec, it->second[2].name));
                cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(vec, it->second[0].name) << endl;
                if (T_scale == temp_scale)
                {
                    it->second[0].initial = getWire(vec, it->second[0].name);
                }
                it->second[0].delay = max(getDelay(vec, it->second[2].name), getDelay(vec, it->second[1].name)) + libComponents[position].getDelayTime();
                // cout << "kjdfhsjdhfksdhfjhsd" << it->second[0].initial << "   " << it->second[0].name << endl;
                if (it->second[0].initial != it->second[0].type)
                {
                    outfile <<  getDelay(vec,it->second[0].name) << " " << it->second[0].name << " " << it->second[0].type << endl;
                }
                cout << "===============================" << endl;
            }
            else if (it->first == "OR2")
            {
                cout << "OR2 Gate " << endl;
                cout << "Inputs : \n"
                     << "First : " << it->second[1].name << " Boolean state : " << getWire(vec, it->second[1].name) << endl;
                cout << "Second : " << it->second[2].name << " Boolean state : " << getWire(vec, it->second[2].name) << endl;
                it->second[0].type = (getWire(vec, it->second[1].name) | getWire(vec, it->second[2].name));
                cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(vec, it->second[0].name) << endl;
                it->second[0].delay = min(getDelay(vec, it->second[2].name), getDelay(vec, it->second[1].name)) + libComponents[position].getDelayTime();
                if (T_scale == temp_scale)
                {
                    it->second[0].initial = getWire(vec, it->second[0].name);
                }
                // cout << "kjdfhsjdhfksdhfjhsd" << it->second[0].initial << "   " << it->second[0].name << endl;
                if (it->second[0].initial != it->second[0].type)
                {
                    outfile <<  getDelay(vec,it->second[0].name) << " " << it->second[0].name << " " << it->second[0].type << endl;
                }
                cout << "===============================" << endl;
            }
            else if (it->first == "NAND2")
            {
                cout << "NAND2 Gate " << endl;
                cout << "Inputs : \n"
                     << "First : " << it->second[1].name << " Boolean state : " << getWire(vec, it->second[1].name) << endl;
                cout << "Second : " << it->second[2].name << " Boolean state : " << getWire(vec, it->second[2].name) << endl;
                it->second[0].type = !(getWire(vec, it->second[1].name) & getWire(vec, it->second[2].name));
                cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(vec, it->second[0].name) << endl;
                it->second[0].delay = max(getDelay(vec, it->second[2].name), getDelay(vec, it->second[1].name)) + libComponents[position].getDelayTime();
                if (T_scale == temp_scale)
                {
                    it->second[0].initial = getWire(vec, it->second[0].name);
                }
                if (it->second[0].initial != it->second[0].type)
                {
                    outfile <<  getDelay(vec,it->second[0].name) << " " << it->second[0].name << " " << it->second[0].type << endl;
                }
                cout << "===============================" << endl;
            }
            else if (it->first == "NOR2")
            {
                cout << "NOR2 Gate " << endl;
                cout << "Inputs : \n"
                     << "First : " << it->second[1].name << " Boolean state : " << getWire(vec, it->second[1].name) << endl;
                cout << "Second : " << it->second[2].name << " Boolean state : " << getWire(vec, it->second[2].name) << endl;
                it->second[0].type = !(getWire(vec, it->second[1].name) || getWire(vec, it->second[2].name));
                cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(vec, it->second[0].name) << endl;
                it->second[0].delay = min(getDelay(vec, it->second[2].name), getDelay(vec, it->second[1].name)) + libComponents[position].getDelayTime();
                if (T_scale == temp_scale)
                {
                    it->second[0].initial = getWire(vec, it->second[0].name);
                }
                if (it->second[0].initial != it->second[0].type)
                {
                    outfile <<  getDelay(vec,it->second[0].name) << " " << it->second[0].name << " " << it->second[0].type << endl;
                }
                cout << "===============================" << endl;
            }
            else if (it->first == "XOR2")
            {
                cout << "XOR2 Gate " << endl;
                cout << "Inputs : \n"
                     << "First : " << it->second[1].name << " Boolean state : " << getWire(vec, it->second[1].name) << endl;
                cout << "Second : " << it->second[2].name << " Boolean state : " << getWire(vec, it->second[2].name) << endl;
                it->second[0].type = (getWire(vec, it->second[1].name) & (!getWire(vec, it->second[2].name))) | (!getWire(vec, it->second[1].name) & getWire(vec, it->second[2].name));
                cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(vec, it->second[0].name) << endl;
                it->second[0].delay = max(getDelay(vec, it->second[2].name), getDelay(vec, it->second[1].name)) + libComponents[position].getDelayTime();
                if (T_scale == temp_scale)
                {
                    it->second[0].initial = getWire(vec, it->second[0].name);
                }
                if (it->second[0].initial != it->second[0].type)
                {
                    outfile <<  getDelay(vec,it->second[0].name) << " " << it->second[0].name << " " << it->second[0].type << endl;
                }
                cout << "===============================" << endl;
            }
            else if (it->first == "AND3")
            {
                cout << "AND3 Gate " << endl;
                cout << "Inputs : \n"
                     << "First : " << it->second[1].name << " Boolean state : " << getWire(vec, it->second[1].name) << endl;
                cout << "Second : " << it->second[2].name << " Boolean state : " << getWire(vec, it->second[2].name) << endl;
                cout << "Third : " << it->second[3].name << " Boolean state : " << getWire(vec, it->second[3].name) << endl;
                it->second[0].type = getWire(vec, it->second[1].name) & (getWire(vec, it->second[2].name) & getWire(vec, it->second[3].name));
                cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(vec, it->second[3].name) << endl;
                if (T_scale == temp_scale)
                {
                    it->second[0].initial = getWire(vec, it->second[0].name);
                }
                if (it->second[0].initial != it->second[0].type)
                {
                    outfile <<  getDelay(vec,it->second[0].name) << " " << it->second[0].name << " " << it->second[0].type << endl;
                }
                // it->second[0].delay = max(it->second[2].delay, it->second[1].delay, it->second[3].delay) + libComponents[position].getDelayTime();
                cout << "===============================" << endl;
            }
            else if (it->first == "OR3")
            {
                cout << "OR3 Gate " << endl;
                cout << "Inputs : \n"
                     << "First : " << it->second[1].name << " Boolean state : " << getWire(vec, it->second[1].name) << endl;
                cout << "Second : " << it->second[2].name << " Boolean state : " << getWire(vec, it->second[2].name) << endl;
                cout << "Third : " << it->second[3].name << " Boolean state : " << getWire(vec, it->second[3].name) << endl;
                it->second[0].type = getWire(vec, it->second[1].name) | (getWire(vec, it->second[2].name) | getWire(vec, it->second[3].name));
                cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(vec, it->second[0].name) << endl;
                if (T_scale == temp_scale)
                {
                    it->second[0].initial = getWire(vec, it->second[0].name);
                }
                if (it->second[0].initial != it->second[0].type)
                {
                    outfile << getDelay(vec,it->second[0].name)<< " " << it->second[0].name << " " << it->second[0].type << endl;
                }
                // it->second[0].delay = min(it->second[2].delay, it->second[1].delay, it->second[3].delay) + libComponents[position].getDelayTime();
                cout << "===============================" << endl;
            }
            else if (it->first == "NAND3")
            {
                cout << "NAND3 Gate " << endl;
                cout << "Inputs : \n"
                     << "First : " << it->second[1].name << " Boolean state : " << getWire(vec, it->second[1].name) << endl;
                cout << "Second : " << it->second[2].name << " Boolean state : " << getWire(vec, it->second[2].name) << endl;
                cout << "Third : " << it->second[3].name << " Boolean state : " << getWire(vec, it->second[3].name) << endl;
                it->second[0].type = !(getWire(vec, it->second[1].name) & (getWire(vec, it->second[2].name) & getWire(vec, it->second[3].name)));
                cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(vec, it->second[0].name) << endl;
                if (T_scale == temp_scale)
                {
                    it->second[0].initial = getWire(vec, it->second[0].name);
                }
                if (it->second[0].initial != it->second[0].type)
                {
                    outfile << getDelay(vec,it->second[0].name) << " " << it->second[0].name << " " << it->second[0].type << endl;
                }
                // it->second[0].delay = min(it->second[2].delay, it->second[1].delay, it->second[3].delay) + libComponents[position].getDelayTime();
                cout << "===============================" << endl;
            }
            else if (it->first == "NOR3")
            {
                cout << "NOR3 Gate " << endl;
                cout << "Inputs : \n"
                     << "First : " << it->second[1].name << " Boolean state : " << getWire(vec, it->second[1].name) << endl;
                cout << "Second : " << it->second[2].name << " Boolean state : " << getWire(vec, it->second[2].name) << endl;
                cout << "Third : " << it->second[3].name << " Boolean state : " << getWire(vec, it->second[3].name) << endl;
                it->second[0].type = !(getWire(vec, it->second[1].name) | (getWire(vec, it->second[2].name) | getWire(vec, it->second[3].name)));
                cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(vec, it->second[0].name) << endl;
                if (T_scale == temp_scale)
                {
                    it->second[0].initial = getWire(vec, it->second[0].name);
                }
                if (it->second[0].initial != it->second[0].type)
                {
                    outfile << getDelay(vec,it->second[0].name) << " " << it->second[0].name << " " << it->second[0].type << endl;
                }
                cout << "===============================" << endl;
            }

            output = it->second[0].type;
        }

        // Output wire status every 100 milliseconds
        // if (elapsedTime % 100 == 0)
        // {
        //     // cout << "Wire status " << output << endl;
        // }

        // // Check if the duration has elapsed
        // if (elapsedTime >= 1 * 1000)
        // {
        //     break;
        // }

        // Sleep for a short duration to avoid busy-waiting
        //  std::this_thread::sleep_for(std::chrono::milliseconds(50));
        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            for (int i = 0; i < it->second.size(); i++)
            {
                wire &currentWire = it->second[i];
                // currentWire.settype(1 - currentWire.type);
                // currentWire.delay += currentWire.delay; // Increment delay for the next cycle

                for (int i = 0; i < stimuli.size(); i++)
                {
                    if (currentWire.name == stimuli[i].getInput())
                    {
                        if (currentWire.delay != 0)
                        {
                            if (T_scale % currentWire.delay == 0)
                            {
                                if (currentWire.type == 1)
                                {
                                    currentWire.type = 0;
                                }
                                else
                                {
                                    currentWire.type = 1;
                                }
                                outfile << currentWire.delay << " " << currentWire.name << " " << currentWire.type << endl;
                            }
                        }
                    }
                }

                if (currentWire.type == 1)
                    output = true;
            }
        }
        T_scale = T_scale + temp_scale;
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

int main()
{
    vector<Gates> libComponents = parseLibraryFile("Tests/libFile.lib");
    vector<Stimuli> stimuli = parseStimuliFile("Tests/TestCircuit1/stimFileCirc1.stim");
    vector<int> Vscale;
    for (int i = 0; i < stimuli.size(); i++)
    {
        Vscale.push_back(stimuli[i].getTimeStamp());
    }
    int OScale = scale(Vscale);
    int maxDelay = findMax(Vscale);
    vector<pair<string, vector<wire>>> mp;
    int i = 0;
    parseCircuitFile("Tests/TestCircuit1/testCircuit1.cir", mp, stimuli);
    ofstream outfile("Tests/TestCircuit1/outputSimulation1.sim");
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
    cout << computingLogic(mp, libComponents, stimuli, outfile, OScale, maxDelay);
    outfile.close();
}
