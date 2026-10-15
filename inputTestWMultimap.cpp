#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

int main()
{

    multimap<string, string> airlineMM;

    ifstream inputFile("airlineFlights.txt");
    
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return 1;
    }
    
    string line;
    string key;
    int tracker = 0;
    
    while(inputFile >> line){
        cout << line << endl;
        if(line == "0"){
            tracker--;
        }else{
            if(tracker==0){
                key = line;
                tracker++;
            }else{
                airlineMM.insert({key, line});
            }
        }
    }

    // Printing the keys and their respective values
    cout << "Airline Multimap: " << endl;
    
    
    auto it = airlineMM.begin();
    while (it != airlineMM.end()) {
        const string& current_key = it->first;
        cout << current_key << ": ";

        // Inner loop consumes all values belonging to current_key
        while (it != airlineMM.end() && it->first == current_key) {
            cout << it->second << " ";
            ++it;
        }
        cout << '\n';
    }

    return 0;
}
