#include <iostream>
#include <vector>
#include "lexanallis.h"
#include "sintaxanalis.h"

using namespace std;

//vector<vector <name_and_type_of_var>> variation;

vector<Lexem> Node::lex = vector<Lexem>();
int Node::index_lex = 0;
void Node::printNode(Node* node) {
        cout << node_name(node->type) << ": " << node->data;
        cout << "{ ";
        for ( Node* el : node->childs) {
            if (el == NULL) {
                cout<<"NULL";
            }
            else 
            printNode(el);
        }
        cout << " }";
    }
bool error_flag = false;
int open_brace = 0;
int close_brace = 0;

string Node::node_name(int A){
    switch (A) {
        case  ND_PLUS: return "ND_PLUS"; break;
        case  ND_MINUS: return "ND_MINUS"; break;
        case  ND_MULT: return "ND_MULT"; break;
        case  ND_DIV: return "ND_DIV"; break;
        case  ND_MOD: return "ND_MOD"; break;
        case  ND_ASSIGN: return "ND_ASSIGN"; break;
        case  ND_INIT: return "ND_INIT"; break;
        case  ND_VARI: return "ND_VARI"; break;
        case  ND_CONST: return "ND_CONST"; break;
        case  ND_PLUCEQU: return "ND_PLUCEQU"; break;
        case  ND_MINUCEQU: return "ND_MINUSEQU"; break;
        case  ND_MULTEQU: return "ND_MULTEQU"; break;
        case  ND_DEVEQU: return "ND_DEVEQU"; break;
        case  ND_PROCEQU: return "ND_PROCEQU"; break;
        case  ND_MORE: return "ND_MORE"; break;
        case  ND_LESS: return "ND_LESS"; break;
        case  ND_EQU: return "ND_EQU"; break;
        case  ND_MOREQU: return "ND_MOREQU"; break;
        case  ND_LESSEQU: return "ND_LESSEQU"; break;
        case  ND_NOTEQU: return "ND_NOTEQU"; break;
        case  ND_AND: return "ND_AND"; break;
        case  ND_OR: return "ND_OR"; break;
        case  ND_NOT: return "ND_NOT"; break;
        case  ND_DOUBPL: return "ND_DOUBPL"; break;
        case  ND_DOUBMIN: return "ND_DOUBMIN"; break;
        case  ND_IF: return "ND_IF"; break;
        case  ND_IFELSE: return "IFELSE"; break;
        case  ND_FOR: return "ND_FOR"; break;
        case  ND_WHILE: return "ND_WHILE"; break;
        case  ND_STMTLIST: return "ND_STMTLIS"; break;
        case  ND_TRUE: return "ND_TRUE"; break;
                }
    return "Anon";
}

