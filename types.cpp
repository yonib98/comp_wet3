#include "types.h"
#include "hw3_output.hpp"
#include <string>
using namespace std;
FuncNode::FuncNode(RetTypeNode return_type, string id, NodeParams* params){
    FuncRecord func_record = FuncRecord(return_type.type, id, params);
    table.insert_func(&func_record);
}
void SymTable::push_scope(){
    scopes.push_back(Scope());
    offsets.push_back(offsets.back()); //Copy last offset
}
Scope SymTable::top_scope(){
    return scopes.back();
}
void SymTable::pop_scope(){
    scopes.pop_back();
    offset.pop_back();
}
int x;{
    int x;
}
Record* SymTamble::get_record(IdNode id_n){
    for (int i=scopes.size()-1;i>=0;i--){
        Scope curr_scope = scopes[i];
        for(int j=0;j<curr_scope.records.size();i++){
            if(curr_scope.records[i].id == id_n.id){
                return &(curr_scope.records[i]);
            }
        }
    }
    return nullptr;
}

void SymTable::check_symbol(IdNode id_n, TypeNode type_n, int lineno) {
    Record* sym_record = get_record(id_n);
    if(sym_record==nullptr){
        output::errorUndef(lineno, id_n.id);
        exit(0);
    }
    if (sym_record.type != type_n.type){
        output::errorMismatch(lineno);
        exit(0);
    }

}
void SymTable::insert_symbol(TypeNode type_n, IdNode id_n, int lineno) {
    if(get_record(id_n)!=nullptr){
        output::errorDef(lineno, id_n.id);
        exit(0);
    }
    int offset = offsets.back();
    offsets.pop_back();
    offsets.push_back(offset+1);
    top_scope().insert_local_and_args_params(type_n.type,offset, id_n.id);
}
void SymTable::insert_and_check_symbol(TypeNode type_n, IdNode id_n, TypeNode exp_n, int lineno) {
    if (type_n.type != exp_n.type){
        output::errorMismatch(lineno);
        exit(0);
    }
    insert_symbol(type_n, id_n, lineno);
}
void SymTable::create_func(TypeNode ret_type_n, IdNode id_n, NodeParams params_n, int lineno) {
    if(get_record(id_n)!=nullptr){
        output::errorDef(lineno, id_n.id);
        exit(0);
    }
    top_scope().insert_func(ret_type_n.type, id_n.id, params);
    table.push_scope();
}