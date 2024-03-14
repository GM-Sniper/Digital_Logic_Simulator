#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

using namespace std;

struct wire
{
    string name;
    int type;
    int delay;
    int initial;
    wire(string n, int t, int d) : name(n), type(t), delay(d), initial(0) {}
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
};

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
    return false;
}

bool function(vector<pair<string, vector<wire>>> vec, int duration)
{
    auto startTime = std::chrono::steady_clock::now();
    bool output = false;

    while (true)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count(); 

        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            for (int i = 0; i < it->second.size(); i++)
            {
                wire &currentWire = it->second[i];

                if (elapsedTime >= currentWire.delay)
                {
                    if (currentWire.getinitial() == 0)
                    {
                        currentWire.setinitial(1);
                    }
                    else
                    {
                        cout << currentWire.delay << " " << currentWire.name << " " << currentWire.type << endl;
                    }
                    currentWire.settype(1 - currentWire.type);
                    currentWire.delay += currentWire.delay; 
                }

                if (currentWire.type == 1) 
                    output = true;
            }
        }

        for (auto it = vec.begin(); it != vec.end(); it++)
        {
            if (it->first == "NOT")
            {
                it->second[0].type = !(it->second[1].type);
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
                it->second[0].type = !(getwire(vec, it->second[1].name) & getwire(vec, it->second[2].name));
            }
            else if (it->first == "NOR2")
            {
                it->second[0].type = !(getwire(vec, it->second[1].name) | getwire(vec, it->second[2].name));
            }
            else if (it->first == "XOR2")
            {
                it->second[0].type = (getwire(vec, it->second[1].name) & (!getwire(vec, it->second[2].name))) | (!getwire(vec, it->second[1].name) && getwire(vec, it->second[2].name));
            }
            else if (it->first == "AND3")
            {
                it->second[0].type = getwire(vec, it->second[1].name) & (getwire(vec, it->second[2].name) & getwire(vec, it->second[3].name));
            }
            else if (it->first == "OR3")
            {
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
            output = it->second[0].type;
        }
        cout << output << endl;

        if (elapsedTime >= duration * 1000) 
        {
            break;
        }
    }

    return output;
}

int main()
{
    wire A("A", 1, 100); 
    wire B("B", 1, 400);
    vector<pair<string, vector<wire>>> vec = {{"NOT", {wire("w1", 0, 0), A}}};
    function(vec, 1); 

    return 0;
}
