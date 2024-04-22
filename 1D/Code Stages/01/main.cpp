#include <iostream>

using namespace std;

#include "geometry/mesh.hpp"
#include "gid/input_output.hpp"

int main (int argc, char** argv) {
    if(argc != 2){
        cout << "Incorrect use of the program, it must be: mef filename\n";
        exit(EXIT_FAILURE);
    }

    Mesh M;

    cout << "Reading geometry and mesh data...\n\n";
    string filename(argv[1]);
    read_input(filename, &M);
    M.report();

    return 0;
}