void Node::print (){
        printNode(this);
    }


    Node::Node(int type){
        this -> type = type;
    }

    Node::Node(vector<Lexem>& lex) {
        Node::lex = lex;
        Node::index_lex = 0;
        if (lex.size() == 0){
            cout<<"Файл пуст"<<"\n";
            exit(0);
        }


        if (Node::cur_lexem().ValuaLexem != "main()" ) {  // проверка на main()
            cout << "main not found";
            exit(0);
        }
        Node::next_lexem();
        if (Node::cur_lexem().ValuaLexem != "{") {      // проверка на { после main
            cout << "expected '{' in"<<result[index_lex].NumberOfStr<<"line";
            exit(0);
        }
        open_brace++;
        Node::next_lexem();

        this->type = ND_STMTLIST;
        this->parse(this);
    }

        Node::Node(int type, int datatype, string data){
        this -> type = type;
        this -> data = data;
        this -> datatype = datatype;
    }

        Node::Node(int type, string data){
        this -> type = type;
        this -> data = data;
    }



    void Node::add_node(Node* node) {
        this->childs.push_back(node);
    }

    Node* unexpected_symbol(string A, string B){
        cout << Node::cur_lexem().NumberOfStr << ": Unexpected symbol: " + A + ". " + B + " expected;"<<'\n';
        //Node::next_lexem();
        error_flag = true;
        exit(0);
        
    }

    Node* unexpected_symbol(string token) {
        cout << Node::cur_lexem().NumberOfStr << ": Unknown start of expression: " << token << ";"<<'\n';
        error_flag = true;
        //Node::next_lexem();
        exit(0);
    }

    string Node::chek_parse_expression (vector<Lexem> &expression, int point, int open_brace, int close_brace){
        string chek_expression = "";
        if (expression.size() == 1 && (expression[point].TypeLexem == "EE_NAME" || expression[point].TypeLexem == "EE_VAL")) {
            return expression[0].ValuaLexem;
        }
        if (expression[0].TypeLexem == "EE_NAME" && (expression[1].ValuaLexem == "++" || expression[1].ValuaLexem == "--")) {
            return chek_expression = expression[0].ValuaLexem + expression[1].ValuaLexem;
            }
        while (point != expression.size()) {
            if (expression[point].ValuaLexem == "(") {
                if (point == 0 || expression[point-1].TypeLexem == "EE_PLUS" || expression[point-1].TypeLexem == "EE_MINUS" || expression[point-1].ValuaLexem == "(" || expression[point-1].TypeLexem == "EE_MULT" || expression[point-1].TypeLexem == "EE_PROC" || expression[point-1].TypeLexem == "EE_DEV") {
                    point++;
                    open_brace++;
                    return chek_parse_expression(expression, point, open_brace, close_brace);
                }
                else {cout<< "Неверное выражение в строке "<<expression[point].NumberOfStr <<'\n'; exit(0);}
                }
            if (expression[point].ValuaLexem == ")") {
                if (point == expression.size() || expression[point-1].TypeLexem == "EE_NAME" || expression[point-1].TypeLexem == "EE_VAL" || expression[point-1].ValuaLexem == ")") {
                    point++;
                    close_brace++;
                    if (expression[point].TypeLexem == "EE_PLUS" || expression[point].TypeLexem == "EE_MINUS" || expression[point].TypeLexem == "EE_MULT" || expression[point].TypeLexem == "EE_PROC" || expression[point].TypeLexem == "EE_DEV"){
                        point++;
                    }
                    return chek_parse_expression(expression, point, open_brace, close_brace);
                }
                else {cout<<"Неверное выражение в строке "<<expression[point].NumberOfStr <<'\n'; exit(0);}
            }
            
            if (expression[point].TypeLexem == "EE_NAME" || expression[point].TypeLexem == "EE_VAL" ) {
                point++;

                if (expression[point].TypeLexem == "EE_PLUS" || expression[point].TypeLexem == "EE_MINUS" || expression[point].TypeLexem == "EE_MULT" || expression[point].TypeLexem == "EE_PROC" || expression[point].TypeLexem == "EE_DEV"){
                    point++;
                    return chek_parse_expression(expression, point, open_brace, close_brace);
                }
                else if (expression[point].ValuaLexem == ")"){
                    return chek_parse_expression(expression, point, open_brace, close_brace);}
                else if (point == expression.size()){
                    for (int i = 0; i<expression.size(); i++) {
                        return chek_expression += expression[i].ValuaLexem;
                    }
                    return chek_expression;
            
                }
                else if (expression[point].ValuaLexem == ")"){
                    return chek_parse_expression(expression, point, open_brace, close_brace);
                }
                else {cout<< "Неверное выражение в строке "<<expression[point].NumberOfStr <<'\n'; exit(0);}
            }
            else {cout<< "Невернsое выражение в строке "<<expression[point].NumberOfStr <<'\n'; exit(0);}
        }
        if (open_brace > close_brace) {
            cout<<"Ожидалась ) в строке "<<expression[point-1].NumberOfStr<<'\n';
            exit(0);
            }
        if (close_brace > open_brace) {
            cout<<"Ожидалась ( в строке "<<expression[point-1].NumberOfStr<<'\n';
            exit(0);
        }
        
        for (int i = 0; i<expression.size(); i++) {
            chek_expression += expression[i].ValuaLexem;
        }
        return chek_expression;
}


   // bool Node::is_in_vector(string name){
    //vector<name_and_type_of_var> A = variation.back();
    //int i = 0;
    //while (i != A.size()) {
      //  if (A[i].name == name) {
        //    return true;
        //}
        
    //}
    //return false;
