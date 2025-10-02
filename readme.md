# Smart Traffic Management System

A hands-on project aimed at simulating real-world urban traffic management. 
It leverages data structures such as graphs, heaps, and priority queues to optimize traffic flow. 
The system provides real-time vehicle routing, traffic signal control, congestion management, and emergency handling.

## Key Features:

1.	City Traffic Network:
Represents the city as a weighted, directed graph with intersections as nodes and roads as edges. 
Supports dynamic modification of roads and intersections and provides a text-based or graphical visualization of the network.

2.	Vehicle Routing System
Calculates the shortest or fastest routes for vehicles using Dijkstra’s Algorithm. 
Adapts to changing traffic conditions and tracks vehicle movements across the network.

3.	Traffic Signal Management
Manages traffic signals to minimize congestion by prioritizing roads based on vehicle density and dynamically adjusting green signal durations. 
Includes an emergency override system for critical situations.

4.	Congestion Monitoring
Tracks vehicle counts on roads using a hash table and identifies congested areas for rerouting traffic. 
Provides visual displays of congestion levels for analysis.

5.	Emergency Vehicle Handling
Ensures minimal delays for emergency vehicles by overriding normal signal operations and calculating the fastest routes. 
Restores regular flow once emergencies are resolved.

6.	Accident and Road Closure Simulation
Simulates road disruptions and updates the traffic network dynamically. Recalculates affected routes and monitors system performance during disruptions.

7. 	Simulation Dashboard
Offrs an interactive interface to visualize traffic flow, congestion, and signal statuses. 
Allows manual vehicle management and generates logs of system actions, including rerouting and signal changes.

## Code Execution Instructions:

1. Download the .csv files attached to this repository.
2. Make sure the "FILE_PATHS" array, within .cpp code, stores the correct locations for your .csv files.
3. Compile and run the .cpp file.

## Demo (Screenshots):

- Simulation Dashboard:
![Simulation Dashboard](image.png)

1. Display City Traffic Network:
![Display City Traffic Network](image_1.png)

2. Simulate Traffic Signals:
![Simulate Traffic Signals](image_2.png)

4. Display Blocked Roads:
![Display Blocked Roads](image_3.png)

5. Vehicle Routing Management:
![Vehicle Routing Management](image_4.png)

8. Handle Emergency Vehicle Routing:
![Handle Emergency Vehicle Routing](image_5.png)