#include "types.h"
#include "hw3_output.hpp"
#include <string>
using namespace std;

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
Record* SymTamble::get_record(IdNode* id_n){
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

void SymTable::check_symbol(IdNode* id_n, TypeNode* type_n, int lineno) {
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
void SymTable::insert_symbol(TypeNode* type_n, IdNode* id_n, int lineno) {
    if(get_record(id_n)!=nullptr){
        output::errorDef(lineno, id_n.id);
        exit(0);
    }
    int offset = offsets.back();
    offsets.pop_back();
    offsets.push_back(offset+1);
    top_scope().insert_local_and_args_params(type_n.type,offset, id_n.id);
}
void SymTable::insert_and_check_symbol(TypeNode* type_n, IdNode* id_n, TypeNode exp_n, int lineno) {
    if (type_n.type != exp_n.type){
        output::errorMismatch(lineno);
        exit(0);
    }
    insert_symbol(type_n, id_n, lineno);
}
void SymTable::create_func(TypeNode* ret_type_n, IdNode* id_n, NodeParams* params_n, int lineno) {
    if(get_record(id_n)!=nullptr){
        output::errorDef(lineno, id_n.id);
        exit(0);
    }
    top_scope().insert_func(ret_type_n->type, id_n.id, params);
    table.current_func = top_scope().records.back();
    if (id_n.id == "main" && ret_type_n->type==VOID){
        table.is_main = true;
    }
    table.push_scope();
}

void SymTable::check_func(FuncCallNode* call_n){
    Record func_record = get_record(call_n.id);
    if(func_record == nullptr){
        output::errorUndefFunc(lineno, call_n.id);
        exit(0);
    }
    TypeNode* tmp_type_node = call_n.types;
    NodeParams* tmp_param_node = func_record.params;
    if( tmp_type_node == nullptr && tmp_param_node == nullptr){
        return;
    }
    if( tmp_type_node == nullptr || tmp_param_node == nullptr){
        errorPrototypeMismatch(lineno, id, types); //TODO: convert type to vector<string>
        exit(0);
    }
    while (tmp_type_node!=nullptr){
        if( tmp_param_node->type != tmp_type_node->type ){
            errorPrototypeMismatch(lineno, id, types); //TODO: convert type to vector<string>
            exit(0);
        }
        tmp_type_node = tmp_type_nope->next;
        tmp_param_node = tmp_param_node->next;
    }
    if(tmp_param_node != nullptr){
        errorPrototypeMismatch(lineno, id, types); //TODO: convert type to vector<string>
        exit(0);
    }
}
void SymTable::check_return(int line_no){
    if (table.current_func->type != VOID){
        errorMismatch(lineno)(lineno); //TODO: convert type to vector<string>
        exit(0);
    }
}
void SymTable::check_return(Type type, int line_no){
    if (table.current_func->type != type){
        errorMismatch(lineno);
        exit(0);
    }
}
void SymTable::check_exp_is_bool(TypeNode* type_n, int line_no){
    if (type_n->type != BOOL){
        errorMismatch(lineno);
        exit(0);
    }
}
void SymTable::check_break(int line_no){
    if (!table.while_scope=){
        errorUnexpectedBreak(line_no);
        exit(0);
    }
}
void SymTable::check_continue(int line_no){
    if (!table.while_scope=){
        errorUnexpectedContinue(line_no);
        exit(0);
    }
}


//enum Type{
//    INT, BYTE, BOOL, STRING, VOID,
//};

Type check_types(Type type_1, Type type_2){
    if(type_1 == type_2){
        return type_1;
    }
    if(type_1 != INT && type_1 != BYTE){
        return VOID;
    }
    if(type_2 != INT && type_2 != BYTE){
        return VOID;
    }
    return INT;
}

TypeNode* SymTable::find_type_from_tri(TypeNode* type_n1, TypeNode* type_n2, TypeNode* type_n3, int line_no){
    if (type_n2->type != BOOL){
        errorMismatch(line_no);
        exit(0);
    }
    Type res = check_types(type_n1->type, type_n3->type);
    if(res == VOID){
        errorMismatch(line_no);
        exit(0);
    }
    return new TypeNode(res);
}



void SymTable::find_type_from_add_min(TypeNode* type_n1, TypeNode* type_n2, int line_no) {
    if(type_1 != INT && type_1 != BYTE || type_2 != INT && type_2 != BYTE){
        errorMismatch(line_no);
        exit(0);
    }
    Type res = check_types(type_n1->type, type_n2->type);
    return new TypeNode(res);
}
void SymTable::find_type_from_mul_div(TypeNode* type_n1, TypeNode* type_n2, int line_no) {
    if(type_1 != INT && type_1 != BYTE || type_2 != INT && type_2 != BYTE){
        errorMismatch(line_no);
        exit(0);
    }
    Type res = check_types(type_n1->type, type_n2->type);
    return new TypeNode(res);
}
void SymTable::find_type_from_id(IdNode* id_n, int line_no){
    Record* record = get_record(call_n.id);
    if(record == nullptr){
        output::errorUndef(lineno, call_n.id);
        exit(0);
    }
    return new TypeNode(record->type);

}
void SymTable::find_type_from_call_func(FuncCallNode* call_n, int line_no){
    Record* func_record = get_record(call_n.id);
    check_func(call_n, line_no);
    return new TypeNode(func_record->type);
}


void start_program() {

    table = SymTable();
    NodeParams* str_node = new NodeParams(STRING, "str");
    table.top_scope().insert_func(VOID, "print",str_node );
    NodeParams* int_node = new NodeParams(INT, "int");
    table.top_scope().insert_func(VOID, "printi", int_node);
}

void end_program(int yychar, int yyeof, int line_no) {
//    if(yychar != yyeof) {
//        output::errorSyn(line_no);
//        exit(0);
//    } OLAY

    if(table.is_main == false) {
        output::errorMainMissing();
        exit(0);
    }
}
