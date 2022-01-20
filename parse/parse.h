typedef struct p_node p_node_t; //Parse node

bool isOp(char c);

bool isPara(char c);

bool isKeyChar(char c);

p_node_t* makeOpNode(char op, p_node_t *left_p, p_node_t *right_p, char* exp);

p_node_t* makeLeafNode(char c, char* exp);

char* substring(char* str, int i, int j);

p_node_t* parse(char* expression);