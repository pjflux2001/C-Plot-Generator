#include <bits/stdc++.h>

using namespace std;

class Vector {
public:
    double x, y, z;

    Vector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

    double Magnitude() const {
        return sqrt(x*x + y*y + z*z);
    }

    Vector Transverse() const {
        return Vector(-y, x, 0);
    }

    Vector TransverseAxis() {
	    Vector u (0, 0, 1);  // Choose an arbitrary vector that is not collinear with Vector(x,y,z)
	    Vector w (this->y*u.z-this->z*u.y, this->z*u.x-this->x*u.z, this->x*u.y-this->y*u.x);
	    double w_mag = sqrt(w.x * w.x + w.y * w.y + w.z * w.z);
	    Vector w_unit((w.x / w_mag), (w.y / w_mag), (w.z / w_mag));
	    return Vector(w_unit.x, w_unit.y, w_unit.z);
	}

    Vector Normalized() const {
        double mag = Magnitude();
        return Vector(x / mag, y / mag, z / mag);
    }

    double DotProduct(const Vector& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector CrossProduct(const Vector& v) const {
        return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    double AngleBetween(const Vector& v) const {
        double dot = DotProduct(v);
        double mag1 = Magnitude();
        double mag2 = v.Magnitude();
        return acos(dot / (mag1 * mag2));
    }

    string to_str() const {
    	return to_string(this->x) + " " 
    		+ to_string(this->y) + " " 
    		+ to_string(this->z);
    }
};

// FourVector class that inherits from Vector and adds energy and mass computation
class FourVector : public Vector {
public:
    double E;

    FourVector(double _x, double _y, double _z, double _E) : Vector(_x, _y, _z), E(_E) {}

    double Mass() const {
        return sqrt(E * E - x * x - y * y - z * z);
    }

    double PT() const {
        return Transverse().Magnitude();
    }
    
    double PX() const {
        return x;
    }
    
    double PY() const {
        return y;
    }
    
    double PZ() const {
        return z;
    }

    string to_str() const {
    	return to_string(this->x) + " " 
    		+ to_string(this->y) + " " 
    		+ to_string(this->z) + " " 
    		+ to_string(this->E);
    }
};

// Reader class to read the data file and prepare the appropriate data class
class DataReader {
public:
    vector<FourVector> ReadDataFromFile(const string& filename) const {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Failed to open file " << filename << endl;
            exit(1);
        }

        vector<FourVector> data;
        double x, y, z, E;
        while (file >> x >> y >> z >> E) {
            data.emplace_back(FourVector(x, y, z, E));
        }

        file.close();
        return data;
    }
};

// Output system to generate 1D histogram data for a given distribution
class HistogramGenerator {
public:
    void Generate(const vector<FourVector>& data, const string& distribution,
                  int nbins, double xmin, double xmax) const {
        // Create histogram data
        vector<int> histogram(nbins, 0);
        double bin_width = (xmax - xmin) / nbins;
        for (const auto& v : data) {
            double value = 0;
            if (distribution == "pT") {
                value = v.PT();
            } else if (distribution == "pX") {
                value = v.PX();
            } else if (distribution == "pY") {
                value = v.PY();
            } else if (distribution == "pZ") {
                value = v.PZ();
            } else if (distribution == "energy") {
                value = v.E;
            } else if (distribution == "mass") {
                value = v.Mass();
            } else {
                cerr << "Error: invalid distribution requested" << endl;
                return;
            }
            if (value >= xmin && value <= xmax) {
                int bin = (int) ((value - xmin) / bin_width);
                histogram[bin]++;
            }
        }

        // Normalize histogram data
        double total = (double) data.size();
        vector<double> histogram_norm(nbins, 0);
        for (int i = 0; i < nbins; i++) {
            histogram_norm[i] = histogram[i] / (total * bin_width);
        }

        // Write histogram data to file
        ofstream file(distribution + ".dat");
        if (!file.is_open()) {
            cerr << "Error: could not create file for histogram output" << endl;
            return;
        }
        for (int i = 0; i < nbins; i++) {
            double x = xmin + (i + 0.5) * bin_width;
            file << x << " " << histogram_norm[i] << endl;
        }
        file.close();
        cout << "Histogram data written to " << distribution << ".dat" << endl;
    }
};

int main(int argc, char* argv[]) {
	if(argc != 6) {
        cout << "Please provide arguments in the following order: \n\tdatafile, distribution name, number of bins, min value for the x-axis, and max value for the x-axis" << endl;
		cout << "\nExample(s) :" << endl;
		cout << "\tg++ sol.cpp; ./a.out input.dat pT 20 0 100" << endl;
		cout << "\tg++ sol.cpp; ./a.out input.dat pX 20 -100 100" << endl;
		cout << "\tg++ sol.cpp; ./a.out input.dat pY 20 -100 100" << endl;
		cout << "\tg++ sol.cpp; ./a.out input.dat pZ 20 -100 100" << endl;
		cout << "\tg++ sol.cpp; ./a.out input.dat energy 20 0 100" << endl;
		cout << "\tg++ sol.cpp; ./a.out input.dat mass 20 0 100" << endl;
		return 0;
	}

    // Read data from file
    DataReader reader;
    vector<FourVector> data = reader.ReadDataFromFile(argv[1]);

    // Generate histograms
    HistogramGenerator histogram;
    histogram.Generate(data, argv[2], stoi(argv[3]), stod(argv[4]), stod(argv[5]));

    return 0;
}