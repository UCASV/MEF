#include <iostream>

using namespace std;

#include "geometry/mesh.hpp"
#include "math_utilities/vector.hpp"
#include "math_utilities/matrix.hpp"
#include "mef_utilities/mef_process.hpp"
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
    //M.report();

    int num_elements = M.get_quantity(NUM_ELEMENTS);
    Matrix local_Ks[num_elements];
    Vector local_bs[num_elements];

    cout << "Creating local systems...\n\n";
    create_local_systems(local_Ks, local_bs, num_elements, &M);

    return 0;
}
