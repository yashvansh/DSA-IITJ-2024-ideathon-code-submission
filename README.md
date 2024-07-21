# Parcel Delivery System

This repository contains the code and files for a parcel delivery simulation system. The system simulates the transportation of parcels from source to destination cities using a graph-based approach.

## Files

- `graph.txt`: Contains the data to create the graph of cities and the connections between them. Each row represents an edge between two cities.
- `bookings.txt`: Contains information about the parcels to be transported, including time tick, ID, origin, destination, and priority. Each line represents a parcel.
- `Parcel-system.cpp`: The main working code for the simulation system. It includes the implementation of the Parcel, MaxHeap, FreightCar, Vertex, Graph, and PRC classes.
- `test_case.cpp`: Used to run the test case information fed in `graph.txt` and `bookings.txt`.
- `test_result.txt`: Shows the results of the test cases, indicating whether they have passed or not.


## Test Cases

To run the test cases:

1. Compile the `test_case.cpp` file similarly to the main system.
2. Run the resulting executable. It will use the information provided in `graph.txt` and `bookings.txt` to simulate the parcel delivery.
3. Check the `test_result.txt` file to see the outcomes of the test cases.

## Team Members

- Siddhartha Singh (B22BB041)
- G Dileep (B22CH011)
- Yash Kumar (B22MT047)
- Ishaan Pandey (B22CI017)
