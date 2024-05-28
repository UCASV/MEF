#include <iostream>

using namespace std;

#include "geometry/mesh.hpp"
#include "math_utilities/matrix_operations.hpp"
#include "mef_utilities/mef_process.hpp"
#include "gid/input_output.hpp"

int main (int argc, char** argv) {
    if(argc != 3){
        cout << "Incorrect use of the program, it must be: mef filename\n";
        exit(EXIT_FAILURE);
    }

    Mesh M;

    cout << "Reading geometry and mesh data...\n\n";
    string filename(argv[1]);
    read_input(filename, &M);
    //M.report();

    short num_nodes = M.get_quantity(NUM_NODES);
    short num_elements = M.get_quantity(NUM_ELEMENTS);
    Matrix K(num_nodes,num_nodes), local_Ks[num_elements];
    Vector b(num_nodes),           local_bs[num_elements];

    cout << "Creating local systems...\n\n";
    create_local_systems(local_Ks, local_bs, num_elements, &M);

    cout << "Performing Assembly...\n\n";
    assembly(&K, &b, local_Ks, local_bs, num_elements, &M);

    //K.show(); b.show();

    cout << "Applying Neumann Boundary Conditions...\n\n";
    apply_neumann_boundary_conditions(&b, &M);

    //b.show();

    cout << "Applying Dirichlet Boundary Conditions...\n\n";
    apply_dirichlet_boundary_conditions(&K, &b, &M);

    //K.show(); b.show();

    cout << "Solving global system...\n\n";
    Vector T(b.get_size()), T_full(num_nodes);
    solve_system(&K, &b, &T, atoi(argv[2]));
    //T.show();
    
    cout << "Preparing results...\n\n";
    merge_results_with_dirichlet(&T, &T_full, num_nodes, &M);
    //T_full.show();

    cout << "Writing output file...\n\n";
    write_output(filename, &T_full);

    return 0;
}
