#include <string>
#include <vector>

using namespace std;

enum Type{
    INT,
    BYTE,
    BOOL,
    STRING,
    VOID,
};
struct Scope{
    struct Record{
        Node* var;
        int offset;

        Record(Node* var, int offset): var(var), offset(offest){};
    };
    vector<Record> records;
    void insert(Node* var, int offset){
        records.push_back(Record(var, offset));
    }
};
struct SymTable{
    vector<Scope> scopes;
    vector<int> offsets;
    void push_scope(){
        scopes.push_back(Scope());
        offsets.push_back(offsets.back()); //Copy last offset
    }
    Scope top_scope(){
        return scopes.back();
    }
    void pop_scope(){
        scopes.pop_back();
        offset.pop_back();
    }
    void insert_symbol(Node* sym, int offset){
        if FuncNode
    }
};

SymTable table;
void createFunc(Node* return_type, Node* id, Node* params);
struct TypeNode {
    Type type;
    int int_value;
    string string_value;
    bool bool_value;
    char byte_value;
public TypeNode(Type type): type(type){};
};

struct FuncNode  {
    FuncNode(Node *return_type, Node *id, Node *params);
};
struct ProgramNode {};
struct FuncsDeclNode {};
struct RetTypeNode {
    Type type;
    RetTypeNode(Type type): type(type) {};`
};
struct FormalsNode {};
struct FormalsListNode {};
struct FormalDeclNode {};
struct StatementsNode {};
struct StatementNode {};
struct CallNode {};
struct ExpListNode {};
struct ExpNodeNode {};


