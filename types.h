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
struct TypeNode {
    Type type;
    NodeParams* next;
    NodeParams* back;

public TypeNode(Type type): type(type), next(nullptr), back(nullptr){};
};
struct IdNode {
    string id;
public IdNode(string id): id(id){};
};

struct NodeParams{
    Type type;
    string id;
    NodeParams* next;
    NodeParams* back;
public NodeParams(TypeNode type, IdNode id) type(type.type), id(id.id), next(nullptr), back(nullptr) {};
};

struct FuncCallNode{
    IdNode id;
    TypeNode types;
    FuncCallNode(IdNode id, TypeNode types): id(id), types(types) {};
    FuncCallNode(IdNode id) : id(id); {}
};

struct Scope{
    //record
    //1 -> local param -> offset +
    //2 -> func arg -> offset -
    //3 -> func -> offset = 0 (na) but has params and is_func == true
    struct Record{
        Type type;
        int offset;
        string id;
        NodeParams* params_head;
        bool is_func;
        Record(Type type, int offset, string id): type(type), offset(offest), id(id), params_head(nullptr), is_func(
                false){};
        Record(Type type, string id, NodeParams* params_head): type(type), offset(0), id(id), params_head(params_head), is_func(true){};
    };
    vector<Record> records;
    void insert_local_param(Type type, int offset, string id){
        records.push_back(Record(type, offset, id));
    }
    void insert_func(Type ret_type, string id, NodeParams* params){
        records.push_back(Record(ret_type, id, params));
    }
    void push_params(NodeParams* params){
        int offset = -1;
        NodeParams* tmp = params;
        while(tmp!= nullptr) {
            Record func_arg = Record(tmp->type, offset, tmp->id);
            offset--;
            tmp = tmp->next
        }
    }
};
struct SymTable{
    vector<Scope> scopes;
    vector<int> offsets;
    void push_scope();
    Scope top_scope();
    void pop_scope();
    void insert_symbol(TypeNode type_n, IdNode id_n, int lineno);
    void check_symbol(IdNode id_n, TypeNode type_n, int lineno);
    void insert_and_check_symbol(TypeNode type_n, IdNode id_n, TypeNode exp_n, int lineno);
    void create_func(TypeNode return_type, IdNode id, NodeParams params,  int lineno);
    bool check_func(FuncCallNode call_n);
    void check_return();
    void check_return(Type type);
    void check_exp_is_bool(TypeNode type_n);
    void check_break();
    void check_continue();
    void find_type_from_tri(TypeNode type_n1, TypeNode type_n2, TypeNode type_n3);
    void find_type_from_add_min(TypeNode type_n1, TypeNode type_n2);
    void find_type_from_mul_div(TypeNode type_n1, TypeNode type_n2);
    void find_type_from_id(IdNode id_n);
    void find_type_from_call_func(FuncCallNode call_n);
    void end_program();
};

SymTable table;


