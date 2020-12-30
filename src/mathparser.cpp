#include "headers/mathparser.h"
#include <math.h>
#include <algorithm>

namespace MathParser{

const int END     = 0;
const int SUM     = 1;
const int DIF     = 2;
const int MUL     = 3;
const int DIV     = 4;
const int POW     = 5;
const int OPEN_P  = 6;
const int CLOSE_P = 7;
const int FUN     = 8;
const int CONST   = 9;
const int VAR     = 10;

const double PI = 3.141592653589793;
const double E  = 2.718281828459045;

double negative(double x){
    return -x;
}

double sign(double x){
    if(x > 0) return 1;
    if(x < 0) return -1;
    return 0;
}

std::unordered_map<std::string, Func> calcFunc = {
    {"abs", std::abs},
    {"sign", sign},
    {"sin", std::sin},
    {"cos", std::cos},
    {"tan", std::tan},
    {"sinh", std::sinh},
    {"cosh", std::cosh},
    {"tanh", std::tanh},
    {"asin", std::asin},
    {"acos", std::acos},
    {"atan", std::atan},
    {"sqrt", std::sqrt},
    {"exp", std::exp},
    {"ln", std::log},
    {"lg", std::log10},
    {"log2", std::log2},
};

NodeBase::~NodeBase() {}

NodeVariable::NodeVariable(int _idx) : idx(_idx) {}
double NodeVariable::evaluate(std::vector<double>& args){
    return args[idx];
}

NodeConstant::NodeConstant(double _value) : value(_value) {}
double NodeConstant::evaluate(std::vector<double>& args){
    return value;
}

NodeBinary::NodeBinary(NodeBase* _left, NodeBase* _right, char _op) :
    left(_left), right(_right), op(_op) {}

NodeBinary::~NodeBinary(){
    delete left;
    delete right;
}

double NodeBinary::evaluate(std::vector<double>& args){
    if(op == '+'){
        return left->evaluate(args) + right->evaluate(args);
    }else if(op == '-'){
        return left->evaluate(args) - right->evaluate(args);
    }else if(op == '*'){
        return left->evaluate(args) * right->evaluate(args);
    }else if(op == '/'){
        return left->evaluate(args) / right->evaluate(args);
    }else if(op == '^'){
        return std::pow(left->evaluate(args), right->evaluate(args));
    }
    return 0;
}

NodeFunc::NodeFunc(Func _f, NodeBase* _arg) : f(_f), arg(_arg) {}

NodeFunc::~NodeFunc(){
    delete arg;
}

double NodeFunc::evaluate(std::vector<double>& args){
    return f(arg->evaluate(args));
}

Expression::Expression() {}

Expression::Expression(Expression&& other){
    expr = std::move(other.expr);
    tokens = std::move(other.tokens);
    argNames = std::move(other.argNames);
    pos = other.pos;
    root = other.root;
    other.root = nullptr;
}

Expression::~Expression(){
    if(root != nullptr) delete root;
}

bool Expression::compile(const std::string& _expr, std::vector<std::string> _argNames){
    pos = 0;
    expr = _expr;
    argNames = _argNames;
    expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());
    if(!tokenize()) return false;
    if(!checkExpr()) return false;
    root = genExpr1();
    return true;
}

double Expression::evaluate(){
    return evaluate(std::vector<double>{});
}

double Expression::evaluate(double x){
    return evaluate(std::vector<double>{x});
}

double Expression::evaluate(double x, double y){
    return evaluate(std::vector<double>{x, y});
}

double Expression::evaluate(std::vector<double> args){
    if(args.size() != argNames.size() || root == nullptr) return 0;
    return root->evaluate(args);
}

bool Expression::isDigitOrPoint(char c){
    return (c >= '0' && c <= '9') || c == '.';
}

