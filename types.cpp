#include "types.h"
#include "hw3_output.hpp"
#include <string>
#include <iostream>
using namespace std;
SymTable table;

//enum Type{
//    INT, BYTE, BOOL, STRING, VOID,
//};

Type check_types(Type type_1, Type type_2){

    if(type_1 == type_2){
        return type_1;
    }
    if(type_1 != _INT && type_1 != _BYTE){
        return _VOID;
    }
    if(type_2 != _INT && type_2 != _BYTE){
        return _VOID;
    }
    return _INT;
}

string hamara(Type type){
    switch (type) {
        case _INT:
            return "INT";
        case _BOOL:
            return "BOOL";
        case _STRING:
            return "STRING";
        case _BYTE:
            return "BYTE";
        case _VOID:
            return "VOID";
    }
}

vector<std::string> hamara(NodeParams* param_node){
    vector<std::string> str_vec;
    NodeParams* tmp = param_node;
    while(tmp != nullptr){
        switch (tmp->type) {
            case _INT:
                str_vec.push_back("INT");
                break;
            case _BOOL:
                str_vec.push_back("BOOL");
                break;
            case _STRING:
                str_vec.push_back("STRING");
                break;
            case _BYTE:
                str_vec.push_back("BYTE");
                break;
            case _VOID:
                str_vec.push_back("VOID");
        }
        tmp = tmp->next;
    }
    return str_vec;
}
void SymTable::push_scope(){
    table.scopes.push_back(Scope());
    table.offsets.push_back(offsets.back()); //Copy last offset
}
Scope& SymTable::top_scope(){
    return table.scopes.back();
}
void SymTable::pop_scope(){
    output::endScope();
    Scope& scope = table.top_scope();
    for(int i=0;i<scope.records.size();i++){
        Scope::Record* rec = scope.records[i];
        if (rec->is_func){
            vector<string> after_hamara = hamara(rec->params_head);
            string ret_type_hamara = hamara(rec->type);
           string str = output::makeFunctionType(ret_type_hamara, after_hamara);
            output::printID(rec->id, rec->offset, str);

        }else{
            output::printID(rec->id, rec->offset, hamara(rec->type));
        }
    }
    table.scopes.pop_back();
    table.offsets.pop_back();
}

Scope::Record* get_record(IdNode* id_n){
    for (int i=table.scopes.size()-1;i>=0;i--){
        Scope& curr_scope = table.scopes[i];
        for(int j=0;j<curr_scope.records.size();j++){
            if(curr_scope.records[j]->id == id_n->id){
                return (curr_scope.records[j]);
            }
        }
    }
    return nullptr;
}

Scope::Record* get_record(string id){
    for (int i=table.scopes.size()-1;i>=0;i--){
        Scope& curr_scope = table.scopes[i];
        for(int j=0;j<curr_scope.records.size();j++){
            if(curr_scope.records[j]->id == id){
                return (curr_scope.records[j]);
            }
        }
    }
    return nullptr;
}

void SymTable::check_symbol(IdNode* id_n, TypeNode* type_n, int lineno) {
    Scope::Record* sym_record = get_record(id_n);
    if(sym_record==nullptr){
        output::errorUndef(lineno, id_n->id);
        exit(0);
    }
    if (sym_record->type != type_n->type && !(sym_record->type==_INT && type_n->type ==_BYTE)){
        output::errorMismatch(lineno);
        exit(0);
    }

}
void SymTable::insert_symbol(TypeNode* type_n, IdNode* id_n, int lineno) {
    if(get_record(id_n)!=nullptr){
        output::errorDef(lineno, id_n->id);
        exit(0);
    }
    int offset = offsets.back();
    offsets.pop_back();
    offsets.push_back(offset+1);
    top_scope().insert_local_param(type_n->type,offset, id_n->id);
}
void SymTable::insert_and_check_symbol(TypeNode* type_n, IdNode* id_n, TypeNode* exp_n, int lineno) {
    if (type_n->type != exp_n->type && !(type_n->type==_INT && exp_n->type ==_BYTE)){
        output::errorMismatch(lineno);
        exit(0);
    }
    insert_symbol(type_n, id_n, lineno);
}
void SymTable::create_func(TypeNode* ret_type_n, IdNode* id_n, NodeParams* params_n, int lineno) {
    if(get_record(id_n)!=nullptr){
        output::errorDef(lineno, id_n->id);
        exit(0);
    }
    top_scope().insert_func(ret_type_n->type, id_n->id, params_n);
    table.current_func = top_scope().records.back();

    if (id_n->id == "main" && ret_type_n->type==_VOID && params_n== nullptr){
        table.is_main = true;
    }
    table.push_scope();
    top_scope().push_params(params_n);

}



