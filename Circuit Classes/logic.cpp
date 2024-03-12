#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct wire
{
    string name;
    bool type;

    wire(string n,bool t): name(n),type(t){}
};

bool getwire(map<string,vector<wire>> vec,string wire_name)
{
    for(auto it=vec.begin();it!=vec.end();it++)
    {
        if(it->second[0].name==wire_name)
        {
            cout<<"annnnaaaaaaaaaaa "<<it->second[0].name<<"   "<<it->second[0].type<<endl;
            return it->second[0].type;
        }
    }
    return 0;
}
bool function(map<string,vector<wire>> vec)
{
    bool output;
    for(auto it=vec.begin();it!=vec.end();it++)
    {
        if(it->first=="NOT")
        {
            cout<<"jhfkshdfhsjkdhfkshdfkjhdkjfhskjhdfkjhskjfh"<<endl;
            it->second[0].type=!(it->second[1].type);
        }
        else if(it->first=="AND2")
        {
            cout<<"jkshdfsjkdfh"<<endl;
            cout<<it->second[1].name<<endl;
            cout<<getwire(vec,it->second[1].name)<<endl;
            it->second[0].type=(getwire(vec,it->second[1].name)&getwire(vec,it->second[2].name));
        }
        output=it->second[0].type;
    }

    return output;
}

int main()
{
    wire A("A",0);
    wire B("B",1);
    wire C("C",0);
    wire D("D",0);
    map<string,vector<wire>> mp={/*{"NOT",{wire("w1",0),A}},*/{"AND2",{wire("w2",0),wire("w1",1),B}}};

    cout<<function(mp)<<endl;
}