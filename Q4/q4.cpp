//
// Created by 71907 on 2025/5/2.
//
#define _USE_MATH_DEFINES
#include <atomic>
#include <iostream>
#include <iomanip>
#include <valarray>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include "q234.hpp"
#include "rk4.cpp"

using namespace std;

// (a) Define the function and set initial conditions
valarray<float> rhs(float t, const valarray<float> Y) {
  valarray<float> dY(6);
  const float g = A_GRAV, vx = Y[3], vy = Y[4], vz = Y[5];

  dY[0] = Y[3];
  dY[1] = Y[4];
  dY[2] = Y[5];

  dY[3] = 0.0;
  dY[4] = 0.0;
  dY[5] = -g;


  // (d) Add the effect of the drag force
   if (drag_on) {
    const float abs_v = sqrt(vx*vx + vy*vy + vz*vz);
    if (abs_v > 1.0e-6f) {                         // Avoid division by 0
      const float area = static_cast<float> (M_PI) * R_BALL * R_BALL;
      const float drag_coeff = -0.5 * C_DRAG * RHO_AIR * area / M_BALL * abs_v;
      dY[3] += drag_coeff * vx;
      dY[4] += drag_coeff * vy;
      dY[5] += drag_coeff * vz;
    }
  }


  // (e) Add the Magnus effect
  if (magnus_on) {
    const float mag_coeff = S_MAGN / M_BALL;
    const float ax_mag = -mag_coeff * omega[2] * vy;
    const float ay_mag =  mag_coeff * omega[2] * vx;
    dY[3] += ax_mag;
    dY[4] += ay_mag;
    // The ball has 0.02m from the vertical post
  }

  return dY;
};

int main(int argc,char* argv[]) {

  float v0 = 25.0;
  if (argc==2) v0 = atof(argv[1]);

  // Set initial conditions
  const float x0 = 0.5 * PITCH_L - 20.0;
  const float y0 = 3.0;
  const float z0 = 0.0;

  const float theta0 = 20.0 * M_PI/180.0;
  const float vx0 = v0 * cos(theta0);
  const float vy0 = 0.0;
  const float vz0 = v0 * sin(theta0);

  valarray<float> Y0 = {x0, y0, z0, vx0, vy0, vz0};

  // Print out a line
  cout << "0.0" << " " << x0 << " " << y0 << " " << z0 << " " << vx0 << " " << vy0 << " " << vz0 << "\n";


  // (b) Write a loop until the ball crosses the goal line or reaches the ground again
  const float dt = 0.01;
  float t = 0.0;
  valarray<float> Y = Y0;

  while (true) {
    Y = rk4(t, Y, dt, 1);
    t += dt;

    cout << t << " " <<  Y[0] << " " << Y[1] << " " << Y[2] << " " << Y[3] << " " << Y[4] << " " << Y[5] << "\n";

    if ( Y[0] >=  PITCH_L/2.0 || Y[2] <= 0.0 ) break;
  }

  if (Y[0] < PITCH_L/2.0) {        // The ball touches the ground before it gets there
    cerr << "The slot is weak\n";
  } else if ( Y[2] < GOAL_H ) {    // The ball crosses the goal line clear under the horizontal bar without touching it
    cerr << "The slot is good\n";
  } else {                         // The ball crosses the goal line but it goes over
    cerr << "The slot is over\n";
  }

  return 0;
}
