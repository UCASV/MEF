void create_local_K(Matrix* K, int element_id, Mesh* M){
    //(k/(x2-x1))*[1 -1; -1 1]

    K->set_size(2,2);

    float k = M->get_problem_data(THERMAL_CONDUCTIVITY);
    float l = M->get_element(element_id)->get_node2()->get_x_coordinate() - M->get_element(element_id)->get_node1()->get_x_coordinate();

    K->set(k/l,0,0);    K->set(-k/l,0,1);
    K->set(-k/l,1,0);   K->set(k/l,1,1);

    //cout << "\t\tLocal matrix created for Element " << element_id+1 << ": "; K->show(); cout << "\n";
}

void create_local_b(Vector* b, int element_id, Mesh* M){
    //(Ql/2)[1;1]
    
    b->set_size(2);

    float Q = M->get_problem_data(HEAT_SOURCE);
    float l = M->get_element(element_id)->get_node2()->get_x_coordinate() - M->get_element(element_id)->get_node1()->get_x_coordinate();

    b->set(Q*l/2,0);
    b->set(Q*l/2,1);

    //cout << "\t\tLocal vector created for Element " << element_id+1 << ": "; b->show(); cout << "\n";
}

void create_local_systems(Matrix* Ks, Vector* bs, int num_elements, Mesh* M){
    for(int e = 0; e < num_elements; e++){
        cout << "\tCreating local system for Element " << e+1 << "...\n\n";
        create_local_K(&Ks[e],e,M);
        create_local_b(&bs[e],e,M);
    }
}

void assembly_K(Matrix* K, Matrix* local_K, int index1, int index2){
    K->add(local_K->get(0,0),index1,index1);    K->add(local_K->get(0,1),index1,index2);
    K->add(local_K->get(1,0),index2,index1);    K->add(local_K->get(1,1),index2,index2);
}

void assembly_b(Vector* b, Vector* local_b, int index1, int index2){
    b->add(local_b->get(0),index1);
    b->add(local_b->get(1),index2);
}

void assembly(Matrix* K, Vector* b, Matrix* Ks, Vector* bs, int num_elements, Mesh* M){
    K->init();
    b->init();
    //K->show(); b->show();

    for(int e = 0; e < num_elements; e++){
        cout << "\tAssembling for Element " << e+1 << "...\n\n";
        int index1 = M->get_element(e)->get_node1()->get_ID() - 1;
        int index2 = M->get_element(e)->get_node2()->get_ID() - 1;

        assembly_K(K, &Ks[e], index1, index2);
        assembly_b(b, &bs[e], index1, index2);
        //cout << "\t\t"; K->show(); cout << "\t\t"; b->show(); cout << "\n";
    }
}

void apply_neumann_boundary_conditions(Vector* b, Mesh* M){
    int num_conditions = M->get_quantity(NUM_NEUMANN);

    for(int c = 0; c < num_conditions; c++){
        Condition* cond = M->get_neumann_condition(c);
        
        int index = cond->get_node()->get_ID() - 1;
        b->add(cond->get_value(), index);
    }
    //cout << "\t\t"; b->show(); cout << "\n";
}

void add_column_to_RHS(Matrix* K, Vector* b, int col, float T_bar){
    for(int r = 0; r < K->get_nrows(); r++)
        b->add(-T_bar*K->get(r,col),r);
}

void apply_dirichlet_boundary_conditions(Matrix* K, Vector* b, Mesh* M){
    int num_conditions = M->get_quantity(NUM_DIRICHLET);
    int previous_removed = 0;

    for(int c = 0; c < num_conditions; c++){
        Condition* cond = M->get_dirichlet_condition(c);    
        int index = cond->get_node()->get_ID() - 1 - previous_removed;
        float cond_value = cond->get_value();

        //K->show();
        K->remove_row(index);
        //K->show();
        //b->show();
        b->remove_row(index);
        //b->show();

        add_column_to_RHS(K, b, index, cond_value);
        //b->show();

        K->remove_column(index);
        //K->show();

        previous_removed++;
    }
}
