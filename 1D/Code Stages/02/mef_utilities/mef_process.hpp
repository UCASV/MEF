void create_local_K(Matrix* K, int element_id, Mesh* M){
    //(k/(x2-x1))*[1 -1; -1 1]

    K->set_size(2,2);

    float k = M->get_problem_data(THERMAL_CONDUCTIVITY);
    float l = M->get_element(element_id)->get_node2()->get_x_coordinate() - M->get_element(element_id)->get_node1()->get_x_coordinate();

    K->set(k/l,0,0);    K->set(-k/l,0,1);
    K->set(-k/l,1,0);   K->set(k/l,1,1);

    cout << "\t\tLocal matrix created for Element " << element_id+1 << ": "; K->show(); cout << "\n";
}

void create_local_b(Vector* b, int element_id, Mesh* M){
    //(Ql/2)[1;1]
    
    b->set_size(2);

    float Q = M->get_problem_data(HEAT_SOURCE);
    float l = M->get_element(element_id)->get_node2()->get_x_coordinate() - M->get_element(element_id)->get_node1()->get_x_coordinate();

    b->set(Q*l/2,0);
    b->set(Q*l/2,1);

    cout << "\t\tLocal vector created for Element " << element_id+1 << ": "; b->show(); cout << "\n";
}

void create_local_systems(Matrix* Ks, Vector* bs, int num_elements, Mesh* M){
    for(int e = 0; e < num_elements; e++){
        cout << "\tCreating local system for Element " << e+1 << "...\n\n";
        create_local_K(&Ks[e],e,M);
        create_local_b(&bs[e],e,M);
    }
}
