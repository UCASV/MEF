float calculate_local_area(float x1, float y1, float x2, float y2, float x3, float y3){
    float A = abs((x1*y2 + x2*y3 + x3*y1) - (x1*y3 + x2*y1 + x3*y2))/2;
    return ((A==0)?0.000001:A);
}

float calculate_local_jacobian(float x1, float y1, float x2, float y2, float x3, float y3){
    float J = (x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1);
    return ((J==0)?0.000001:J);
}

void calculate_B(Matrix* B){
    B->set(-1,0,0);  B->set(1,0,1);  B->set(0,0,2);
    B->set(-1,1,0);  B->set(0,1,1);  B->set(1,1,2);
}

void calculate_local_A(Matrix* A, float x1, float y1, float x2, float y2, float x3, float y3){
    A->set(y3-y1, 0, 0);   A->set(x1-x3, 0, 1);
    A->set(y1-y2, 1, 0);   A->set(x2-x1, 1, 1);
}

void create_local_K(Matrix* K, int element_id, Mesh* M){
    K->set_size(3,3);

    float k = M->get_problem_data(THERMAL_CONDUCTIVITY);
    float x1 = M->get_element(element_id)->get_node1()->get_x_coordinate(), y1 = M->get_element(element_id)->get_node1()->get_y_coordinate(),
          x2 = M->get_element(element_id)->get_node2()->get_x_coordinate(), y2 = M->get_element(element_id)->get_node2()->get_y_coordinate(),
          x3 = M->get_element(element_id)->get_node3()->get_x_coordinate(), y3 = M->get_element(element_id)->get_node3()->get_y_coordinate();
    float Area = calculate_local_area(x1, y1, x2, y2, x3, y3);
    float J = calculate_local_jacobian(x1, y1, x2, y2, x3, y3);

    Matrix B(2,3), A(2,2);
    calculate_B(&B);
    calculate_local_A(&A, x1, y1, x2, y2, x3, y3);
    //B.show(); A.show();

    Matrix Bt(3,2), At(2,2);
    transpose(&B,2,3,&Bt);
    transpose(&A,2,2,&At);
    //Bt.show(); At.show();

    Matrix res1, res2, res3;
    product_matrix_by_matrix(&A,&B,&res1);
    product_matrix_by_matrix(&At,&res1,&res2);
    product_matrix_by_matrix(&Bt,&res2,&res3);
    product_scalar_by_matrix(k*Area/(J*J),&res3,3,3,K);

    //cout << "\t\tLocal matrix created for Element " << element_id+1 << ": "; K->show(); cout << "\n";
}

void create_local_b(Vector* b, int element_id, Mesh* M){
    b->set_size(3);

    float Q = M->get_problem_data(HEAT_SOURCE);
    float x1 = M->get_element(element_id)->get_node1()->get_x_coordinate(), y1 = M->get_element(element_id)->get_node1()->get_y_coordinate(),
          x2 = M->get_element(element_id)->get_node2()->get_x_coordinate(), y2 = M->get_element(element_id)->get_node2()->get_y_coordinate(),
          x3 = M->get_element(element_id)->get_node3()->get_x_coordinate(), y3 = M->get_element(element_id)->get_node3()->get_y_coordinate();
    float J = calculate_local_jacobian(x1, y1, x2, y2, x3, y3);

    b->set(Q*J/6,0);
    b->set(Q*J/6,1);
    b->set(Q*J/6,2);

    //cout << "\t\tLocal vector created for Element " << element_id+1 << ": "; b->show(); cout << "\n";
}

void create_local_systems(Matrix* Ks, Vector* bs, int num_elements, Mesh* M){
    for(int e = 0; e < num_elements; e++){
        cout << "\tCreating local system for Element " << e+1 << "...\n\n";
        create_local_K(&Ks[e],e,M);
        create_local_b(&bs[e],e,M);
    }
}

void assembly_K(Matrix* K, Matrix* local_K, int index1, int index2, int index3){
    K->add(local_K->get(0,0),index1,index1);    K->add(local_K->get(0,1),index1,index2);    K->add(local_K->get(0,2),index1,index3);
    K->add(local_K->get(1,0),index2,index1);    K->add(local_K->get(1,1),index2,index2);    K->add(local_K->get(1,2),index2,index3);
    K->add(local_K->get(2,0),index3,index1);    K->add(local_K->get(2,1),index3,index2);    K->add(local_K->get(2,2),index3,index3);
}

void assembly_b(Vector* b, Vector* local_b, int index1, int index2, int index3){
    b->add(local_b->get(0),index1);
    b->add(local_b->get(1),index2);
    b->add(local_b->get(2),index3);
}

void assembly(Matrix* K, Vector* b, Matrix* Ks, Vector* bs, int num_elements, Mesh* M){
    K->init();
    b->init();
    //K->show(); b->show();

    for(int e = 0; e < num_elements; e++){
        cout << "\tAssembling for Element " << e+1 << "...\n\n";
        int index1 = M->get_element(e)->get_node1()->get_ID() - 1;
        int index2 = M->get_element(e)->get_node2()->get_ID() - 1;
        int index3 = M->get_element(e)->get_node3()->get_ID() - 1;

        assembly_K(K, &Ks[e], index1, index2, index3);
        assembly_b(b, &bs[e], index1, index2, index3);
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

void solve_system(Matrix* K, Vector* b, Vector* T, int mode){
    int n = K->get_nrows();
    
    Matrix Kinv(n,n);

    cout << "\tCalculating inverse of global matrix K...\n\n";
    if(mode == 1) calculate_inverse(K, n, &Kinv); //1
    else calculate_inverse_Cholesky(K, n, &Kinv); //2

    cout << "\tPerforming final calculation...\n\n";
    product_matrix_by_vector(&Kinv, b, n, n, T);
}

void merge_results_with_dirichlet(Vector* T, Vector* Tf, int n, Mesh* M){
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
            Tf->set(T->get(cont_T),i);
            cont_T++;
        }
    }
}
