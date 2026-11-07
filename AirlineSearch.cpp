#include <string>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <fstream>
#include <deque>

using namespace std;

using Graph = multimap<string, string>;

deque<string> breadthFirstSearch(const Graph& graph, const std::string& startNode, const std::string& targetNode) {
    queue<string> q;
    set<string> visited;
    map<string, string> tracker;
    bool found = false;

    // Initializing
    q.push(startNode);
    visited.insert(startNode);

    //loop to cycle through all the neighbors and extended neighbors of the startNode
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
    
    //vector<string> path;
    deque<string> pathDeque;
    if(!found){
        //error checking
        //pathDeque.push_back("There is no valid path between the two locations.");
        cout << "There is no valid path between the two locations.";
    }else{
        //reconstructing the BFS path to only include relevant nodes
        //vector<string> path;
        string pathTrack = targetNode;
        while(pathTrack != startNode){
            //path.push_back(pathTrack);
            pathDeque.push_front(pathTrack);
            pathTrack = tracker[pathTrack];
        }
        //printing the found path
        pathDeque.push_front(startNode);
        for (int i = 0; i < pathDeque.size(); i++) {
            cout << pathDeque[i];
            if (i != pathDeque.size() - 1) {
                cout << " -> ";
            }
        }
    }
    cout << endl << endl;
    return pathDeque;
}

// Find the route with the shortest number of connections from location A to location D, 
// through locations B and C (B and C in any order)
void smallestRouteWithTwoStops(const Graph& graph, const std::string& A, const std::string& B, const std::string& C, const std::string& D) {
    // First, we find the subpaths (A to B/A to C, B to C/C to B, B to D/C to D).
    // At each of these steps, if neither has a route, we can conclude that no valid path exists.

    // Find the shortest path from A to B and A to C. These should both be exclusive of D, not yet implemented
    deque<string> AtoB = breadthFirstSearch(graph, A, B);
    deque<string> AtoC = breadthFirstSearch(graph, A, C);
    // If there is no path from A to either B or C, no suitable route exists.
    if(AtoB.empty() && AtoC.empty()){
        cout << "No valid path from " << A << " to " << D << " through " << B << " and " << C << "." << endl;
        return;
    }

    // Find shortest path from B to C and C to B. These should both be exclusive of A & D, not yet implemented
    deque<string> BtoC = breadthFirstSearch(graph, B, C);
    deque<string> CtoB = breadthFirstSearch(graph, C, B);
    // If there is no path between B and C, no suitable route exists.
    if(BtoC.empty() && CtoB.empty()){
        cout << "No valid path from " << A << " to " << D << " through " << B << " and " << C << "." << endl;
        return;
    }

    // Find shortest path from B to D. Should be exclusive of A and C, not yet implemented
    deque<string> BtoD = breadthFirstSearch(graph, B, D);
    // Find shortest path from C to D. Should be exclusive of A and B, not yet implemented
    deque<string> CtoD = breadthFirstSearch(graph, C, D);
    // If there is no path from B to D and C to D, no suitable route exists.
    if(BtoD.empty() && CtoD.empty()){
        cout << "No valid path from " << A << " to " << D << " through " << B << " and " << C << "." << endl;
        return;
    }
}

int main()
{

    multimap<string, string> airlineMM;
    ifstream inputFile("airlineFlightsBFS.txt");
    
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