#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib> // rand(), srand()
#include <ctime>   // time()
using namespace std;


struct Body {
    double x, y;    //
    double mass;
    double vx, vy;  
};

struct Force {
    double fx, fy;
};


const double G = 1.0;
const double DT = 1e-6;
const double EPS = 1e-6;
const double BOUND_X = 10.0;
const double BOUND_Y = 10.0;


void readFromFile(Body* &bodies, int &N, string filename);
void generateRandomPoints(string filename, int N);
int removeOutOfBoundary(Body* bodies, int N);
void calculateForcesAndUpdate(Body* bodies, Force* forces, int N, ofstream &fout);

int main() {
    srand(time(0)); 

    Body* bodies = nullptr;
    Force* forces = nullptr; 
    int N;

    // -------------------- Objective 1 & 1.5 --------------------
    int choice;
    cout << "Choose input method:\n1) Enter points manually\n2) Read from file\n";
    cin >> choice;

    if (choice == 1) {
        cout << "Enter number of points: ";
        cin >> N;
        bodies = new Body[N];
        forces = new Force[N];
        cout << "Enter mass, x, y for each particle:\n";
        for (int i = 0; i < N; i++) {
            cin >> bodies[i].mass >> bodies[i].x >> bodies[i].y;
            bodies[i].vx = bodies[i].vy = 0.0;
        }
    } else if (choice == 2) {
        string filename;
        cout << "Enter filename: ";
        cin >> filename;
        readFromFile(bodies, N, filename); // -------------------- Objective 2
        forces = new Force[N];
    } else {
        cout << "Invalid choice!\n";
        return 0;
    }

    // -------------------- Objective 2.5 --------------------
    N = removeOutOfBoundary(bodies, N);

    int K;
    cout << "Enter number of simulation steps: ";
    cin >> K;

    ofstream fout("simulation_output.txt");
    fout << "x, y, mass\n"; // Header

    // -------------------- Objective 4,5,6,7 --------------------
    for (int step = 0; step < K; step++) {
        calculateForcesAndUpdate(bodies, forces, N, fout);
        fout << "\n\n"; // marks end of current step - Objective 6
        cout << "Step " << step+1 << " recorded.\n";
    }

    fout.close();

    // -------------------- Objective 8 --------------------
    int N_random;
    cout << "Enter number of random points to generate: ";
    cin >> N_random;
    generateRandomPoints("random_points.txt", N_random);
    cout << "Random points saved in random_points.txt\n";

    
    delete[] bodies;
    delete[] forces;

    cout << "Simulation complete. Results saved in simulation_output.txt\n";
    return 0;
}

// -------------------- Objective 2: Read from file safely using only dynamic arrays --------------------
void readFromFile(Body* &bodies, int &N, string filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Error opening file!\n";
        N = 0;
        return;
    }

    string line;
    int count = 0;

    // -------------------- First pass: count valid lines --------------------
    while (getline(fin, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.empty()) continue; // skip blank lines
        if (line.find_first_not_of("0123456789+-.eE,") != string::npos) continue; // skip headers
        count++;
    }

    N = count;
    bodies = new Body[N];

    fin.clear();
    fin.seekg(0, ios::beg);

    // -------------------- Second pass: read data --------------------
    int index = 0;
    while (getline(fin, line) && index < N) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.empty()) continue;
        if (line.find_first_not_of("0123456789+-.eE,") != string::npos) continue;

        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        bodies[index].mass = stod(line.substr(0, pos1));
        bodies[index].x = stod(line.substr(pos1 + 1, pos2 - pos1 - 1));
        bodies[index].y = stod(line.substr(pos2 + 1));
        bodies[index].vx = bodies[index].vy = 0.0;
        index++;
    }

    fin.close();
}

// -------------------- Objective 2.5: Remove points outside boundary --------------------
int removeOutOfBoundary(Body* bodies, int N) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (bodies[i].x >= -BOUND_X && bodies[i].x <= BOUND_X &&
            bodies[i].y >= -BOUND_Y && bodies[i].y <= BOUND_Y) {
            bodies[count++] = bodies[i]; // keep in place
        }
    }
    return count;
}

// -------------------- Objective 4 & 5: Force calculation and position update --------------------
void calculateForcesAndUpdate(Body* bodies, Force* forces, int N, ofstream &fout) {
    // Reset forces
    for (int i = 0; i < N; i++) {
        forces[i].fx = 0.0;
        forces[i].fy = 0.0;
    }

    // Pairwise forces
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            double dx = bodies[j].x - bodies[i].x;
            double dy = bodies[j].y - bodies[i].y;
            double distSq = dx*dx + dy*dy + EPS;
            double dist = sqrt(distSq);

            double F = (G * bodies[i].mass * bodies[j].mass) / distSq;
            double Fx = F * dx / dist;
            double Fy = F * dy / dist;

            forces[i].fx += Fx; forces[i].fy += Fy;
            forces[j].fx -= Fx; forces[j].fy -= Fy; // Newton's third law
        }
    }

    
    for (int i = 0; i < N; i++) {
        bodies[i].vx += forces[i].fx / bodies[i].mass * DT;
        bodies[i].vy += forces[i].fy / bodies[i].mass * DT;

        bodies[i].x += bodies[i].vx * DT;
        bodies[i].y += bodies[i].vy * DT;

        fout << bodies[i].x << ", " << bodies[i].y << ", " << bodies[i].mass << "\n";
    }
}

// -------------------- Objective 8: Generate random points --------------------
void generateRandomPoints(string filename, int N) {
    ofstream fout(filename);
    for (int i = 0; i < N; i++) {
        double x = -BOUND_X + static_cast<double>(rand()) / RAND_MAX * (2*BOUND_X);
        double y = -BOUND_Y + static_cast<double>(rand()) / RAND_MAX * (2*BOUND_Y);
        double mass = 1 + static_cast<double>(rand()) / RAND_MAX * 10; // 1-11
        fout << mass << "," << x << "," << y << "\n";
    }
    fout.close();
}
