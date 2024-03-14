#include<iostream>
#include<vector>
#include<string>
#include<sstream>

using namespace std;

int main()
{
    string ex = "AND2,2,i1&i2,200";

    string Stemp;
    vector<string> temp;
    pair<string, vector<string>> E;

    stringstream ss(ex);
    string item;
    int count = 0;

    while (getline(ss, item, ',')) {
        if (count == 0) {
            Stemp = item;
        } else {
            temp.push_back(item);
        }
        count++;
    }
cout<<"the vector of strings\n";
    for (int i = 0; i < temp.size(); i++) {
        cout << temp[i] << " ";
    }

    E = make_pair(Stemp, temp);
    cout<<endl;
    cout<<"prinint the vector of pairs\n";
    cout << "First: " << E.first << ", Second: ";
    for (int i = 0; i < E.second.size(); i++) {
        cout << E.second[i] << " ";
    }
    cout << endl;

    return 0;
}