bool Expression::isLetter(char c){
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

bool Expression::isDelim(char c){
    return c == '+' || c == '-' || c == '*' || c == '/' ||
           c == '^' || c == '(' || c == ')';
}

bool Expression::tokenize(){
    tokens.clear();

    int charType[expr.size()+1];
    charType[expr.size()] = 0;
    for(size_t i = 0; i < expr.size(); i++){
        if(isLetter(expr[i])){
            charType[i] = 1;
        } else if(isDigitOrPoint(expr[i])){
            charType[i] = 2;
        } else if(isDelim(expr[i])){
            charType[i] = 3;
        }else return false;
    }

    size_t tokenBegin = 0;
    for(size_t i = 0; i < expr.size(); i++){
        if(charType[i] == 3){
            tokenBegin = i+1;
            if(expr[i] == '+'){
                tokens.push_back(Token(SUM));
            } else if(expr[i] == '-'){
                tokens.push_back(Token(DIF));
            } else if(expr[i] == '*'){
                tokens.push_back(Token(MUL));
            } else if(expr[i] == '/'){
                tokens.push_back(Token(DIV));
            } else if(expr[i] == '^'){
                tokens.push_back(Token(POW));
            } else if(expr[i] == '('){
                tokens.push_back(Token(OPEN_P));
            } else if(expr[i] == ')'){
                tokens.push_back(Token(CLOSE_P));
            } else return false;
            continue;
        }
        if(charType[i+1] != charType[i]){
            auto tokenStr = expr.substr(tokenBegin, i - tokenBegin + 1);
            if(charType[i] == 1){
                int idx = std::find(argNames.begin(), argNames.end(), tokenStr) - argNames.begin();
                if(idx < (int)argNames.size()){
                    Token token(VAR);
                    token.value = idx;
                    tokens.push_back(token);
                } else if(tokenStr == "pi"){
                    Token token(CONST);
                    token.value = PI;
                    tokens.push_back(token);
                } else if(tokenStr == "e"){
                    Token token(CONST);
                    token.value = E;
                    tokens.push_back(token);
                } else if(calcFunc.count(tokenStr) == 1){
                    Token token(FUN);
                    token.f = calcFunc.at(tokenStr);
                    tokens.push_back(token);
                } else return false;
            }else{
                Token token(CONST);
                token.value = atof(tokenStr.c_str());
                tokens.push_back(token);
            }
            tokenBegin = i+1;
        }
    }
    tokens.push_back(Token(END));

    return true;
}

bool Expression::isOperator(int type){
    return type == SUM || type == DIF || type == MUL || type == DIV || type == POW;
}

bool Expression::isOperand(int type){
    return type == OPEN_P || type == CONST || type == VAR || type == FUN;
}

bool Expression::checkExpr(){
    if(tokens.size() < 2) return false;
    if(!isOperand(tokens[0].type) && tokens[0].type != DIF) return false;

    for(size_t i = 0; i + 1 < tokens.size(); i++){
        if(isOperator(tokens[i].type)){
            if(!isOperand(tokens[i+1].type)) return false;
        }
        if(tokens[i].type == OPEN_P){
            if(!isOperand(tokens[i+1].type) && tokens[i+1].type != DIF) return false;
        }
        if(tokens[i].type == FUN){
            if(tokens[i+1].type != OPEN_P) return false;
        }
        if(tokens[i].type == CONST || tokens[i].type == VAR || tokens[i].type == CLOSE_P){
            if(!isOperator(tokens[i+1].type) &&
               tokens[i+1].type != END &&
               tokens[i+1].type != CLOSE_P) return false;
        }
    }

    int count = 0;
    for(size_t i = 0; i + 1 < tokens.size(); i++){
        if(tokens[i].type == OPEN_P) count++;
        if(tokens[i].type == CLOSE_P) count--;
        if(count < 0) return false;
    }
    if(count != 0) return false;

    return true;
}

Token& Expression::curToken(){
    return tokens[pos];
}

NodeBase* Expression::genExpr1(){
    NodeBase* left = genExpr2();
    if(curToken().type == SUM){
        pos++;
        return (NodeBase*)(new NodeBinary(left, genExpr1(), '+'));
    }
    if(curToken().type == DIF){
        pos++;
        return (NodeBase*)(new NodeBinary(left, genExpr1(), '-'));
    }
    return left;
}

NodeBase* Expression::genExpr2(){
    NodeBase* left = genExpr3();
    if(curToken().type == MUL){
        pos++;
        return (NodeBase*)(new NodeBinary(left, genExpr2(), '*'));
    }
    if(curToken().type == DIV){
        pos++;
        return (NodeBase*)(new NodeBinary(left, genExpr2(), '/'));
    }
    return left;
}

NodeBase* Expression::genExpr3(){
    NodeBase* left = genExpr4();
    if(curToken().type == POW){
        pos++;
        return (NodeBase*)(new NodeBinary(left, genExpr3(), '^'));
    }
    return left;
}

NodeBase* Expression::genExpr4(){
    if(curToken().type == DIF){
        pos++;
        return (NodeBase*)(new NodeFunc(negative, genExpr4()));
    }
    if(curToken().type == OPEN_P) return genParentheses();
    if(curToken().type == FUN){
        Func f = curToken().f;
        pos++;
        return (NodeBase*)(new NodeFunc(f, genParentheses()));
    }
    if(curToken().type == CONST){
        double value = curToken().value;
        pos++;
        return (NodeBase*)(new NodeConstant(value));
    }
    if(curToken().type == VAR){
        int idx = (int)curToken().value;
        pos++;
        return (NodeBase*)(new NodeVariable(idx));
    }
    return nullptr;
}

NodeBase* Expression::genParentheses(){
    pos++;
    NodeBase* res = genExpr1();
    pos++;
    return res;
}
};
