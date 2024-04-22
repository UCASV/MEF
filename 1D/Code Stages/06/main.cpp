#include <iostream>

using namespace std;

#include "geometry/mesh.hpp"
#include "math_utilities/matrix_operations.hpp"
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

    int num_nodes = M.get_quantity(NUM_NODES);
    Matrix K(num_nodes,num_nodes);
    Vector b(num_nodes);
    cout << "Performing Assembly...\n\n";
    assembly(&K, &b, local_Ks, local_bs, num_elements, &M);

    cout << "Applying Neumann Boundary Conditions...\n\n";
    apply_neumann_boundary_conditions(&b, &M);

    cout << "Applying Dirichlet Boundary Conditions...\n\n";
    apply_dirichlet_boundary_conditions(&K, &b, &M);

    //K.show(); b.show();

    cout << "Solving global system...\n\n";
    Vector T(b.get_size());
    solve_system(&K, &b, &T);
    T.show();
    
    Vector T_full(num_nodes);
    cout << "Preparing results...\n\n";
    merge_results_with_dirichlet(&T, &T_full, num_nodes, &M);
    T_full.show();

    return 0;
}