//}

    Node* Node::parse_expression() {
        vector<Lexem> expression;

        //if ((cur_lex.TypeLexem == "EE_NAME"))// and //(!is_in_vector(cur_lex.ValuaLexem)))
        //{
          //  cout<<"Переменная не определена в строке "<<cur_lex.NumberOfStr<<'\n';
            //exit(0);
        //}
        Lexem cur_lex = Node::cur_lexem();
        //expression.push_back(cur_lex);
        string chek_expression;
        while ((cur_lex.TypeLexem == "EE_NAME" || cur_lex.TypeLexem == "EE_VAL" || cur_lex.TypeLexem == "EE_PLUS" || cur_lex.TypeLexem == "EE_MINUS" || cur_lex.TypeLexem == "EE_MULT" || cur_lex.TypeLexem == "EE_DEV" || cur_lex.TypeLexem == "EE_PROC"||cur_lex.TypeLexem == "EE_QUOTE" ||cur_lex.TypeLexem == "EE_DOUBPL" || cur_lex.TypeLexem == "EE_EQ" ||(cur_lex.ValuaLexem == ")")||(cur_lex.ValuaLexem == "("))and(result[index_lex+1].ValuaLexem!="{")){
            //if ((cur_lex.TypeLexem == "EE_NAME"))// and (!is_in_vector(cur_lex.ValuaLexem)))
            //{
              //  cout<<"Переменная не определена в строке "<<cur_lex.NumberOfStr<<'\n';
                //exit(0);
            //}
                expression.push_back(cur_lex);
                chek_expression+=cur_lex.ValuaLexem;
                cur_lex = next_lexem();
             
            
        }
        
        chek_expression = chek_parse_expression(expression,0,0,0);
        Node* exp = new Node(ND_VARI, chek_expression);
        return exp;
        
    }

    Node* Node::parse_var_init() {
        Lexem cur_lex = Node::cur_lexem();
        //name_and_type_of_var parts_of_Lisa;
        //parts_of_Lisa.type = cur_lex.ValuaLexem;
        cur_lex = Node::next_lexem();
        //if(is_in_vector(cur_lex.ValuaLexem)){
          //  cout<<"Переопределение переменной:"<<cur_lex.ValuaLexem<<" в строке "<<cur_lex.NumberOfStr<<'\n';
            //exit(0);
       // }
        if (cur_lex.TypeLexem != "EE_NAME") {
            return unexpected_symbol(cur_lex.ValuaLexem, "EE_NAME");
            
        }
        //parts_of_Lisa.name = cur_lex.ValuaLexem;
        //variation[variation.size()-1].push_back(parts_of_Lisa);
        Node* node = new Node(ND_INIT, cur_lex.ValuaLexem);
        cur_lex = Node::next_lexem();
        if (cur_lex.TypeLexem == "EE_EQ") {
            cur_lex = Node::next_lexem();
            node -> add_node(node->parse_expression());
            cur_lex = Node::cur_lexem();
            if(cur_lex.TypeLexem != "EE_SIMICOL") {
                return unexpected_symbol(cur_lex.ValuaLexem, "EE_SIMICOL");
                            }
        }
        if (cur_lex.TypeLexem == "EE_SIMICOL") {
            cur_lex = Node::next_lexem();
            return node;
        } else
            return unexpected_symbol(cur_lex.ValuaLexem, "EE_EQ or EE_SIMICOL");
        
    }
    Node* Node::parse_assign(string name){
        Lexem cur_lex = cur_lexem();
        //if (!is_in_vector(name)) {
          //  cout<<"Переменная не определена в строке "<<cur_lex.NumberOfStr<<'\n';
            //exit(0);
        //}
                Node* node;
        cur_lex = Node::cur_lexem();
        if (cur_lex.TypeLexem == "EE_EQ") {
            node = new Node (ND_ASSIGN, name);
        }
        if (cur_lex.TypeLexem == "EE_PLUCEQU" ) {
            node = new Node (ND_PLUCEQU, name);
        }
        if (cur_lex.TypeLexem == "EE_MINUCEQU" ) {
            node = new Node (ND_MINUCEQU, name);
        }
        if (cur_lex.TypeLexem == "EE_MULTECO" ) {
            node = new Node (ND_MULTEQU, name);
        }
        
        if (cur_lex.TypeLexem == "EE_DEVEQU" ) {
                node = new Node (ND_DEVEQU, name);
            }
        if (cur_lex.TypeLexem == "EE_PROCEQU" ) {
                node = new Node (ND_PROCEQU, name);
            }
        if (cur_lex.TypeLexem == "EE_DOUBPL" ) {
            node = new Node (ND_DOUBPL, name);
            Node::next_lexem();
            return node;
        }
        if (cur_lex.TypeLexem == "EE_DOUBMIN" ) {
            node = new Node (ND_DOUBMIN, name);
            Node::next_lexem();
            return node;
        }
        Node::next_lexem();
        node->add_node(parse_expression());
        return node;
        
    }
    string Node::chek_parce_logic_expression(vector <Lexem>& chek_expression, int point, string chek){ //недоделана!!!
        vector<Lexem> expression;
        while (point != chek_expression.size() and chek_expression[point].TypeLexem != "EE_MORE" and chek_expression[point].TypeLexem != "EE_LESS" and chek_expression[point].TypeLexem != "EE_MOREQU" and chek_expression[point].TypeLexem != "EE_LESSEAU" and chek_expression[point].TypeLexem != "EE_EQUAL" and chek_expression[point].TypeLexem != "EE_NOTEQU" and chek_expression[point].TypeLexem != "EE_AND" and chek_expression[point].TypeLexem != "EE_OR" and chek_expression[point].TypeLexem != "EE_NOT" and chek_expression[point].TypeLexem != "EE_TRUE" and chek_expression[point].TypeLexem != "EE_FALSE"){
            
            expression.push_back(chek_expression[point]);
            point++;
            }
        chek += chek_parse_expression(expression, 0, 0, 0);
        chek += chek_expression[point].ValuaLexem;
        
        return NULL;
}

    Node* Node::parse_logic_expression(){
        vector<Lexem> chek_expression;
        Lexem cur_lex = cur_lexem();
        string expression = "";
        while (cur_lex.TypeLexem == "EE_NAME" || cur_lex.TypeLexem == "EE_VAL" || cur_lex.TypeLexem == "EE_PLUS" || cur_lex.TypeLexem == "EE_MINUS" || cur_lex.TypeLexem == "EE_MULT" || cur_lex.TypeLexem == "EE_DEV" || cur_lex.TypeLexem == "EE_PROC" || cur_lex.TypeLexem == "EE_MORE" ||cur_lex.TypeLexem == "EE_LESS" || cur_lex.TypeLexem == "EE_MOREQU" ||cur_lex.TypeLexem == "EE_LESSEAU" || cur_lex.TypeLexem == "EE_EQUAL" || cur_lex.TypeLexem == "EE_NOTEQU" ||cur_lex.TypeLexem == "EE_AND" ||cur_lex.TypeLexem == "EE_OR" || cur_lex.TypeLexem == "EE_NOT" ||cur_lex.TypeLexem == "EE_TRUE" || cur_lex.TypeLexem == "EE_FALSE" ){
            expression += cur_lex.ValuaLexem;
            //chek_expression.push_back(cur_lex);
            cur_lex = next_lexem();
        }
        
        //if (chek_expression.size() == 0) {
          //  cout<<"Expected logic expression: "<<cur_lex.NumberOfStr;
            //exit(0);
        //}
       // expression = chek_parce_logic_expression(chek_expression, 0,"");
        Node* exp = new Node(ND_VARI, expression);
        return exp;
    }

    Node* Node::parse_stmt_list() {
        Lexem cur_lex = Node::next_lexem();
        Node* node = new Node(ND_STMTLIST);
        while ( (cur_lex = Node::cur_lexem()).ValuaLexem != "}" ) {
            node->add_node(parse_statement());
        }
        close_brace++;
                 next_lexem();
       
        return node;
    }



    Node* Node::parse_statement() {
        
        vector<name_and_type_of_var> Lisa;
        //variation.push_back(Lisa);
        while (true) {
            Lexem cur_lex = Node::cur_lexem();
            cout<<cur_lex.ValuaLexem<<'\n';

            if (cur_lex.TypeLexem == "EE_SIMICOL") {
                next_lexem();
                continue;
            } else if (cur_lex.TypeLexem == "EE_TYPE") {
                return parse_var_init();
            }
            else if (cur_lex.TypeLexem == "EE_NAME"){
                string name = cur_lex.ValuaLexem;
              //  if (!is_in_vector(name)) {
                //    cout<<"Переменная не определена в строке "<<cur_lex.NumberOfStr<<'\n';
                  //  exit(0);
                //}
                cur_lex = Node::next_lexem();
                if ((cur_lex.ValuaLexem == "=") || (cur_lex.ValuaLexem == "+=") || (cur_lex.ValuaLexem == "-=") || (cur_lex.ValuaLexem == "*=") || (cur_lex.ValuaLexem == "/=") || (cur_lex.ValuaLexem == "%=") || (cur_lex.ValuaLexem == "++") ||(cur_lex.ValuaLexem == "--"))  {
                    Node* node = parse_assign (name);
                    cur_lex = Node::cur_lexem();
                    if (cur_lex.ValuaLexem != ";") {
                        return unexpected_symbol(cur_lex.ValuaLexem, "EE_SIMICOL");
                    }
                    Node::next_lexem();
                    return node;
                } else {
                    cout << "Unused expression found;"<<cur_lex.NumberOfStr;
                    error_flag = true;
                    Node::next_lexem();
                    return NULL;
                }
            }
            else if (cur_lex.TypeLexem == "EE_IF"){
                cur_lex = Node::next_lexem();
                if (cur_lex.ValuaLexem != "(") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "OPEN BRACKET");
                }
                cur_lex = Node::next_lexem();
                Node* node = new Node(ND_IF);
                node->add_node(parse_logic_expression());
                //index_lex=index_lex-1;
                cur_lex = Node::cur_lexem();
                if (cur_lex.ValuaLexem != ")") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "CLOSE BRACKET");
                    
                }
                cur_lex = Node::next_lexem();
                if (cur_lex.ValuaLexem != "{") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "OPEN BRACKET '{'");


                }
                open_brace++;
                node->add_node(parse_stmt_list());
                index_lex=index_lex-1;
                cur_lex = Node::cur_lexem();
                //if (cur_lex.ValuaLexem != "}") {
                  //  return unexpected_symbol(cur_lex.ValuaLexem, "CLOSE BRACKET '}'");
                //}
               // variation.pop_back();
               // close_brace++;
                cur_lex=next_lexem();
                if (cur_lex.ValuaLexem == "else") {
                    node -> type = ND_IFELSE;
                    cur_lex = Node::next_lexem();
                    if (cur_lex.ValuaLexem != "{") {
                        return unexpected_symbol(cur_lex.ValuaLexem, "OPEN BRACKET '{'");
                    }
                    open_brace++;
                    node->add_node(parse_stmt_list());
                    cur_lex = Node::cur_lexem();
                }
                return node;
            } else if(cur_lex.TypeLexem == "EE_WHILE") {
                cur_lex = Node::next_lexem();
                if (cur_lex.ValuaLexem != "(") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "OPEN BRACKET");
                }
                cur_lex = Node::next_lexem();
                Node* node = new Node(ND_WHILE);
                node->add_node(parse_logic_expression());
                cur_lex = Node::cur_lexem();
                if (cur_lex.ValuaLexem != ")") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "CLOSE BRACKET");
                }
                cur_lex = Node::next_lexem();
                if (cur_lex.ValuaLexem != "{") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "OPEN BRACKET '{'");
                    return NULL;

                }
                open_brace++;
                node->add_node(parse_stmt_list());
                return node;
            } else if (cur_lex.TypeLexem == "EE_FOR") {
                cur_lex = Node::next_lexem();
                Node* node = new Node(ND_FOR);
                if (cur_lex.ValuaLexem != "(") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "OPEN BRACKET");
                    }
                cur_lex = Node::next_lexem();
                if (cur_lex.TypeLexem != "EE_TYPE") {
                    node->add_node(parse_expression());
                    cur_lex = Node::cur_lexem();
                }
                else{
                node->add_node(parse_var_init());
                //cur_lex = Node::next_lexem();
                index_lex = index_lex-1;
                cur_lex = Node::cur_lexem();
                }
                
                if (cur_lex.ValuaLexem != ";") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "SIMICOLON");
                }
                cur_lex = Node::next_lexem();
                node->add_node(parse_logic_expression());
                cur_lex = Node::cur_lexem();
                if (cur_lex.ValuaLexem != ";") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "SIMICOLON");
                }
                cur_lex = Node::next_lexem();
                node->add_node(parse_expression());
                cur_lex = Node::cur_lexem();
                if (cur_lex.ValuaLexem != ")") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "CLOSE BRACKET");
                }
                cur_lex = Node::next_lexem();
                if (cur_lex.ValuaLexem != "{") {
                    return unexpected_symbol(cur_lex.ValuaLexem, "OPEN BRACKET '{'");
                }
                open_brace++;
                node->add_node(parse_stmt_list());
                return node;
            }
           else if (cur_lex.TypeLexem == "EE_ERROR_2"){
               cout<< "unexpected_symbol" << cur_lex.ValuaLexem;
               error_flag = true;
               cur_lex = Node::next_lexem();
               return NULL;
            }
           else if (cur_lex.TypeLexem == "EE_ERROR_1"){
               cout<< "wrong name" << cur_lex.ValuaLexem;
               error_flag = true;
               cur_lex = Node::next_lexem();
               return NULL;
           }
            else {
                return unexpected_symbol(cur_lex.TypeLexem);
            }
        }
    }

    void Node::parse (Node* node) {
        Lexem cur_lex;
        while ( (cur_lex = Node::cur_lexem()).ValuaLexem != "}" ) {
            node->add_node(parse_statement());
                    }
        
        close_brace++;
        if (open_brace > close_brace) {
            cout<<"Ожидалась }"<<'\n';
            exit(0);
        }
        if (close_brace > open_brace) {
            cout<<"Ожидалась  {"<<'\n';
            exit(0);
        }
    }
