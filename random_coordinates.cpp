#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

// X and Y bounds for box
const double BOUND_X = 100.0;
const double BOUND_Y = 100.0;

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

        fout << x << "," << y << "," << mass << "\n";
    }
cout<<"Random points written to random_coordinates.txt\n";
    fout.close();
}


int main() {
    srand(time(0));

    string filename;
    int N;
    cout << "Enter output filename: ";
    cin >> filename;
    cout << "Enter number of random points: ";
    cin >> N;
    generateRandomPoints(filename, N);

    return 0;
}
