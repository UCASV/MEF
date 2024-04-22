class Vector {
    private:
        int size;
        float* data;

        void create(){
            data = (float*) malloc(sizeof(float) * size);
        }

    public:
        Vector(){}
        Vector(int data_qty){
            size = data_qty;
            create();
        }
        ~Vector(){
            free(data);
        }

        void init(){
            for(int i = 0; i < size; i++)
                data[i] = 0;
        }

        void set_size(int num_values){
            size = num_values;
            create();
        }
        int get_size(){
            return size;
        }

        void set(float value, int position){
            data[position] = value;
        }
        void add(float value, int position){
            data[position] += value;
        }
        float get(int position){
            return data[position];
        }

        void show(){
            cout << "[ " << data[0];
            for(int i = 1; i < size; i++)
                cout << "; " << data[i];
            cout << " ]\n\n";
        }
};