#include <cmath>

#include "vector.hpp"
#include "matrix.hpp"

void product_scalar_by_matrix(float scalar, Matrix* M, int n, Matrix* R){
    for(int r = 0; r < n; r++)
        for(int c = 0; c < n; c++)
            R->set(scalar*M->get(r,c),r,c);
}

void product_matrix_by_vector(Matrix* M, Vector* V, int n, Vector* R){
    for(int r = 0; r < n; r++){
        float acc = 0;
        for(int c = 0; c < n; c++)
            acc += M->get(r,c)*V->get(c);
        R->set(acc,r);
    }
}

float determinant(Matrix* M);

float determinant_auxiliar(Matrix* M){
    int n = M->get_ncols();
    float acc = 0;

    for(int c = 0; c < n; c++){
        Matrix clon(n,n);
        M->clone(&clon);
        clon.remove_row(0);
        clon.remove_column(c);
        acc += pow(-1,c)*M->get(0,c)*determinant(&clon);
    }

    return acc;
}

float determinant(Matrix* M){
    float ans;
    switch(M->get_ncols()){
        case 1: ans = M->get(0,0); break;
        case 2: ans = M->get(0,0)*M->get(1,1) - M->get(0,1)*M->get(1,0); break;
        case 3: ans = M->get(0,0)*M->get(1,1)*M->get(2,2)-M->get(0,0)*M->get(1,2)*M->get(2,1)-M->get(0,1)*M->get(1,0)*M->get(2,2)+M->get(0,1)*M->get(1,2)*M->get(2,0)+M->get(0,2)*M->get(1,0)*M->get(2,1)-M->get(0,2)*M->get(1,1)*M->get(2,0); break;
        default: ans = determinant_auxiliar(M);
    }
    return ans;
}

float get_minor(Matrix* M, int n, int r, int c){
    Matrix clon(n,n);
    M->clone(&clon);

    //M.show(); clon.show();

    clon.remove_row(r);
    clon.remove_column(c);

    //clon.show();

    return determinant(&clon);
}

void conjugate_matrix(Matrix* M, int n, Matrix* C){
    for(int r = 0; r < n; r++)
        for(int c = 0; c < n; c++)
            C->set(pow(-1,r+c)*get_minor(M,n,r,c),r,c);
}

void transpose(Matrix* M, int n, Matrix* T){
    for(int r = 0; r < n; r++)
        for(int c = 0; c < n; c++)
            T->set(M->get(r,c),c,r);
}

void calculate_inverse(Matrix* M, int n, Matrix* R){
    cout << "\t\tCalculating determinant...\n\n";
    float detM = determinant(M);
    //cout << "n: " << M.get_ncols() << "\n";
    //cout << "detM: " << detM << "\n";
    
    cout << "\t\tCalculating Conjugate Matrix...\n\n";
    Matrix Conj(n,n);
    conjugate_matrix(M,n,&Conj);
    //Conj.show();
    
    cout << "\t\tCalculating Adjunct Matrix...\n\n";
    Matrix Adj(n,n);
    transpose(&Conj, n, &Adj);
    //Adj.show();

    cout << "\t\tMultiplying the Adjunct by the determinant...\n\n";
    product_scalar_by_matrix(1/detM, &Adj, n, R);
    //R->show();
}