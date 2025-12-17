#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

// X and Y bounds for box
const double BOUND_X = 100.0;
const double BOUND_Y = 100.0;

const double MASS_LOW = 10;
const double MASS_HIGH = 1e4;

const double v_LIM = 1;

void generateRandomPoints(const string& filename, int N) {
    ofstream fout(filename);

    if (!fout) {
        cerr << "Error: Could not open file.\n";
        return;
    }

    for (int i = 0; i < N; i++) {
        double x = -BOUND_X + (static_cast<double>(rand()) / RAND_MAX) * (2 * BOUND_X);
        double y = -BOUND_Y + (static_cast<double>(rand()) / RAND_MAX) * (2 * BOUND_Y);
        double mass = MASS_LOW  
            + (static_cast<double>(rand()) / RAND_MAX) *(MASS_HIGH - MASS_LOW);
        double vx, vy;
        if ((static_cast<double>(rand()) / RAND_MAX) > 0.9) {
            vx = (static_cast<double>(rand()) / RAND_MAX) * v_LIM;
        }
        if ((static_cast<double>(rand()) / RAND_MAX) > 0.9) {
            vy = (static_cast<double>(rand()) / RAND_MAX) * v_LIM;
        }
        fout << x << "," << y << "," << mass << "," << vx << "," << vy << "\n";
    }

cout<<"Random points written to random_coordinates.txt\n";
    fout.close();
}


int main() {
    srand(time(0));

    string filename = "random_coordinates.txt";
    int N;
    cout << "Enter number of random points: ";
    cin >> N;
    generateRandomPoints(filename, N);

    return 0;
}