/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <string>
#include <set>
#include <vector>

using namespace std;

using Graph = multimap<string, string>;

// Check whether a direct one-way flight exists
bool hasFlight(const Graph& graph, const string& from,
               const string& to) {

    auto range = graph.equal_range(from);

    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == to) {
            return true;
        }
    }

    return false;
}

// DFS with backtracking to find the best round trip
void findBestRoute(
    const Graph& graph,
    const string& current,
    const string& start,
    set<string>& visited,
    vector<string>& currentPath,
    vector<string>& bestPath
) {

    // Check whether we can return to the starting location
    if (hasFlight(graph, current, start)) {

        vector<string> completedPath = currentPath;
        completedPath.push_back(start);

        // Prefer visiting more unique locations.
        // If tied, prefer fewer flights.
        if (bestPath.empty() ||
            completedPath.size() > bestPath.size() ||
            (completedPath.size() == bestPath.size() &&
             completedPath.size() < bestPath.size())) {

            bestPath = completedPath;
        }
    }

    // Explore all outgoing flights
    auto range = graph.equal_range(current);

    for (auto it = range.first; it != range.second; ++it) {

        string neighbor = it->second;

        // Do not revisit a location
        if (visited.find(neighbor) == visited.end()) {

            visited.insert(neighbor);
            currentPath.push_back(neighbor);

            findBestRoute(
                graph,
                neighbor,
                start,
                visited,
                currentPath,
                bestPath
            );

            // Backtracking
            currentPath.pop_back();
            visited.erase(neighbor);
        }
    }
}

int main() {

    Graph airlineMM;
    set<string> allLocations;

    ifstream inputFile("airlineFlightsDFS.txt");

    if (!inputFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return 1;
    }

    string line;

    // Read airline data into the multimap
    while (getline(inputFile, line)) {

        stringstream ss(line);
        string key;
        string destination;

        if (!(ss >> key)) {
            continue;
        }

        allLocations.insert(key);

        while (ss >> destination) {

            if (destination == "0") {
                break;
            }

            airlineMM.insert({key, destination});
            allLocations.insert(destination);
        }
    }

    inputFile.close();
    
    string start = "DA";
    cin >> start;

    if (allLocations.find(start) == allLocations.end()) {
        cout << "Starting location does not exist." << endl;
        return 1;
    }

    set<string> visited;
    vector<string> currentPath;
    vector<string> bestPath;

    visited.insert(start);
    currentPath.push_back(start);

    findBestRoute(
        airlineMM,
        start,
        start,
        visited,
        currentPath,
        bestPath
    );

    // Print results
    if (bestPath.empty()) {

        cout << "No valid round trip exists." << endl;

    }
    else {

        cout << "Best round trip: ";

        for (int i = 0; i < bestPath.size(); i++) {
            cout << bestPath[i];

            if (i != bestPath.size() - 1) {
                cout << " -> ";
            }
        }

        cout << endl;

        cout << "Destinations visited: "
             << bestPath.size() - 1 << endl;

        cout << "Total flights: "
             << bestPath.size() - 1 << endl;
    }

    return 0;
}

