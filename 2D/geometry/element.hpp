class Element {
    private:
        int ID;
        Node* node1;
        Node* node2;
        Node* node3;

    public:
        Element (int identifier, Node* first_node, Node* second_node, Node* third_node){
            ID = identifier;
            node1 = first_node;
            node2 = second_node;
            node3 = third_node;
        }

        void set_ID(int identifier){
            ID = identifier;
        }
        int get_ID(){
            return ID;
        }

        void set_node1(Node* node){
            node1 = node;
        }
        Node* get_node1(){
            return node1;
        }

        void set_node2(Node* node){
            node2 = node;
        }
        Node* get_node2(){
            return node2;
        }

        void set_node3(Node* node){
            node3 = node;
        }
        Node* get_node3(){
            return node3;
        }
};