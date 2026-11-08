# project-World-Airline-Route-Search
Madison Ulmer, Oishani Chatterji, Todd Jolley

This project takes a dataset of multiple locations and destinations to represent possible flights between locations. Each location represents a node and each flight is an edge in a directed graph. This dataset is formatted into a multi-map for our search algorithms. The first algorithm finds the shortest (least amount of layovers) route between two locations using a breadth first search and returns them, or if there is no valid path it returns that the requested path is not possible.
the second makes a round trip at a starting location with the most destinations visited and fewest on the way back using a depth first search and returns the amount visited or if there is no possible round trip returns round trip not possible.
