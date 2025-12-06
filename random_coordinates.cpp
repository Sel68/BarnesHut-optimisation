#include <iostream>
#include <fstream>
#include <cstdlib>      // For rand() and srand()
#include <ctime>        // For time()
using namespace std;

// Set bounds for random x and y generation
const double BOUND_X = 100.0;
const double BOUND_Y = 100.0;

// Function that generates N random points and writes them to a file
void generateRandomPoints(const string& filename, int N) {
    ofstream fout(filename);

    if (!fout) {
        cerr << "Error: Could not open file.\n";
        return;
    }

    for (int i = 0; i < N; i++) {

        double x = -BOUND_X + (static_cast<double>(rand()) / RAND_MAX) * (2 * BOUND_X);
        double y = -BOUND_Y + (static_cast<double>(rand()) / RAND_MAX) * (2 * BOUND_Y);
        double mass = 1 + (static_cast<double>(rand()) / RAND_MAX) * 10;

        // WRITE AS x, y, mass
        fout << x << "," << y << "," << mass << "\n";
    }
cout<<"Random points written to random_coordinates.txt\n";
    fout.close();
}


int main() {
    srand(time(0));   // Seed random generator

    string filename;
    int N;

    cout << "Enter output filename: ";
    cin >> filename;

    cout << "Enter number of random points: ";
    cin >> N;

    generateRandomPoints(filename, N);

    return 0;
}
