#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

const int MAX_NODES = 26;  // 26 nodes (A to Z)
const char BASE_CHAR = 'A'; // Base character for node labels
const string FILE_PATHS[] = {
    "emergency_vehicles.csv",
    "road_network.csv",
    "vehicles.csv",
    "road_closures.csv",
    "traffic_signals.csv"
};


const int CONGESTION_THRESHOLD = 20; // Threshold for congestion, can be adjusted

class TrafficManagementSystem {
private:
    struct Edge {
        int destination;
        int weight;
    };

    struct BlockedRoad {
        char from;
        char to;
    };


    struct DynamicArray {
        Edge edges[MAX_NODES];
        int size;

        DynamicArray() : size(0) {}

        void push_back(const Edge& edge) {
            if (size < MAX_NODES) {
                edges[size++] = edge;
            }
        }

        Edge operator[](int index) const {
            return edges[index];
        }

        int getSize() const {
            return size;
        }
    };

    struct Graph {
        DynamicArray adjacencyList[MAX_NODES];

        void addEdge(int src, int dest, int weight) {
            adjacencyList[src].push_back({ dest, weight });
        }

        void removeEdge(int src, int dest) {
            for (int i = 0; i < adjacencyList[src].getSize(); ++i) {
                if (adjacencyList[src][i].destination == dest) {
                    // Shift all elements after the one being removed
                    for (int j = i; j < adjacencyList[src].getSize() - 1; ++j) {
                        adjacencyList[src].edges[j] = adjacencyList[src].edges[j + 1];
                    }
                    adjacencyList[src].size--;
                    break;
                }
            }
        }

        const DynamicArray& getEdges(int node) const {
            return adjacencyList[node];
        }
    };

    struct Vehicle {
        string id;
        int currentNode;  // The current node (intersection) of the vehicle
        int destinationNode; // Destination intersection
    };


    Vehicle vehicles[150];  // Array to store vehicles
    int vehicleCount = 0;  // To track the number of vehicles in the array

