class Node {
    private:
        int ID;
        float x_coordinate;
        float y_coordinate;

    public:
        Node(int identifier, float x_value, float y_value){
            ID = identifier;
            x_coordinate = x_value;
            y_coordinate = y_value;
        }

        void set_ID(int identifier){
            ID = identifier;
        }
        int get_ID(){
            return ID;
        }

        void set_x_coordinate(float x_value){
            x_coordinate = x_value;
        }
        float get_x_coordinate(){
            return x_coordinate;
        }

        void set_y_coordinate(float y_value){
            y_coordinate = y_value;
        }
        float get_y_coordinate(){
            return y_coordinate;
        }
};
