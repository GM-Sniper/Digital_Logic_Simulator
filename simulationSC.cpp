#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include "Circuit Classes/Circuits.h"
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
vector<pair<string, vector<wire>>> mp;
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

        // components.push_back({name, numInputs, outputExpression, delayPs});
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

vector<CircuitComponent> parseCircuitFile(const string &filename, const string &filenameStimuli)
{
    vector<CircuitComponent> components;
    vector<Stimuli> stimuli = parseStimuliFile(filenameStimuli);
    vector<Stimuli> inputsWithData;
    string input;
    vector<string> inputs2;

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return components;
    }

    string line;
    while (getline(file, line))
    {
        // Skip empty lines
        if (line.empty())
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
                // Parse each component line
                // Example: G0, NOT, w1, C
                istringstream iss(line);
                string name, output;
                string type;

                // vector<Stimuli> inputs;
                vector<wire> outputs;

                if (iss >> name >> type >> output)
                { // wire(output,0);

                    type.erase(type.length() - 1, 1);
                    output.erase(output.length() - 1, 1);
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

                        vec.push_back(wire(input, 0));
                        // Create Stimuli object for each input and add it to a vector
                        // Stimuli stimuli(0, input[0], false); // Set initial time stamp to 0 and logic value to false
                        // You might want to validate and process the input further as needed
                        // inputs.push_back(stimuli); // this has a problem becasue only the first inputs has the time delay
                        //, so for the others you will need to compute it
                    }

                    inputs2.push_back(input);
                    // for(int i=0;i<input.length();i++)
                    // {
                    //     if(input)
                    //      Stimuli sti(0,input[i],false);
                    //     outputs.push_back(sti);
                    // }
                    // Create Gates object for the component type
                    // Gates gate(name, type , 0, "", 0); // You might need to provide appropriate values here
                    // Create CircuitComponent object and add it to the vector
                    // components.push_back(CircuitComponent(name, gate, output, inputs));
                }
                mp.push_back(make_pair(type, vec));
            }
        }
    }

    return components;
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
        cout << "dsfghsdgfhjsdgfhgdsgchjscgvhujgdsxhgf" << it->first << endl;
        if (it->first == "NOT")
        {
            // cout << "Nooooooooooooooooooooooooooooot" << endl;
            // cout << it->second[1].name << endl;
            it->second[0].type = !(it->second[1].type);
            // cout << "hgsdhgfjs  " << it->second[0].type << endl;
        }
        else if (it->first == "AND2")
        {
            it->second[0].type = (getwire(vec, it->second[1].name) & getwire(vec, it->second[2].name));
        }
        else if (it->first == "OR2")
        {
            it->second[0].type = (getwire(vec, it->second[1].name) | getwire(vec, it->second[2].name));
        }
        else if (it->first == "NAND2")
        {
            cout<<"NAaannnnnnnnnnn"<<it->second[1].name<<"   "<<it->second[2].name<<endl;
            cout<<getwire(vec, it->second[1].name)<<" "<<getwire(vec, it->second[2].name)<<endl;
            it->second[0].type = !(getwire(vec, it->second[1].name) & getwire(vec, it->second[2].name));
        }
        else if (it->first == "NOR2")
        {
            it->second[0].type = !(getwire(vec, it->second[1].name) | getwire(vec, it->second[2].name));
        }
        else if (it->first == "XOR2")
        {
            cout<<"xoooorrrrrrrrrr"<<getwire(vec, it->second[1].name)<<"  "<<!getwire(vec, it->second[2].name)<<endl;
            it->second[0].type = (getwire(vec, it->second[1].name) & (!getwire(vec, it->second[2].name))) | (!getwire(vec, it->second[1].name) && getwire(vec, it->second[2].name));
        }
        else if (it->first == "AND3")
        {
            it->second[0].type = getwire(vec, it->second[1].name) & (getwire(vec, it->second[2].name) & getwire(vec, it->second[3].name));
        }
        else if (it->first == "OR3")
        {
            cout << getwire(vec, it->second[1].name) << "   " << getwire(vec, it->second[2].name) << "  " << getwire(vec, it->second[3].name) << endl;
            it->second[0].type = getwire(vec, it->second[1].name) | (getwire(vec, it->second[2].name) | getwire(vec, it->second[3].name));
        }
        else if (it->first == "NAND3")
        {
            it->second[0].type = !(getwire(vec, it->second[1].name) & (getwire(vec, it->second[2].name) & getwire(vec, it->second[3].name)));
        }
        else if (it->first == "NOR3")
        {
            it->second[0].type = !(getwire(vec, it->second[1].name) | (getwire(vec, it->second[2].name) | getwire(vec, it->second[3].name)));
        }
        //cout << "helllllllllllllllllooooooooooooooooooooooooo" << it->second[0].name << endl;
        output = it->second[0].type;
    }

    return output;
}
int main()
{
    // vector<Gates> components = parseLibraryFile("Tests/libFile.lib");
    vector<Stimuli> stimuli = parseStimuliFile("Tests/TestCircuit5/stimFileCir5.stim");
    /* for(const auto& component : components) {
         cout << "Component: " << component.getGateName() << " Num Inputs: " << component.getNumOfInputs()
                   << " Output Expression: " << component.getOutputExpression() << " Delay (ps): " << component.getDelayTime() << endl;
     }*/
    vector<CircuitComponent> comps = parseCircuitFile("Tests/TestCircuit4/testCircuit4.cir", "Tests/TestCircuit4/stimFileCir4.stim");

    mp.pop_back();
    for (auto it = mp.begin(); it != mp.end(); it++)
    {
        cout << "nn" << it->first << " ";
        for (int i = 0; i < it->second.size(); i++)
        {
            cout << it->second[i].name << " ";
        }
        cout << endl;
    }
    cout << function(mp);
}