    void loadVehicleDataFromFile(const string& filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cout << "Error: Cannot open file: " << filePath << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string startInter, endInter, vehicleID;
            char comma;

            // Skipping header
            if (line.find("VehicleID") != string::npos)
                continue;

            if (getline(ss, vehicleID, ',') &&
                getline(ss, startInter, ',') &&
                getline(ss, endInter)) {
                // Convert start and end intersections to node indices
                int startNode = startInter[0] - BASE_CHAR;
                int endNode = endInter[0] - BASE_CHAR;

                // Add vehicle to system
                if (vehicleCount < 100) {
                    vehicles[vehicleCount++] = { vehicleID, startNode, endNode };
                }
            }
        }
        file.close();
    }

    void addVehicle(const string& filePath) {
        if (vehicleCount >= 150) {
            cout << "Vehicle limit reached. Cannot add more vehicles.\n";
            return;
        }

        Vehicle newVehicle;
        cout << "Enter Vehicle ID: ";
        cin >> newVehicle.id;

        char currentChar, destinationChar;
        cout << "Enter current location (A-Z): ";
        cin >> currentChar;
        cout << "Enter destination (A-Z): ";
        cin >> destinationChar;

        // Convert letters to numeric indices
        newVehicle.currentNode = currentChar - BASE_CHAR;
        newVehicle.destinationNode = destinationChar - BASE_CHAR;

        // Add vehicle to array
        vehicles[vehicleCount++] = newVehicle;

        // Write to file
        ofstream file(filePath, ios::app); // Open in append mode
        if (file.is_open()) {
            file << newVehicle.id << "," << currentChar << "," << destinationChar << "\n";
            file.close();
            cout << "Vehicle added successfully and saved to file.\n";
        }
        else {
            cout << "Error: Unable to open file for writing.\n";
        }
    }

    void removeVehicle(const string& filePath) {
        string vehicleId;
        cout << "Enter Vehicle ID to remove: ";
        cin >> vehicleId;

        bool found = false;

        // Search for the vehicle and remove it by shifting the remaining vehicles
        for (int i = 0; i < vehicleCount; ++i) {
            if (vehicles[i].id == vehicleId) {
                found = true;

                // Shift all vehicles after the removed vehicle to the left
                for (int j = i; j < vehicleCount - 1; ++j) {
                    vehicles[j] = vehicles[j + 1];
                }

                // Decrease the vehicle count
                --vehicleCount;
                cout << "Vehicle removed successfully.\n";
                break;
            }
        }

        if (!found) {
            cout << "Vehicle ID not found.\n";
            return;
        }

        // Rewrite the file with updated vehicle data
        ofstream file(filePath);
        if (file.is_open()) {
            // Write header
            file << "VehicleID,StartIntersection,EndIntersection\n";

            // Write updated vehicle data
            for (int i = 0; i < vehicleCount; ++i) {
                char currentChar = BASE_CHAR + vehicles[i].currentNode;
                char destinationChar = BASE_CHAR + vehicles[i].destinationNode;
                file << vehicles[i].id << "," << currentChar << "," << destinationChar << "\n";
            }

            file.close();
            cout << "File updated successfully.\n";
        }
        else {
            cout << "Error: Unable to open file for writing.\n";
        }
    }

    // Function to display all vehicles
    void displayVehicles() {
        if (vehicleCount == 0) {
            cout << "No vehicles to display.\n";
            return;
        }

        cout << "\nList of Vehicles:\n";
        for (int i = 0; i < vehicleCount; ++i) {
            cout << "Vehicle ID: " << vehicles[i].id
                << ", Location: " << char(BASE_CHAR + vehicles[i].currentNode)
                << ", Destination: " << char(BASE_CHAR + vehicles[i].destinationNode) << "\n";
        }
    }


    void trackVehicleMovement() {

        for (int i = 0; i < vehicleCount; ++i) {
            Vehicle& vehicle = vehicles[i];
            cout << "Tracking Vehicle ID: " << vehicle.id
                << " Current Node: " << char(BASE_CHAR + vehicle.currentNode)
                << " Destination: " << char(BASE_CHAR + vehicle.destinationNode) << "\n";

            // Simulate vehicle movement here (moving to the next node)
            if (vehicle.currentNode != vehicle.destinationNode) {
                vehicle.currentNode++;  // Simple simulation: just moving the vehicle forward
                if (vehicle.currentNode >= MAX_NODES) vehicle.currentNode = 0; // Loop back to A
            }
            // Update vehicle count for the current node
            vehicleCounts[vehicle.currentNode]++;
        }
    }

    struct TrafficSignal {
        int greenTime;  // Time duration for Green Light
        bool isGreen;  // Traffic signal state: true = Green, false = Red

        TrafficSignal() : isGreen(true), greenTime(30) {}  // Default to Green with 30s duration

        void toggle() {
            isGreen = !isGreen;
            if (isGreen) {
                greenTime = (rand() % 40) + 15;  // Random green time between 15s and 55s
            }
            else {
                greenTime = (rand() % 20) + 10;  // Random red time between 10s and 30s
            }
        }

        void displayStatus(int node) {
            cout << "Intersection " << char(BASE_CHAR + node) << " Green Time: " << greenTime << "s\n";
        }
    };

    Graph graph;
    int vehicleCounts[MAX_NODES]; // 2D array to store vehicle counts for each road
    TrafficSignal signals[MAX_NODES];  // Array to hold traffic signals at each node

    BlockedRoad blockedRoads[100];
    int blockedCount;

    void initializeArray(int* arr, int size, int value) {
        for (int i = 0; i < size; ++i) {
            arr[i] = value;
        }
    }

    void initializeBoolArray(bool* arr, int size, bool value) {
        for (int i = 0; i < size; ++i) {
            arr[i] = value;
        }
    }

    int findMinDistance(int* dist, bool* visited, int nodes) {
        int minDist = 1000000000;  // A large value for infinity
        int minIndex = -1;
        for (int i = 0; i < nodes; ++i) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minIndex = i;
            }
        }
        return minIndex;
    }

    void initializeBlockedRoads() {
        blockedCount = 0;
    }

    void loadBlockedRoadsFromFile(const string& filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cout << "Error: Cannot open file: " << filePath << endl;
            return;
        }
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            char roadFrom, roadTo;
            string status;
            char comma;
            if (ss >> roadFrom >> comma >> roadTo >> comma >> status) {
                if (status == "Blocked" && blockedCount < 100) {
                    blockedRoads[blockedCount++] = { roadFrom, roadTo };
                }
            }
        }
        file.close();
    }


    void blockRoad(char from, char to) {
        // Add the new road only if it's not already blocked
        for (int i = 0; i < blockedCount; ++i) {
            if (blockedRoads[i].from == from && blockedRoads[i].to == to) {
                cout << "Road already blocked.\n";
                return;
            }
        }
        blockedRoads[blockedCount++] = { from, to };
        removeRoad(from - BASE_CHAR, to - BASE_CHAR);
        removeRoad(to - BASE_CHAR, from - BASE_CHAR);
        cout << "Road blocked between " << from << " and " << to << ".\n";
    }

    void unblockRoad(char from, char to) {
        bool roadFound = false;
        for (int i = 0; i < blockedCount; ++i) {
            if (blockedRoads[i].from == from && blockedRoads[i].to == to) {
                roadFound = true;
                // Shift elements to remove the blocked road
                for (int j = i; j < blockedCount - 1; ++j) {
                    blockedRoads[j] = blockedRoads[j + 1];
                }
                blockedCount--;
                // Add the road back to the graph
                addRoad(from - BASE_CHAR, to - BASE_CHAR, 1); // Default weight
                //addRoad(to - BASE_CHAR, from - BASE_CHAR, 1); // Default weight
                cout << "Road unblocked between " << from << " and " << to << ".\n";
                break;
            }
        }
        if (!roadFound) {
            cout << "Road " << from << " to " << to << " is not blocked.\n";
        }
    }


    void showBlockedRoads() const {
        cout << "\nBlocked Roads:\n";
        for (int i = 0; i < blockedCount; i++) {
            cout << blockedRoads[i].from << " -> " << blockedRoads[i].to << "\n";
        }
    }

    void updateVehicleCounts(int src, int dest, int delta) {
        // Update vehicle counts for the road segment (src -> dest)
        for (int i = 0; i < graph.getEdges(src).getSize(); ++i) {
            if (graph.getEdges(src)[i].destination == dest) {
                vehicleCounts[src] += delta;
                break;
            }
        }
    }

    void detectCongestion() {
        cout << "\nCongestion Status\n";
        bool isCongested = false;

        for (int i = 0; i < MAX_NODES; ++i) {
            for (int j = 0; j < graph.getEdges(i).getSize(); ++j) {
                int dest = graph.getEdges(i)[j].destination;
                int count = vehicleCounts[i];
                if (count > CONGESTION_THRESHOLD) {
                    cout << "Road " << char(BASE_CHAR + i) << " -> " << char(BASE_CHAR + dest)
                        << " is congested with " << count << " vehicles.\n";
                    isCongested = true;
                }
            }
        }

        if (!isCongested) {
            cout << "No congestion detected on any road.\n";
        }
    }

    void dijkstra(int src, int dest, bool useTime) {
        int dist[MAX_NODES];
        bool visited[MAX_NODES];
        int prev[MAX_NODES]; // To store the path

        initializeArray(dist, MAX_NODES, 1000000000); // Initialize distances to infinity
        initializeBoolArray(visited, MAX_NODES, false); // Initialize visited nodes
        initializeArray(prev, MAX_NODES, -1); // Initialize previous nodes for path reconstruction

        dist[src] = 0;

        for (int count = 0; count < MAX_NODES - 1; ++count) {
            int u = findMinDistance(dist, visited, MAX_NODES); // Get the nearest unvisited node
            if (u == -1) break; // All reachable nodes are visited
            visited[u] = true;

            // Update distances for neighbors of u
            const DynamicArray& edges = graph.getEdges(u);
            for (int i = 0; i < edges.getSize(); ++i) {
                int v = edges[i].destination;
                int weight = edges[i].weight;

                // Adjust weight based on time or distance
                if (useTime) {
                    weight += vehicleCounts[v] / 10; // Adding delay based on congestion
                }

                if (!visited[v] && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u; // Update path
                }
            }
        }

        if (dist[dest] == 1000000000) {
            cout << "No path found from " << char(BASE_CHAR + src) << " to " << char(BASE_CHAR + dest) << ".\n";
            return;
        }

        // Print the shortest/fastest path
        cout << (useTime ? "Fastest" : "Shortest") << " path from " << char(BASE_CHAR + src) << " to " << char(BASE_CHAR + dest) << ":\n";
        int curr = dest;
        string path = "";
        while (curr != -1) {
            path = char(BASE_CHAR + curr) + (path.empty() ? "" : " -> ") + path;
            curr = prev[curr];
        }
        cout << path << " with total " << (useTime ? "time: " : "distance: ") << dist[dest] << "\n";
    }


    void simulateVehicleTracking() {
        cout << "Simulating vehicle movement...\n";
        for (int i = 0; i < vehicleCount; ++i) {
            Vehicle& vehicle = vehicles[i];

            cout << "Vehicle ID: " << vehicle.id
                << " Current Node: " << char(BASE_CHAR + vehicle.currentNode)
                << " Destination: " << char(BASE_CHAR + vehicle.destinationNode) << "\n";

            // Check if the vehicle has reached its destination
            if (vehicle.currentNode == vehicle.destinationNode) {
                cout << "Vehicle " << vehicle.id << " has reached its destination.\n";
                continue;
            }

            // Recalculate route dynamically if congestion is detected
            if (vehicleCounts[vehicle.currentNode] > CONGESTION_THRESHOLD) {
                cout << "Congestion detected at " << char(BASE_CHAR + vehicle.currentNode) << ". Recalculating route...\n";
                dijkstra(vehicle.currentNode, vehicle.destinationNode, true); // Use fastest route
            }

            // Update vehicle counts for current movement
            int nextNode = (vehicle.currentNode + 1) % MAX_NODES; // Simplified next node
            updateVehicleCounts(vehicle.currentNode, nextNode, -1); // Decrement count at current node
            vehicle.currentNode = nextNode;
            updateVehicleCounts(vehicle.currentNode, nextNode, 1); // Increment count at the new node

            cout << "Vehicle " << vehicle.id << " moved to " << char(BASE_CHAR + vehicle.currentNode) << ".\n";
        }
    }


    // Queue handling logic (basic queue class logic for managing queue)
    struct Queue {
        int front, rear, size;
        int* arr;
        Queue(int capacity) : front(0), rear(-1), size(0) {
            arr = new int[capacity];
        }

        ~Queue() {
            delete[] arr;
        }

        void enqueue(int value) {
            if (size < MAX_NODES) {
                rear = (rear + 1) % MAX_NODES;
                arr[rear] = value;
                ++size;
            }
        }

        int dequeue() {
            if (size == 0) {
                throw runtime_error("Queue underflow");
            }
            int value = arr[front];
            front = (front + 1) % MAX_NODES;
            --size;
            return value;
        }

        bool isEmpty() {
            return size == 0;
        }

        int getFront() {
            if (isEmpty()) {
                throw runtime_error("Queue is empty");
            }
            return arr[front];
        }

        int getSize() {
            return size;
        }
    };

    // Method to load traffic signals data from file
    void loadTrafficSignalsFromFile(const string& filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cout << "Error: Cannot open file: " << filePath << endl;
            return;
        }
        int count = 0;
        string line;
        getline(file, line); // Skip the header
        while (getline(file, line)) {
            stringstream ss(line);
            char intersection;
            int greenTime;
            char comma;
            if (ss >> intersection >> comma >> greenTime) {
                int index = intersection - BASE_CHAR; // Convert char to index (A -> 0, B -> 1, etc.)
                signals[index].greenTime = greenTime;
            }
        }
        file.close();
    }

