#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct wire             
{
    string name;
    bool type;

    // determine (that type change from input/ wire / output to be stages hence we intisialize it to be zero then if it is used for once)
    // it will be output(1) ; if it is used again it will be input(2) // there should be a desgin wise how to change it in order not to
    // fall down what if the wire after it is became output, it was used twice as inputs (we can know that by the time delay) 

    wire(string n, bool t) : name(n), type(t) {}
};

bool getwire(map<string, vector<wire>> vec, string wire_name)
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
bool function(map<string, vector<wire>> vec)
{
    bool output = false;
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
        else if(it->first=="OR3")
        {
            it->second[0].type= getwire(vec, it->second[1].name) | (getwire(vec, it->second[2].name) | getwire(vec, it->second[3].name));
        }
        else if(it->first=="NAND3")
        {
            it->second[0].type= !(getwire(vec, it->second[1].name) & (getwire(vec, it->second[2].name) & getwire(vec, it->second[3].name)));
        }
        else if(it->first=="NOR3")
        {
            it->second[0].type=!(getwire(vec, it->second[1].name) | (getwire(vec, it->second[2].name) | getwire(vec, it->second[3].name)));
        }
        output = it->second[2].type;
    }

    return output;
}

int main()
{
    wire A("A", 1);
    wire B("B", 1);
    wire C("C", 1);
    wire D("D", 0);
    map<string, vector<wire>> mp = {{"NOT", {wire("w1", 0), A}}, {"AND2", {wire("w2", 0), wire("w1", 0), B}}, {"XOR2", {wire("w3", 0), D, C}}};
    cout << function(mp) << endl;
    // function(mp);
}