void SymTable::check_func(FuncCallNode* call_n, int lineno){
    Scope::Record* func_record = get_record(call_n->id->id);
    if(func_record == nullptr){
        output::errorUndefFunc(lineno, call_n->id->id);
        exit(0);
    }
    TypeNode* tmp_type_node = call_n->types;
    NodeParams* tmp_param_node = func_record->params_head;
    vector<string> after_hamara = hamara(func_record->params_head);

    if( tmp_type_node == nullptr && tmp_param_node == nullptr){
        return;
    }
    if( tmp_type_node == nullptr || tmp_param_node == nullptr){
        output::errorPrototypeMismatch(lineno, call_n->id->id, after_hamara);
        exit(0);
    }
    while (tmp_type_node!=nullptr && tmp_param_node!=nullptr ){
        if( check_types(tmp_param_node->type, tmp_type_node->type ) == _VOID){
            output::errorPrototypeMismatch(lineno, call_n->id->id, after_hamara);
            exit(0);
        }
        tmp_type_node = tmp_type_node->next;
        tmp_param_node = tmp_param_node->next;
    }
    if(tmp_param_node != nullptr || tmp_type_node != nullptr){
        output::errorPrototypeMismatch(lineno, call_n->id->id, after_hamara);
        exit(0);
    }
}
void SymTable::check_return(int line_no){
    if (table.current_func->type != _VOID){
        output::errorMismatch(line_no);
        exit(0);
    }
}
void SymTable::check_return(Type type, int line_no){
    if (table.current_func->type==_VOID){
        output::errorMismatch(line_no);
        exit(0);
    }
    if (table.current_func->type != type && !(table.current_func->type == _INT && type == _BYTE)){
        output::errorMismatch(line_no);
        exit(0);
    }
}
void SymTable::check_exp_is_bool(TypeNode* type_n, int line_no){
    if (type_n->type != _BOOL){
        output::errorMismatch(line_no);
        exit(0);
    }
}
void SymTable::check_break(int line_no){

    if (!table.while_scope){
        output::errorUnexpectedBreak(line_no);
        exit(0);
    }
}
void SymTable::check_continue(int line_no){

    if (!table.while_scope){
        output::errorUnexpectedContinue(line_no);
        exit(0);
    }
}

void SymTable::check_relop(TypeNode* type_n1, TypeNode* type_n2, int line_no ){
    if ((type_n1->type!=_INT && type_n1->type!=_BYTE) || type_n2->type!=_INT && type_n2->type!=_BYTE){
        output::errorMismatch(line_no);
        exit(0);
    }
}

void SymTable::check_casting(TypeNode* type_n1, TypeNode* type_n2, int line_no){
    if (check_types(type_n1->type, type_n2->type)== _VOID){
        output::errorMismatch(line_no);
        exit(0);
    }
}
void SymTable::check_logical_op(TypeNode* type_n1, TypeNode* type_n2, int line_no){
    if (type_n1->type!=_BOOL || type_n2->type!=_BOOL){
        output::errorMismatch(line_no);
        exit(0);
    }
}
void SymTable::check_logical_op(TypeNode* type_n1, int line_no){
    if (type_n1->type!=_BOOL){
        output::errorMismatch(line_no);
        exit(0);
    }
}


void SymTable::check_byte(NumNode* num_n, int line_no){
    int byte_val = std::stoi(num_n->value);
    if (!(byte_val>=0 && byte_val<=255)){
        output::errorByteTooLarge(line_no, num_n->value);
        exit(0);
    }
}
TypeNode* SymTable::find_type_from_tri(TypeNode* type_n1, TypeNode* type_n2, TypeNode* type_n3, int line_no){

    if (type_n2->type != _BOOL){
        output::errorMismatch(line_no);
        exit(0);
    }
    Type res = check_types(type_n1->type, type_n3->type);
    if(res == _VOID){
        output::errorMismatch(line_no);
        exit(0);
    }
    return new TypeNode(res);
}



TypeNode* SymTable::find_type_from_add_min(TypeNode* type_n1, TypeNode* type_n2, int line_no) {
    if(type_n1->type != _INT && type_n1->type != _BYTE || type_n2->type != _INT && type_n2->type != _BYTE){
        output::errorMismatch(line_no);
        exit(0);
    }
    Type res = check_types(type_n1->type, type_n2->type);
    return new TypeNode(res);
}
TypeNode* SymTable::find_type_from_mul_div(TypeNode* type_n1, TypeNode* type_n2, int line_no) {
    if(type_n1->type != _INT && type_n1->type != _BYTE || type_n2->type != _INT && type_n2->type != _BYTE){
        output::errorMismatch(line_no);
        exit(0);
    }
    Type res = check_types(type_n1->type, type_n2->type);
    return new TypeNode(res);
}
TypeNode* SymTable::find_type_from_id(IdNode* id_n, int line_no){
    Scope::Record* record = get_record(id_n->id);
    if(record == nullptr){
        output::errorUndef(line_no, id_n->id);
        exit(0);
    }
    return new TypeNode(record->type);

}
TypeNode* SymTable::find_type_from_call_func(FuncCallNode* call_n, int line_no){

    Scope::Record* func_record = get_record(call_n->id->id);
    check_func(call_n, line_no);
    return new TypeNode(func_record->type);
}


void start_program() {
    table = SymTable();
    TypeNode* type_str_node = new TypeNode(_STRING);
    IdNode* id_str_node = new IdNode("str");

    NodeParams* str_node = new NodeParams(type_str_node,id_str_node);
    table.top_scope().insert_func(_VOID, "print",str_node );
    TypeNode* type_int_node = new TypeNode(_INT);
    IdNode* id_int_node = new IdNode("int");
    NodeParams* int_node = new NodeParams(type_int_node, id_int_node);
    table.top_scope().insert_func(_VOID, "printi", int_node);
}

void end_program(int yychar, int yyeof, int line_no) {
    if (yychar != yyeof){
        output::errorSyn(line_no);
        exit(0);
    }
    if(table.is_main == false) {
        output::errorMainMissing();
        exit(0);
    }
    table.pop_scope();
}
