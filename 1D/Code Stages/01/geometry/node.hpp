class Node {
    private:
        int ID;
        float x_coordinate;

    public:
        Node(int identifier, float x_value){
            ID = identifier;
            x_coordinate = x_value;
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
};
