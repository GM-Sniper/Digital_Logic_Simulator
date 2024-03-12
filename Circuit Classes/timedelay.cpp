#include<iostream>
#include <chrono>
#include <thread>

using namespace std; 




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

int A=0;
timedelay(A,1);







    return 0;
}