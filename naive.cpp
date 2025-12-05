#include <iostream>
#include <cmath>
using namespace std;

struct Body{
    double x, y;
    double vx, vy;
    double mass;
};

void calculateForce(Body& b1, Body& b2, double dt) {
    double dx = b2.x - b1.x;
    double dy = b2.y - b1.y;
    double dist = sqrt(dx*dx + dy*dy);
    
    if (dist < 1.0) return;  
    
    double G = 1.0;  
    double force = (G * b1.mass * b2.mass) / (dist * dist);
    
    
    double fx = force * dx / dist;
    double fy = force * dy / dist;
    
    
    b1.vx += fx / b1.mass * dt;
    b1.vy += fy / b1.mass * dt;
    
    b2.vx -= fx / b2.mass * dt;
    b2.vy -= fy / b2.mass * dt;
}

void naiveNBody(Body bodies[], int n, double dt) {
    int pairCount = 0;
    
    
    for (int i = 0; i < n; i++) {
        
        for (int j = i + 1; j < n; j++) {
            calculateForce(bodies[i], bodies[j], dt);
            pairCount++;
        }
    }
    
    cout << "Checked " << pairCount << " pairs (N=" << n << ")" << endl;
}

void updatePositions(Body bodies[], int n, double dt) {
    for (int i = 0; i < n; i++) {
        bodies[i].x += bodies[i].vx * dt;
        bodies[i].y += bodies[i].vy * dt;
    }
}


void printBodies(Body bodies[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "Body " << i << ": ";
        cout << "pos(" << (int)bodies[i].x << "," << (int)bodies[i].y << ") ";
        cout << "vel(" << (int)bodies[i].vx << "," << (int)bodies[i].vy << ")" << endl;
    }
}

int main() {
    cout << "=== NAIVE N-BODY ALGORITHM ===" << endl;
    cout << "Complexity: O(N²)\n" << endl;
    
    
    int n = 4;
    Body bodies[4];
    
    
    bodies[0].x = 0;   bodies[0].y = 0;   bodies[0].mass = 100;  
    bodies[0].vx = 0;  bodies[0].vy = 0;
    
    bodies[1].x = 50;  bodies[1].y = 0;   bodies[1].mass = 10;   
    bodies[1].vx = 0;  bodies[1].vy = 0;
    
    bodies[2].x = -50; bodies[2].y = 0;   bodies[2].mass = 10;   
    bodies[2].vx = 0;  bodies[2].vy = 0;
    
    bodies[3].x = 0;   bodies[3].y = 50;  bodies[3].mass = 10;   
    bodies[3].vx = 0;  bodies[3].vy = 0;
    
    cout << "Initial state:" << endl;
    printBodies(bodies, n);
    
    
    double dt = 0.1;
    cout << "\n--- Running 5 steps ---" << endl;
    for (int step = 0; step < 5; step++) {
        cout << "\nStep " << step + 1 << ":" << endl;
        naiveNBody(bodies, n, dt);
        updatePositions(bodies, n, dt);
    }
    
    cout << "\nFinal state:" << endl;
    printBodies(bodies, n);
    
    
    cout << "\n=== WHY IT'S O(N^2) ===" << endl;
    cout << "N bodies | Pairs checked" << endl;
    cout << "---------|---------------" << endl;
    for (int N = 2; N <= 64; N *= 2) {
        cout << "   " << N << "     |   " << N*(N-1)/2 << endl;
    }
    
    return 0;
}

