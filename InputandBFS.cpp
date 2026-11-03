#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include <set>
#include <vector>

using namespace std;
using Graph = multimap<string, string>;

void breadthFirstSearch(const Graph& graph, const string& startNode, const string& targetNode) {
    queue<string> q;
    set<string> visited;
    map<string, string> tracker;
    bool found = false;

    // Initializing
    q.push(startNode);
    visited.insert(startNode);

    while (!q.empty()) {
        string current = q.front();
        q.pop();
        
        //Checking for the targetNode
        if(current == targetNode){
            found = true;
            break;
        }

        //Get all neighbors of the current node
        auto range = graph.equal_range(current);
        for (auto it = range.first; it != range.second; ++it) {
            string neighbor = it->second;
            
            //Checking if neighbor has been visited
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                tracker[neighbor] = current;
                q.push(neighbor);
            }
        }
    }
    
    cout << "Shortest path from " << startNode << " to " << targetNode << " is: ";
    
    if(!found){
        //error checking
        cout << "There is no valid path between the two locations.";
    }else{
        //reconstructing the BFS path to only include relevant nodes
        vector<string> path;
        string pathTrack = targetNode;
        while(pathTrack != startNode){
            path.push_back(pathTrack);
            pathTrack = tracker[pathTrack];
        }
        //printing the found path
        path.push_back(startNode);
        for(int i = path.size() - 1; i >= 0; i--){
            cout << path[i];
            if(i!=0){
                cout << " -> ";
            }
        }
    }
    cout << endl << endl;
    return;
}

int main()
{

    multimap<string, string> airlineMM;
    ifstream inputFile("airlineFlights.txt");
    
    //error checking
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return 1;
    }
    
    string line;
    string key;
    int tracker = 0;
    
    //Organizing airline data into a multimap
    while(inputFile >> line){
        /*commented for testbench purposes
        cout << line << endl; */
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
    /*Set to comment for testbench purposes
    cout << "Airline Multimap: " << endl;
    
    
    auto it = airlineMM.begin();
    while (it != airlineMM.end()) {
        const string& startKey = it->first;
        cout << startKey << ": ";

        while (it != airlineMM.end() && it->first == startKey) {
            cout << it->second << " ";
            ++it;
        }
        cout << '\n';
    }
    
    */ 
    breadthFirstSearch(airlineMM, "AirlineOne", "AirlineTwo");
    //expected: AirlineOne -> AirlineTwo
    breadthFirstSearch(airlineMM, "AirlineTwo", "AirlineOne");
    //expected: AirlineTwo -> AirlineThree -> AirlineOne
    breadthFirstSearch(airlineMM, "AirlineThree", "AirlineSix");
    //expected: AirlineThree -> AirlineSix
    breadthFirstSearch(airlineMM, "AirlineFour", "AirlineFour");
    //expected: AirlineFour
    breadthFirstSearch(airlineMM, "AirlineFive", "AirlineThree");
    //expected: No valid path

    return 0;
}
