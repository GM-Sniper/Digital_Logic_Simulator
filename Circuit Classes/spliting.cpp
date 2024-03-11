// #include<iostream>
// #include<string>
// using namespace std;

// void spliting(string arr[], char arr1[], string propirity) 
// {
//     int count = 0;
//     int count1 = 0;
//     for (int i = 0; i < propirity.length(); i++)
//     {
//         if (propirity[i] != '&')
//         {
//             arr[count] = propirity[i];
//             count++;
//         }
//         else
//         {
//             arr1[count1] = propirity[i]; 
//             count1++;
//         }
//     }
// }

// int main()
// {
//     string arr[5]={" "," "," "," "," "};
//     char arr1[1];

//     string trail1 = "i1&i2";

//     spliting(arr, arr1, trail1);

//     for (int i = 0; i < 5; i++)
//     {
//         cout << arr[i];
//     }

//     for (int i = 0; i < 1; i++) 
//     {
//         cout << arr1[i];
//     }

//     return 0; 
// }
#include<iostream>
#include<string>
#include<vector>
using namespace std;

void spliting(vector<string>& inputs, vector<char>& gatesExpressions, const string& outputexpression) 
{
    string tempinput;
   
    for (int i = 0; i < outputexpression.length(); i++)
    {
        if (outputexpression[i] == '&')
        {
            gatesExpressions.push_back(outputexpression[i]);  
           
        }
        else if(outputexpression[i] == '|')
        {
            gatesExpressions.push_back(outputexpression[i]); 
        }
        else if(outputexpression[i] == '~')
        {
            gatesExpressions.push_back(outputexpression[i]);   
        }
        else
        {
            tempinput += outputexpression[i]; 
        }
    }
    inputs.push_back(tempinput);
}

int main()
{
    vector<string> inputs; 
    vector<char> gatesExpressions; 

    string outputExpressions = "i1&i2";

    spliting(inputs, gatesExpressions, outputExpressions);

    for (int i = 0; i < inputs.size(); i++)
    {
        cout << inputs[i];
    }

    for (int i = 0; i < gatesExpressions.size(); i++) 
    {
        cout << gatesExpressions[i];
    }

    return 0; 
}
