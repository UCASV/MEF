void calculate_inverse(Matrix* A, int n, Matrix* X){
    Matrix L(n,n), Y(n,n);
    float acum;

    for(int i= 0; i < n; i++){
        for(int j= 0; j < n; j++){
            if(i == j){
                acum = 0;
                for(int k = 0; k < j; k++){
                    acum += pow(L.get(j,k),2);
                }
                L.set(sqrt(A->get(j,j) - acum),j,j);
            }
            else{
                if(i > j){
                    acum = 0;
                    for(int k = 0; k < j; k++){
                        acum += L.get(i,k)*L.get(j,k);
                    }
                    L.set((1/L.get(j,j))*(A->get(i,j) - acum),i,j);
                } 
                else{
                    L.set(0,i,j);
                }
            }
        }
    }

    for(int i= 0; i < n; i++){
        for(int j= 0; j < n; j++){
            if(i == j){
                Y.set(1/L.get(i,i),i,i);
            }
            else{
                if(i > j){
                    acum = 0;
                    for(int k = j; k < i; k++){
                        acum += L.get(i,k)*Y.get(k,j);
                    }
                    Y.set(-(1/L.get(i,i))*acum,i,j);
                }
                else{
                    Y.set(0,i,j);
                }
            }
        }
    }

    for(int i= n-1; i >= 0; i--){
        for(int j= 0; j < n; j++){
            acum = 0;
            for(int k = i+1; k < n; k++){
                acum += L.get(k,i)*X->get(k,j);
            }
            X->set((1/L.get(i,i))*( Y.get(i,j) - acum ),i,j);
        }
    }
}
