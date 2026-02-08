//
// Created by 71907 on 2025/5/1.
//

#include <iostream>
#include <iomanip>
#include <valarray>
#include <vector>
#include <string>
#include <fstream>
#include "q234.hpp"
#include "interp.hpp"
#include "interp.cpp"

using namespace std;


int main() {
  // (a) Use the Lagrange interpolation code to obtain the interpolating polynomial PA(v) at v = 5.8.
  valarray<float> v_A = {0, 3, 5, 8};
  valarray<float> P_A = {100,700,1100,2000};
  float PA58 = Lagrange_N(v_A, P_A, 5.8f);
  cout << "PA(5.8) = " << PA58 << " W\n" << endl;


  // (b) Degree 3: Using interp_coeffs() is better since it is faster than calling Lagrange_N() many times.


  // (c) Compute the values by using the timeseries of speeds and store
  ifstream sd("speed.dat");
  vector<float> t, v; // Use vector so we can use .push_back()
  for (float t_i, v_i; sd >> t_i >> v_i; ) {
    t.push_back(t_i);
    v.push_back(v_i);
  }
  valarray<float> v_arr(&v[0], v.size()); // // Copy data of vector into valarray
  valarray<float> p_i(v_arr.size());
  for( size_t i = 0; i < p_i.size(); ++i ) {
    p_i[i] = Lagrange_N(v_A, P_A, v_arr[i]);
  }


  // (d) Numerical integration by using the composite version of the trapezoid rule
  float dt = t[1] - t[0]; // Set step size
  float EA = 0.0f;
  for (size_t i = 0; i < p_i.size(); ++i ) {
    // If i is a endpoint, c = dt/2. Otherwise, c = dt
    float c = ( i==0 || i+1==p_i.size() ) ? dt/2 : dt;
    EA += c * p_i[i];
  }
  cout << "EA = " << EA << " J\n";


  // (e) Modify and extend the code (see below)
  // The four-point Newton–Cotes is fourth-order accurate, the error decreases proportionally to 1/N^4


  return 0;
}


// Copy from the solutions of Exercise set 8
valarray<double> trapz_coeffs(int N, double h) {
  valarray<double> ci(1.0, N+1);
  ci[0] = 0.5;
  ci[N] = 0.5;

  return h*ci;
}

valarray<double> simpson_coeffs(int N, double h) {
  if (N%2) {
    cout << "ERROR: Need even number of segments for composite Simpson integration!" << endl;
    exit(-1);
  }
  valarray<double> ci(2.0/3.0, N+1);
  for (int i = 0; i < ci.size(); i++) {
    ci[i] += (2.0/3.0 * (i % 2));
  }
  ci[0] = ci[N] = 1.0/3.0;
  return h*ci;
}


// Define a new function for the next-order Newton-Cotes formula (a composite four-point formula)
valarray<double> NC4(int N, double h)
{
  if (N%3) {
    cout << "ERROR: Need N divisible by 3 for Simpson 3/8 integration!" << endl;
    exit(-1);
  }
  valarray<double> ci(0.0, N+1);
  for (int i = 0; i < ci.size(); ++i) {
    ci[i] = (i % 3 == 0) ? 3.0 / 8.0 : 9.0 / 8.0;
  }
  return h*ci;
}

// Copy and modify nintegrate1D() from the solutions of set 8
double nintegrate1D(double a, double b,
                    double (*func)(double),
                    std::size_t N=10,
                    int  NCpoints=2)
  {
  double h = (b-a)/N;
  valarray<double> xi(N + 1);
  for (std::size_t i = 0; i <= N; ++i) xi[i] = a + i * h;
  valarray<double> fi = xi.apply(func);

  valarray<double> ci;

  switch (NCpoints) {
    case 2:  // trapezoid rule
      ci = trapz_coeffs(N, h);
    break;
    case 3:  // Simpson's rule
      ci = simpson_coeffs(N, h);
    break;
    case 4:
      ci = NC4(N, h);
    break;
    default:
      cout << "Newton-Cotes " << NCpoints << "-point formula not available. Exiting..." << endl;
    return -1.0;
  }

  return (ci*fi).sum();
}