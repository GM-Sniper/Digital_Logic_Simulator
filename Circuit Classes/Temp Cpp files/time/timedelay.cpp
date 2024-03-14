#include<iostream>
#include <chrono>
#include <thread>
#include<string>
#include<vector>

using namespace std; 

struct wire             
{
    string name;
    int type;
    int delay;
    wire(string n, bool t) : name(n), type(t) {}
    void settype(bool t)
    {
        type=1-t;
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
    return 0;
}
bool function(vector<pair<string, vector<wire>>> vec, int duration)
{
    auto startTime = std::chrono::steady_clock::now();


    bool output = false;


    while (true) 
    {
    auto currentTime = std::chrono::steady_clock::now();

    // for( auto it= vec.begin();it!=vec.end();it++)
    // {

    // }
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
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
        output = it->second[0].type;
    }
    cout<<output<<endl;
        // Stop the loop after the desired duration
        if (elapsedTime >= duration) {
            break;
        }
    }


    return output;
}


void timedelay( int A, int duration )
{
    // we can edit the function to change more number of inputs and the duration can be done for 
    // the rest of the curcuit, but we will to calculate the time delay of the (wires)/(outputs) 
    auto startTime = std::chrono::steady_clock::now();
    while (true) {
        std::cout << "A is now " << A << std::endl;

        A = 1 - A;
        

        // Introduce a delay of 100 milliseconds
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto currentTime = std::chrono::steady_clock::now();

        // Calculate the elapsed time in seconds
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        // Stop the loop after the desired duration
        if (elapsedTime >= duration) {
            break;
        }
    }

}


int main()
{

//int A=0;
//timedelay(A,1);


    wire A("A", 1);
    wire B("B", 1);
    wire C("C", 1);
    wire D("D", 0);
    vector<pair<string, vector<wire>>> vec = {{"NOT", {wire("w1", 0), A}}};//, {"AND2", {wire("w2", 0), wire("w1", 0), B}}, {"XOR2", {wire("w3", 0), D, C}}};
     function(vec,1);




    return 0;
}
