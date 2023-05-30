class Node {
    private:
        short ID;
        float x_coordinate;

    public:
        Node(short identifier, float x_value){
            ID = identifier;
            x_coordinate = x_value;
        }

        void set_ID(short identifier){
            ID = identifier;
        }
        short get_ID(){
            return ID;
        }

        void set_x_coordinate(float x_value){
            x_coordinate = x_value;
        }
        float get_x_coordinate(){
            return x_coordinate;
        }
};
