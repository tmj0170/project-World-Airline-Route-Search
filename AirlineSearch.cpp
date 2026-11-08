#include <string>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <fstream>
#include <deque>

using namespace std;

using Graph = multimap<string, string>;

deque<string> breadthFirstSearch(const Graph& graph, const std::string& startNode, const std::string& targetNode, const set<string>& excludedNodes = {}) {
    queue<string> q;
    set<string> visited;
    map<string, string> tracker;
    bool found = false;

    // Initializing
    q.push(startNode);
    visited.insert(startNode);
    visited.insert(excludedNodes.begin(), excludedNodes.end());

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
    // This keeps track of the route so far, preventing additional work
    deque<string> completeRoute;

    // First, we find the subpaths (A to B/A to C, B to C/C to B, B to D/C to D).
    // At each of these steps, if neither has a route, we can conclude that no valid path exists.

    // Find the shortest path from A to B. Should be exclusive of C and D, not yet implemented
    set<string> excludedNodesAtoB = {C, D};
    deque<string> AtoB = breadthFirstSearch(graph, A, B, excludedNodesAtoB);
    // Find the shortest path from A to C. Should be exclusive of B and D, not yet implemented
    set<string> excludedNodesAtoC = {B, D};
    deque<string> AtoC = breadthFirstSearch(graph, A, C, excludedNodesAtoC);
    if (AtoC.empty()) {
        cout << "No valid path from " << A << " to " << C << "." << endl;
    }
    // If there is no path from A to either B or C, no suitable route exists.
    if(AtoB.empty() && AtoC.empty()){
        cout << "No valid path from " << A << " to " << D << " through " << B << " and " << C << "." << endl;
        return;
    }

    // Find shortest path from B to C and C to B. These should both be exclusive of A & D, not yet implemented
    set<string> excludedNodesBtoC = {A, D};
    deque<string> BtoC = breadthFirstSearch(graph, B, C, excludedNodesBtoC);
    if (BtoC.empty()) {
        cout << "No valid path from " << B << " to " << C << "." << endl;
    }
    deque<string> CtoB = breadthFirstSearch(graph, C, B, excludedNodesBtoC);
    if (AtoB.empty()) {
        cout << "No valid path from " << A << " to " << B << "." << endl;
    }
    // After finding the second pair of subpaths, there are more conditions we can check to rule out suitable paths
    // If there is no path between B and C, in either direction, no suitable route exists.
    // Unlike from A to B or A to C, since the graph is directed, it doesn't need to be possible to travel from B to C and from C to B. 
    if(BtoC.empty() && CtoB.empty()){
        cout << "No valid path from " << A << " to " << D << " through " << B << " and " << C << "." << endl;
        return;
    }
    // If there is no path from A to B (exclusive of C), the path has to go through C first. 
    //If there is also no path from C to B, no suitable route exists.
    if(AtoB.empty() && CtoB.empty()){
        cout << "No valid path from " << A << " to " << D << " through " << B << " and " << C << "." << endl;
        return;
    }
    // If there is no path from A to C (exclusive of B), the path has to go through B first.
    // If there is also no path from  B to C, no suitable route exists.
    if(AtoC.empty() && BtoC.empty()){
        cout << "No valid path from " << A << " to " << D << " through " << B << " and " << C << "." << endl;
        return;
    }

    // Find shortest path from B to D. Should be exclusive of A and C, not yet implemented
    set<string> excludedNodesBtoD = {A, C};
    deque<string> BtoD = breadthFirstSearch(graph, B, D, excludedNodesBtoD);
    // Find shortest path from C to D. Should be exclusive of A and B, not yet implemented
    set<string> excludedNodesCtoD = {A, B};
    deque<string> CtoD = breadthFirstSearch(graph, C, D, excludedNodesCtoD);
    // If there is no path from B to D and C to D, no suitable route exists.
    if(BtoD.empty() && CtoD.empty()){
        cout << "No valid path from " << A << " to " << D << " through " << B << " and " << C << "." << endl;
        return;
    }
    // If there is a path from A to C, but no path from B to C and B to D, no suitable route exists.
    if(BtoC.empty() && BtoD.empty()){
        cout << "No valid path from " << A << " to " << D << " through " << B << " and " << C << "." << endl;
        return;
    }

    // At this point, we have all the subpaths. We can now combine them to form the complete route.
    // Combine the subpaths to form the complete route
    if ((AtoB.size() + BtoC.size() + CtoD.size()) < AtoC.size() + CtoB.size() + BtoD.size()){
        completeRoute.insert(completeRoute.end(), AtoB.begin(), AtoB.end()-1);
        completeRoute.insert(completeRoute.end(), BtoC.begin(), BtoC.end()-1);
        completeRoute.insert(completeRoute.end(), CtoD.begin(), CtoD.end());
    }else{
        completeRoute.insert(completeRoute.end(), AtoC.begin(), AtoC.end()-1);
        completeRoute.insert(completeRoute.end(), CtoB.begin(), CtoB.end()-1);
        completeRoute.insert(completeRoute.end(), BtoD.begin(), BtoD.end());
    }
    //if(!AtoB.empty() && !BtoC.empty() && !CtoD.empty()){
    //    completeRoute.insert(completeRoute.end(), AtoB.begin(), AtoB.end());
    //    completeRoute.insert(completeRoute.end(), BtoC.begin(), BtoC.end());
    //    completeRoute.insert(completeRoute.end(), CtoD.begin(), CtoD.end());
    //}else if(!AtoC.empty() && !CtoB.empty() && !BtoD.empty()){
    //    completeRoute.insert(completeRoute.end(), AtoC.begin(), AtoC.end());
    //    completeRoute.insert(completeRoute.end(), CtoB.begin(), CtoB.end());
    //    completeRoute.insert(completeRoute.end(), BtoD.begin(), BtoD.end());
    //}

    // Print the complete route
    if(!completeRoute.empty()){
        cout << "Shortest route from " << A << " to " << D << " through " << B << " and " << C << ": ";
        for(const auto& node : completeRoute){
            cout << node << " ";
        }
        cout << endl;
    }else{
        cout << "No valid path from " << A << " to " << D << " through " << B << " and " << C << "." << endl;
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
    //Set to comment for testbench purposes
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
    
     
    //breadthFirstSearch(airlineMM, "AirlineOne", "AirlineTwo");
    //expected: AirlineOne -> AirlineTwo
    //breadthFirstSearch(airlineMM, "AirlineTwo", "AirlineOne");
    //expected: AirlineTwo -> AirlineThree -> AirlineOne
    //breadthFirstSearch(airlineMM, "AirlineThree", "AirlineSix");
    //expected: AirlineThree -> AirlineSix
    //breadthFirstSearch(airlineMM, "AirlineFour", "AirlineFour");
    //expected: AirlineFour
    //breadthFirstSearch(airlineMM, "AirlineFive", "AirlineThree");
    //expected: No valid path

    smallestRouteWithTwoStops(airlineMM, "AirlineOne", "AirlineTwo", "AirlineThree", "AirlineFour");
    //expected: AirlineOne -> AirlineTwo -> AirlineThree -> AirlineFour

    return 0;
}