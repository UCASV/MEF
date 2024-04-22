#include <stdio.h>

class Matrix {
    private:
        int nrows, ncols;
        float** data;

        void create(){
            data = (float**) malloc(sizeof(float*) * nrows);
            for(int r = 0; r < nrows; r++)
                data[r] = (float*) malloc(sizeof(float) * ncols);
        }

    public:
        Matrix(){}
        Matrix(int rows, int cols){
            nrows = rows;
            ncols = cols;
            create();
        }
        ~Matrix(){
            for(int r = 0; r < nrows; r++)
                free(data[r]);
            free(data);
        }

        void init(){
            for(int r = 0; r < nrows; r++)
                for(int c = 0; c < ncols; c++)
                    data[r][c] = 0;
        }

        void set_size(int rows, int cols){
            nrows = rows;
            ncols = cols;
            create();
        }
        int get_nrows(){
            return nrows;
        }
        int get_ncols(){
            return ncols;
        }

        void set(float value, int row, int col){
            data[row][col] = value;
        }
        void add(float value, int row, int col){
            data[row][col] += value;
        }
        float get(int row, int col){
            return data[row][col];
        }

        void show(){
            cout << "[ ";
            for(int r = 0; r < nrows; r++){
                cout << "[ " << data[r][0];
                for(int c = 1; c < ncols; c++){
                    cout << ", " << data[r][c];
                }
                cout << " ] ";
            }
            cout << " ]\n\n";
        }
};