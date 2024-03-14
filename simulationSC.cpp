#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <cctype>
#include "Circuit Classes/Stimuli.h"
#include "Circuit Classes/Gates.h"

using namespace std;
struct wire
{
    string name;
    bool type;
    int delay;

    wire(string n, bool t, int d = 0) : name(n), type(t), delay(d) {}
};

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

vector<Stimuli> parseStimuliFile(const string &filename)
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
        stringstream ss(line);
        string token;

        int timeStamp;
        string input;
        bool logicValue;

        // Parse time stamp
        getline(ss, token, ',');
        timeStamp = stoll(token);

        // Parse input
        getline(ss, token, ',');
        for (int i = 0; i < token.length(); i++)
        {
            if (token[i] != ' ')
            {
                input.push_back(token[i]);
            }
        }

        // Parse logic value
        getline(ss, token, ',');
        logicValue = stoi(token);

        // Add stimulus to the vector
        stimuli.push_back({timeStamp, input, logicValue});
    }

    file.close();
    return stimuli;
}

void parseCircuitFile(const string &filename, vector<pair<string, vector<wire>>>& mp)
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
        if (line.empty() || line[0]==' ') // Skip lines that are empty or start with a whitespace character
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

                vector<wire> vec;
                // Parse each component line into a vector pair
                // Example: G0, NOT, w1, C
                istringstream iss(line);
                string name, output;
                string type;
                vector<wire> outputs;
                if (line.empty() || line[0]==' ') // Skip lines that are empty or start with a whitespace character
                continue;
                if (iss >> name >> type >> output)
                {
                    if (type.at(type.length() - 1) == ',')
                    {
                        type.erase(type.length() - 1, 1);
                    }
                    if(type.at(0)==' ')
                    {
                        type.erase(0, 1);
                    }
                    if (output.at(output.length() - 1) == ',')
                    {
                        output.erase(output.length() - 1, 1);
                    }
                    if(output.at(0)==' ')
                    {
                        output.erase(0, 1);
                    }

                    vec.push_back(wire(output, 0));
                    // assume that the output is one char
                    // Parse input signals for the component
                    // we need to change the conspet of inputs and outputs to be inputs,outputs,wires but we will face a problem
                    // which is (the wires will be outputs and inputs in the same time)
                    // in order to solve this porblem we can use many approches. For instance we read the data stored and if it matches
                    // with the data in the input vector, we remove it (we get rid of the inputs)
                    // then we loop over ......... if it is unique, so it is the ouput, so we store it in other place, then remove it
                    // after that we will be left with the wires only. However, that is not enough becuase now you do not know which input or wire
                    // is with who. There are many approaches to solve such a problem, like 2D vector (the first vector will be of the output(wheter it is an out put or wire acts like output))
                    // then the second vector will be for the (inputs, whether it is inputs or wires acts like inputs)
                    // or we use a system of commas, like before. (Other ideas can be implemented)

                    while (iss >> input)
                    {
                        if (input.at(input.length() - 1) == ',')
                        {
                            input.erase(input.length() - 1, 1);
                        }
                        if(input.at(0)==' ')
                        {
                            input.erase(0, 1);
                        }

                        vec.push_back(wire(input, 0));
                    }

                    inputs2.push_back(input);
                }
                mp.push_back(make_pair(type, vec));
            }
        }
    }

    return;
}
bool getwire(vector<pair<string, vector<wire>>> vec, string wire_name)
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
bool function(vector<pair<string, vector<wire>>> vec)
{
    bool output = false;
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        if (it->first == "NOT")
        {
            cout << "Not Gate " << endl;
            cout << "Input : " << it->second[1].name << "  Boolean state : " << getwire(vec, it->second[1].name) << endl;
            it->second[0].type = !(it->second[1].type);
            cout << "Output : " << it->second[0].name << "  Boolean state : " << getwire(vec, it->second[0].name) << endl;
            cout << "===============================" << endl;
        }
        else if (it->first == "AND2")
        {
            cout << "AND2 Gate " << endl;
            cout << "Inputs : \n"
                 << "First : " << it->second[1].name << " Boolean state : " << getwire(vec, it->second[1].name) << endl;
            cout << "Second : " << it->second[2].name << " Boolean state : " << getwire(vec, it->second[2].name) << endl;
            it->second[0].type = (getwire(vec, it->second[1].name) & getwire(vec, it->second[2].name));
            cout << "Output : " << it->second[0].name << "  Boolean state : " << getwire(vec, it->second[0].name) << endl;
            cout << "===============================" << endl;
        }
        else if (it->first == "OR2")
        {
            cout << "OR2 Gate " << endl;
            cout << "Inputs : \n"
                 << "First : " << it->second[1].name << " Boolean state : " << getwire(vec, it->second[1].name) << endl;
            cout << "Second : " << it->second[2].name << " Boolean state : " << getwire(vec, it->second[2].name) << endl;
            it->second[0].type = (getwire(vec, it->second[1].name) | getwire(vec, it->second[2].name));
            cout << "Output : " << it->second[0].name << "  Boolean state : " << getwire(vec, it->second[0].name) << endl;
            cout << "===============================" << endl;
        }
        else if (it->first == "NAND2")
        {
            cout << "NAND2 Gate " << endl;
            cout << "Inputs : \n"
                 << "First : " << it->second[1].name << " Boolean state : " << getwire(vec, it->second[1].name) << endl;
            cout << "Second : " << it->second[2].name << " Boolean state : " << getwire(vec, it->second[2].name) << endl;
            it->second[0].type = !(getwire(vec, it->second[1].name) & getwire(vec, it->second[2].name));
            cout << "Output : " << it->second[0].name << "  Boolean state : " << getwire(vec, it->second[0].name) << endl;
            cout << "===============================" << endl;
        }
        else if (it->first == "NOR2")
        {
            cout << "NOR2 Gate " << endl;
            cout << "Inputs : \n"
                 << "First : " << it->second[1].name << " Boolean state : " << getwire(vec, it->second[1].name) << endl;
            cout << "Second : " << it->second[2].name << " Boolean state : " << getwire(vec, it->second[2].name) << endl;
            it->second[0].type = !(getwire(vec, it->second[1].name) || getwire(vec, it->second[2].name));
            cout << "Output : " << it->second[0].name << "  Boolean state : " << getwire(vec, it->second[0].name) << endl;
            cout << "===============================" << endl;
        }
        else if (it->first == "XOR2")
        {
            cout << "XOR2 Gate " << endl;
            cout << "Inputs : \n"
                 << "First : " << it->second[1].name << " Boolean state : " << getwire(vec, it->second[1].name) << endl;
            cout << "Second : " << it->second[2].name << " Boolean state : " << getwire(vec, it->second[2].name) << endl;
            it->second[0].type = (getwire(vec, it->second[1].name) & (!getwire(vec, it->second[2].name))) | (!getwire(vec, it->second[1].name) & getwire(vec, it->second[2].name));
            cout << "Output : " << it->second[0].name << "  Boolean state : " << getwire(vec, it->second[0].name) << endl;
            cout << "===============================" << endl;
        }
        else if (it->first == "AND3")
        {
            cout << "AND3 Gate " << endl;
            cout << "Inputs : \n"
                 << "First : " << it->second[1].name << " Boolean state : " << getwire(vec, it->second[1].name) << endl;
            cout << "Second : " << it->second[2].name << " Boolean state : " << getwire(vec, it->second[2].name) << endl;
            cout << "Third : " << it->second[3].name << " Boolean state : " << getwire(vec, it->second[3].name) << endl;
            it->second[0].type = getwire(vec, it->second[1].name) & (getwire(vec, it->second[2].name) & getwire(vec, it->second[3].name));
            cout << "Output : " << it->second[0].name << "  Boolean state : " << getwire(vec, it->second[3].name) << endl;
            cout << "===============================" << endl;
        }
        else if (it->first == "OR3")
        {
            cout << "OR3 Gate " << endl;
            cout << "Inputs : \n"
                 << "First : " << it->second[1].name << " Boolean state : " << getwire(vec, it->second[1].name) << endl;
            cout << "Second : " << it->second[2].name << " Boolean state : " << getwire(vec, it->second[2].name) << endl;
            cout << "Third : " << it->second[3].name << " Boolean state : " << getwire(vec, it->second[3].name) << endl;
            it->second[0].type = getwire(vec, it->second[1].name) | (getwire(vec, it->second[2].name) | getwire(vec, it->second[3].name));
            cout << "Output : " << it->second[0].name << "  Boolean state : " << getwire(vec, it->second[0].name) << endl;
            cout << "===============================" << endl;
        }
        else if (it->first == "NAND3")
        {
            cout << "NAND3 Gate " << endl;
            cout << "Inputs : \n"
                 << "First : " << it->second[1].name << " Boolean state : " << getwire(vec, it->second[1].name) << endl;
            cout << "Second : " << it->second[2].name << " Boolean state : " << getwire(vec, it->second[2].name) << endl;
            cout << "Third : " << it->second[3].name << " Boolean state : " << getwire(vec, it->second[3].name) << endl;
            it->second[0].type = !(getwire(vec, it->second[1].name) & (getwire(vec, it->second[2].name) & getwire(vec, it->second[3].name)));
            cout << "Output : " << it->second[0].name << "  Boolean state : " << getwire(vec, it->second[0].name) << endl;
            cout << "===============================" << endl;
        }
        else if (it->first == "NOR3")
        {
            cout << "NOR3 Gate " << endl;
            cout << "Inputs : \n"
                 << "First : " << it->second[1].name << " Boolean state : " << getwire(vec, it->second[1].name) << endl;
            cout << "Second : " << it->second[2].name << " Boolean state : " << getwire(vec, it->second[2].name) << endl;
            cout << "Third : " << it->second[3].name << " Boolean state : " <<getwire(vec, it->second[3].name) << endl;
            it->second[0].type = !(getwire(vec, it->second[1].name) | (getwire(vec, it->second[2].name) | getwire(vec, it->second[3].name)));
            cout << "Output : " << it->second[0].name << "  Boolean state : " << getwire(vec, it->second[0].name) << endl;
            cout << "===============================" << endl;
        }
        output = it->second[0].type;
    }

    return output;
}
int main()
{
    vector<Gates> libComponents = parseLibraryFile("Tests/libFile.lib");
    vector<Stimuli> stimuli = parseStimuliFile("Tests/TestCircuit1/stimFileCir1.stim");
    vector<pair<string, vector<wire>>> mp;
    int i=0;
    parseCircuitFile("Tests/TestCircuit4/Circuit4.cir",mp);
    for (auto it = mp.begin(); it != mp.end(); it++)
    {   
        cout <<i<<" "<< it->first << " ";
        for (int i = 0; i < it->second.size(); i++)
        {
            cout << it->second[i].name << " ";
        }
        cout << endl;
        i++;
    }
    cout << function(mp);
}
