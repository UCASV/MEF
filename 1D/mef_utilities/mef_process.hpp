void create_local_K(Matrix* K, short element_id, Mesh* M){
    K->set_size(2,2);

    float k = M->get_problem_data(THERMAL_CONDUCTIVITY);
    float l = M->get_element(element_id)->get_node2()->get_x_coordinate() - M->get_element(element_id)->get_node1()->get_x_coordinate();

    K->set(k/l,0,0);    K->set(-k/l,0,1);
    K->set(-k/l,1,0);   K->set(k/l,1,1);

    //cout << "\t\tLocal matrix created for Element " << element_id+1 << ": "; K->show(); cout << "\n";
}

void create_local_b(Vector* b, short element_id, Mesh* M){
    b->set_size(2);

    float Q = M->get_problem_data(HEAT_SOURCE);
    float l = M->get_element(element_id)->get_node2()->get_x_coordinate() - M->get_element(element_id)->get_node1()->get_x_coordinate();

    b->set(Q*l/2,0);
    b->set(Q*l/2,1);

    //cout << "\t\tLocal vector created for Element " << element_id+1 << ": "; b->show(); cout << "\n";
}

void create_local_systems(Matrix* Ks, Vector* bs, short num_elements, Mesh* M){
    for(int e = 0; e < num_elements; e++){
        cout << "\tCreating local system for Element " << e+1 << "...\n\n";
        create_local_K(&Ks[e],e,M);
        create_local_b(&bs[e],e,M);
    }
}

void assembly_K(Matrix* K, Matrix local_K, short index1, short index2){
    K->add(local_K.get(0,0),index1,index1);    K->add(local_K.get(0,1),index1,index2);
    K->add(local_K.get(1,0),index2,index1);    K->add(local_K.get(1,1),index2,index2);
}

void assembly_b(Vector* b, Vector local_b, short index1, short index2){
    b->add(local_b.get(0),index1);
    b->add(local_b.get(1),index2);
}

void assembly(Matrix* K, Vector* b, Matrix* Ks, Vector* bs, short num_elements, Mesh* M){
    K->init();
    b->init();
    //K->show(); b->show();

    for(int e = 0; e < num_elements; e++){
        cout << "\tAssembling for Element " << e+1 << "...\n\n";
        short index1 = M->get_element(e)->get_node1()->get_ID() - 1;
        short index2 = M->get_element(e)->get_node2()->get_ID() - 1;

        assembly_K(K, Ks[e], index1, index2);
        assembly_b(b, bs[e], index1, index2);
        //cout << "\t\t"; K->show(); cout << "\t\t"; b->show(); cout << "\n";
    }
}

void apply_neumann_boundary_conditions(Vector* b, Mesh* M){
    short num_conditions = M->get_quantity(NUM_NEUMANN);

    for(int c = 0; c < num_conditions; c++){
        Condition* cond = M->get_neumann_condition(c);
        
        short index = cond->get_node()->get_ID() - 1;
        b->add(cond->get_value(), index);
    }
    //cout << "\t\t"; b->show(); cout << "\n";
}

void add_column_to_RHS(Matrix* K, Vector* b, int col, float T_bar){
    for(int r = 0; r < K->get_nrows(); r++)
        b->add(-T_bar*K->get(r,col),r);
}

void apply_dirichlet_boundary_conditions(Matrix* K, Vector* b, Mesh* M){
    short num_conditions = M->get_quantity(NUM_DIRICHLET);

    for(int c = 0; c < num_conditions; c++){
        Condition* cond = M->get_dirichlet_condition(c);
        
        short index = cond->get_node()->get_ID() - 1;
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
    }
}

void solve_system(Matrix K, Vector b, Vector* T){
    int n = K.get_nrows();
    
    Matrix Kinv(n,n);

    cout << "\tCalculating inverse of global matrix K...\n\n";
    calculate_inverse(K, n, &Kinv);

    cout << "\tPerforming final calculation...\n\n";
    product_matrix_by_vector(Kinv, b, n, T);
}

void merge_results_with_dirichlet(Vector T, Vector* Tf, int n, Mesh* M){
    int num_dirichlet = M->get_quantity(NUM_DIRICHLET);

    int cont_dirichlet = 0;
    int cont_T = 0;
    for(int i = 0; i < n; i++){
        if(M->does_node_have_dirichlet_condition(i+1)){
            Condition* cond = M->get_dirichlet_condition(cont_dirichlet);
            cont_dirichlet++;
        
            float cond_value = cond->get_value();

            Tf->set(cond_value,i);
        }else{
            Tf->set(T.get(cont_T),i);
            cont_T++;
        }
    }
}
