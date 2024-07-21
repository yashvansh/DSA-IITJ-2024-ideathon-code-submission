#include "Parcel-system.cpp"
#include <cassert>
#include <iostream>
#include <fstream>

int main() {
    // create a PRC object
    PRC prc(2, 2);
    // create a graph
    prc.create_graph("graph.txt");
    prc.process_parcels("booking.txt");
    prc.run_simulation(20);
    std::ofstream resultFile("test_results.txt", std::ios::app);

    if (prc.time_tick < 20){
        resultFile << "Test Case: Passed\n";
    }
    else{
        resultFile << "Test Case: Failed\n";
    }

    return 0;
}