public:
    void addRoad(int src, int dest, int weight) {
        graph.addEdge(src, dest, weight);
    }

    void removeRoad(int src, int dest) {
        graph.removeEdge(src, dest);
        graph.removeEdge(dest, src);  // Assuming bidirectional roads
    }

    void loadRoadNetworkFromFile(const string& filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cout << "Error: Cannot open file: " << filePath << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            char src, dest;
            int weight;
            char comma;
            if (ss >> src >> comma >> dest >> comma >> weight) {
                addRoad(src - BASE_CHAR, dest - BASE_CHAR, weight);
                addRoad(dest - BASE_CHAR, src - BASE_CHAR, weight); // Bidirectional graph
            }
            else {
                cout << "Error parsing road network line: " << line << endl;
            }
        }
        file.close();
    }


    void loadAllData() {
        loadVehicleDataFromFile(FILE_PATHS[2]); // Load vehicle data
    }
    void displayGraph() {
        cout << "City Traffic Network:\n";
        for (int i = 0; i < MAX_NODES; ++i) {
            if (graph.getEdges(i).getSize() == 0) continue;
            cout << char(BASE_CHAR + i) << " -> ";
            for (int j = 0; j < graph.getEdges(i).getSize(); ++j) {
                Edge edge = graph.getEdges(i)[j];
                cout << "(" << char(BASE_CHAR + edge.destination) << ", " << edge.weight << ") ";
            }
            cout << "\n";
        }
    }


    // Method to recalculate routes dynamically based on congestion and traffic conditions
    void recalculateRoutes() {
        cout << "Recalculating routes dynamically...\n";
        loadVehicleDataFromFile(FILE_PATHS[2]);  // Reload vehicle data to account for changes
        loadBlockedRoadsFromFile(FILE_PATHS[3]); // Reload blocked roads
        detectCongestion();  // Detect congested areas
    }

    void simulateEmergencyVehicleRouting() {
        cout << "Enter start and end intersections for emergency vehicle: ";
        char startChar, endChar;
        cin >> startChar >> endChar;

        int start = startChar - BASE_CHAR;
        int end = endChar - BASE_CHAR;

        cout << "Emergency Vehicle is being routed...\n";

        // Call Dijkstra's Algorithm to find the shortest path
        string path = findShortestPath(start, end);
        cout << "Emergency Vehicle path: " << path << "\n";

        // Simulate clearing traffic signals for the route
        clearTrafficForEmergency(path);

        // Restore normal traffic flow
        cout << "Normal traffic flow restored.\n";
    }

    string findShortestPath(int start, int end) {
        int dist[MAX_NODES];
        int prev[MAX_NODES];
        bool visited[MAX_NODES];

        initializeArray(dist, MAX_NODES, 1000000000); // Initialize distances to infinity
        initializeArray(prev, MAX_NODES, -1);        // Initialize previous node tracker
        initializeBoolArray(visited, MAX_NODES, false);

        dist[start] = 0;

        for (int i = 0; i < MAX_NODES; ++i) {
            int u = findMinDistance(dist, visited, MAX_NODES);
            if (u == -1) break; // No more reachable nodes
            visited[u] = true;

            for (int j = 0; j < graph.getEdges(u).getSize(); ++j) {
                Edge edge = graph.getEdges(u)[j];
                int v = edge.destination;
                int weight = edge.weight;

                if (!visited[v] && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                }
            }
        }

        // Construct the path from start to end
        string path = "";
        for (int at = end; at != -1; at = prev[at]) {
            path = char(BASE_CHAR + at) + path;
            if (at != start) path = " " + path;
        }
        return path;
    }

    void clearTrafficForEmergency(const string& path) {
        cout << "Clearing traffic signals along the path: " << path << "\n";
        for (char c : path) {
            if (c != ' ') {
                int node = c - BASE_CHAR;
                signals[node].isGreen = true; // Set all signals to green
            }
        }
    }



    void simulateDashboard() {
        initializeBlockedRoads();
        loadRoadNetworkFromFile(FILE_PATHS[1]); // Load road network once at the start
        loadAllData();
        loadBlockedRoadsFromFile(FILE_PATHS[3]);

        while (true) {
            cout << "\nSimulation Dashboard\n";
            cout << "1. Display City Traffic Network\n";
            cout << "2. Simulate Traffic Signals\n";
            cout << "3. Detect Congestion\n";
            cout << "4. Display Blocked Roads\n";
            cout << "5. Vehicle Routing Management\n";
            cout << "6. Block a Road\n";
            cout << "7. Unblock a Road\n";
            cout << "8. Handle Emergency Vehicle Routing\n";
            cout << "9. Exit Simulation\n";

            cout << "Enter your choice: ";
            int choice;
            cout << endl;

            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Please enter a valid choice.\n";
                continue;
            }

            switch (choice) {
            case 1: {
                // Display and manage graph, allow adding/removing roads
                displayGraph();  // Display the current graph
                while (true) {
                    cout << "\nCity Traffic Network Options\n";
                    cout << "1. Add Road\n";
                    cout << "2. Remove Road\n";
                    cout << "3. Back to Main Menu\n";
                    cout << "Enter your choice: ";
                    int subChoice;
                    cin >> subChoice;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid input. Please enter a valid choice.\n";
                        continue;
                    }

                    switch (subChoice) {
                    case 1: {  // Add a new road
                        char from, to;
                        int weight;
                        cout << "Enter source, destination, and weight: ";
                        cin >> from >> to >> weight;
                        addRoad(from - BASE_CHAR, to - BASE_CHAR, weight);
                        cout << "Road added successfully from " << from << " to " << to << " with weight " << weight << ".\n\n";
                        break;
                    }
                    case 2: {  // Remove an existing road
                        char from, to;
                        cout << "Enter source, and destination: ";
                        cin >> from >> to;
                        removeRoad(from - BASE_CHAR, to - BASE_CHAR);
                        cout << "Road removed successfully between " << from << " and " << to << ".\n\n";
                        break;
                    }
                    case 3:
                        cout << "Returning to main menu...\n";
                        break;
                    default:
                        cout << "Invalid choice. Please enter a valid option.\n";
                        break;
                    }

                    if (subChoice == 3) break;  // Exit sub-menu and return to main menu
                }
                break;  // Exit case 1 loop
            }
            case 2:
                loadTrafficSignalsFromFile(FILE_PATHS[4]); // Load traffic signals data
                for (int i = 0; i < MAX_NODES; ++i) {
                    signals[i].displayStatus(i);
                }
                break;
            case 3:

                detectCongestion();  // Check for congestion

                break;
            case 4:
                showBlockedRoads();
                break;

            case 5: { // Vehicle Routing System
                while (true) {
                    cout << "\nVehicle Routing System\n";
                    cout << "1. Display all Vehicle\n";
                    cout << "2. Add a Vehicle\n";
                    cout << "3. Remove a Vehicle\n";
                    cout << "4. Calculate Shortest/Fastest Route\n";
                    cout << "5. Recalculate Routes Dynamically\n";
                    cout << "6. Track Vehicle Movement\n";
                    cout << "7. Back to Main Menu\n";
                    cout << "Enter your choice: ";
                    int subChoice;
                    cin >> subChoice;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid input. Please enter a valid choice.\n";
                        continue;
                    }

                    switch (subChoice) {
                    case 1:
                        displayVehicles();
                        break;
                    case 2:
                        addVehicle("vehicles.csv"); // Dynamically recalculate routes
                        break;
                    case 3:
                        removeVehicle("vehicles.csv"); // Dynamically recalculate routes
                        break;
                    case 4: {
                        char src, dest;
                        cout << "Enter source and destination intersections (A-Z): ";
                        cin >> src >> dest;

                        cout << "Calculate by:\n";
                        cout << "1. Shortest Distance\n";
                        cout << "2. Fastest Time\n";
                        cout << "Enter your choice: ";
                        int calcType;
                        cin >> calcType;

                        if (calcType == 1) {
                            dijkstra(src - BASE_CHAR, dest - BASE_CHAR, false); // Shortest path
                        }
                        else if (calcType == 2) {
                            dijkstra(src - BASE_CHAR, dest - BASE_CHAR, true); // Fastest route
                        }
                        else {
                            cout << "Invalid choice.\n";
                        }
                        break;
                    }
                    case 5:
                        recalculateRoutes(); // Dynamically recalculate routes
                        break;
                    case 6:
                        simulateVehicleTracking(); // Track vehicle movement
                        break;
                    case 7:
                        cout << "Returning to main menu...\n";
                        break;
                    default:
                        cout << "Invalid choice. Please enter a valid option.\n";
                        break;
                    }

                    if (subChoice == 7) break;
                }
                break;
            }
            case 6: {
                char from, to;
                cout << "Enter the road to block (from to): ";
                cin >> from >> to;
                blockRoad(from, to);
                break;
            }
            case 7: {
                char from, to;
                cout << "Enter the road to unblock (from to): ";
                cin >> from >> to;
                unblockRoad(from, to);
                break;
            }

            case 8:
                simulateEmergencyVehicleRouting();
                break;

            case 9:
                cout << "Exiting Simulation...\n";
                return;

            default:
                cout << "Invalid choice. Please enter a valid option.\n";
                break;
            }
        }
    }


};

int main() {
    TrafficManagementSystem system;
    system.simulateDashboard();
    return 0;
}