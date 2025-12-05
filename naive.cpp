#include <iostream>
#include <cmath>
using namespace std;

struct Body{
    double x, y;
    double vx, vy;
    double mass;
}

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

