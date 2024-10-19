#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>  // For output formatting

using namespace std;

// Structure to represent a city
struct City {
    int id;
    double x, y;
};

// Function to calculate Euclidean distance between two cities
double euclideanDistance(const City& a, const City& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Function to calculate the total tour cost
double calculateTotalCost(const vector<City>& cities, const vector<int>& tour) {
    double cost = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        cost += euclideanDistance(cities[tour[i]], cities[tour[i + 1]]);
    }
    cost += euclideanDistance(cities[tour.back()], cities[tour[0]]); // Return to start
    return cost;
}

// Function to generate a neighbor solution by swapping two cities
void swapCities(vector<int>& tour) {
    int a = rand() % tour.size();
    int b = rand() % tour.size();
    swap(tour[a], tour[b]);
}

// Simulated annealing algorithm for TSP
vector<int> simulatedAnnealing(const vector<City>& cities, int iterations, double initialTemp, double finalTemp) {
    int n = cities.size();
    vector<int> bestTour(n), currentTour(n);
    
    // Initialize the tour with a random permutation of cities
    for (int i = 0; i < n; ++i) {
        currentTour[i] = i;
    }
    random_shuffle(currentTour.begin(), currentTour.end());
    bestTour = currentTour;

    double bestCost = calculateTotalCost(cities, bestTour);
    double currentCost = bestCost;
    double temp = initialTemp;

    for (int iter = 0; iter < iterations; ++iter) {
        // Generate a neighboring solution
        vector<int> newTour = currentTour;
        swapCities(newTour);

        double newCost = calculateTotalCost(cities, newTour);

        // Accept the new solution based on cost and temperature
        if (newCost < currentCost || exp((currentCost - newCost) / temp) > (rand() / (double)RAND_MAX)) {
            currentTour = newTour;
            currentCost = newCost;

            // Update the best solution
            if (newCost < bestCost) {
                bestTour = newTour;
                bestCost = newCost;
            }
        }

        // Gradually decrease the temperature
        temp = initialTemp * pow((finalTemp / initialTemp), (double)iter / iterations);
    }

    cout << "\n\n====================================================\n";
    cout << "           FINAL RESULTS OF SIMULATED ANNEALING     \n";
    cout << "====================================================\n";
    cout << "Best Cost Achieved: " << bestCost << endl;
    cout << "====================================================\n";
    return bestTour;
}

// Function to parse the TSP file and extract city coordinates
vector<City> parseTSPFile(const string& filename) {
    ifstream file(filename);
    vector<City> cities;
    string line;

    // Skip lines until we find the node data
    while (getline(file, line)) {
        if (line == "NODE_COORD_SECTION") {
            break;
        }
    }

    // Read city data
    while (getline(file, line)) {
        if (line == "EOF") break;
        stringstream ss(line);
        City city;
        ss >> city.id >> city.x >> city.y;
        cities.push_back(city);
    }

    return cities;
}

// Function to print the best tour in a well-formatted way
void printTour(const vector<City>& cities, const vector<int>& tour) {
    cout << "\n====================================================\n";
    cout << "                   OPTIMAL TOUR                    \n";
    cout << "====================================================\n";
    cout << "Sequence of Cities:\n";
    for (int i = 0; i < tour.size(); ++i) {
        cout << setw(4) << cities[tour[i]].id;
        if ((i + 1) % 10 == 0) cout << "\n";  // Print 10 cities per line
    }
    cout << "\n====================================================\n";
}

int main() {
    srand(time(0));

    // Load cities from the TSP file
    string filename = "xqf131.tsp";  // Replace with the correct filename
    vector<City> cities = parseTSPFile(filename);

    int iterations = 10000;
    double initialTemp = 1000.0;
    double finalTemp = 0.001;

    // Header for the project
    cout << "\n****************************************************\n";
    cout << "*             TRAVELING SALESMAN PROBLEM            *\n";
    cout << "*          Solved Using Simulated Annealing         *\n";
    cout << "****************************************************\n";
    cout << "Dataset: " << filename << "\n";
    cout << "Initial Temperature: " << initialTemp << "\n";
    cout << "Final Temperature: " << finalTemp << "\n";
    cout << "Iterations: " << iterations << "\n";
    cout << "====================================================\n";

    // Run simulated annealing
    vector<int> bestTour = simulatedAnnealing(cities, iterations, initialTemp, finalTemp);

    // Output the best tour
    printTour(cities, bestTour);

    return 0;
}

