#include <iostream>
#include <iomanip>
#include <valarray>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "q234.hpp"
#include "interp.hpp"

using namespace std;

/* Read a file with position timeseries data formatted
 * in 3 space-separated columns:
 * t x y
 * and return the numerical data in the form of a single
 * valarray of size N*3. The data is read line-by line,
 * so that the value of row i and column j is stored in
 * the (3*i + j)-th component of the valarray
 */
valarray<float> read_tracking_data(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return valarray<float>();
    }

    vector<float> data; // Use vector so we can .push_back()

    string line;
    int row = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        int col=0;
        while (getline(ss, cell, ' ') && col < 3) {
            try {
                data.push_back(stod(cell));
            } catch (const invalid_argument& e) {
                cerr << "Error: Invalid data in file " << filename << " at row " << row << endl;
                return valarray<float>();
            }
            ++col;
        }
        ++row;
    }

    // Copy data of vector into valarray and return it
    valarray<float> valdata(data.data(), data.size());

    return valdata;
}


/* Add your functions here */

// (a) Transform the data
void coord_xfm(valarray<float> &x_out, valarray<float> &y_out,
               valarray<float> x_data, valarray<float> y_data) {
  x_out = ( x_data - 0.5f ) * PITCH_L;
  y_out = ( y_data - 0.5f ) * PITCH_W;
               }


float operator-(const slice_array<float> & lhs, const slice_array<float> & rhs);

ostream operator<<(const ostream & lhs, const valarray<float> & vx);

int main(int argc, char *argv[]) {

    valarray<float> data = read_tracking_data("tracking_data.dat");
    size_t Ndata = data.size()/3;

    /* Initialise new individual arrays by taking slices of
     * data (with stride=3) to extract the three columns  */
    valarray<float> t_varr(data[slice(0, Ndata, 3)]);
    valarray<float> x_varr(data[slice(1, Ndata, 3)]);
    valarray<float> y_varr(data[slice(2, Ndata, 3)]);

    /* Continue the main() here  */


    // (b)  Store the results in two new valarray variables
    valarray<float> x_phys(Ndata), y_phys(Ndata);
    coord_xfm(x_phys, y_phys, x_varr, y_varr);


    // (c) Compute the speed components and magnitude by second-ordered centered finite differences
    valarray<float> vx = ( x_phys.shift(-1) - x_phys.shift(1) ) / (2 * 0.2f);
    valarray<float> vy = ( y_phys.shift(-1) - y_phys.shift(1) ) / (2 * 0.2f);
    valarray<float> v_mag = sqrt(vx * vx + vy * vy);

    // cts(c) output the data to a file and show the maximum speed
    ofstream vf("speed.dat");
    for (size_t i = 0; i < Ndata; i++) {
        vf << t_varr[i] << " " << v_mag[i] << "\n";
    }
    vf.close();
    float v_max = v_mag.max();
    cout << "Max speed : " << v_max << " m/s\n";
    // Plot shown in PDF


    // (d) Compute the acceleration  components and magnitude by second-ordered centered finite differences
    valarray<float> ax = ( vx.shift(-1) - vx.shift(1) ) / ( 2 * 0.2f );
    valarray<float> ay = ( vy.shift(-1) - vy.shift(1) ) / ( 2 * 0.2f );
    valarray<float> a_mag = sqrt( ax * ax + ay * ay );
    cout << "Lengths : v_mag=" << v_mag.size() << ", a_mag=" << a_mag.size() << "\n";
    /* If using slice(), then the length of speed is ( Ndata-2 ) and acceleration is ( Ndata-4 )
     * However, symbols errors are always displayed when l used slice() and l don't know how to fix
     * So l use .shift() instead of slice() here */

    /* In the case with .shift(), the lengths of speed and acceleration are the same ( = Ndata )
     * because .shift() always returns a same-size valarray and the element of the end is replaced by 0 */


    return 0;
}
