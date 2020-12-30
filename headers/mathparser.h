#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace MathParser{
using Func = double (*)(double);

struct Token{
    int type;
    double value;
    Func f;

    Token(int _type) : type(_type){}
};

class NodeBase{
public:
    virtual double evaluate(std::vector<double>& args) = 0;
    virtual ~NodeBase();
};

class NodeVariable : public NodeBase{
public:
    NodeVariable(int _idx);
    double evaluate(std::vector<double>& args) override;
private:
    int idx;
};

class NodeConstant : public NodeBase{
public:
    NodeConstant(double _value);
    double evaluate(std::vector<double>& args) override;
private:
    double value;
};

class NodeBinary : public NodeBase{
public:
    NodeBinary(NodeBase* _left, NodeBase* _right, char _op);
    ~NodeBinary();
    double evaluate(std::vector<double>& args) override;
private:
    NodeBase* left;
    NodeBase* right;
    char op;
};

class NodeFunc : public NodeBase{
public:
    NodeFunc(Func _f, NodeBase* _arg);
    ~NodeFunc();
    double evaluate(std::vector<double>& args) override;
private:
    Func f;
    NodeBase* arg;
};

class Expression{

public:
    Expression();
    Expression(const Expression& other) = delete;
    Expression(Expression&& other);
    ~Expression();
    bool compile(const std::string& _expr, std::vector<std::string> _argNames);
    double evaluate();
    double evaluate(double x);
    double evaluate(double x, double y);
    double evaluate(std::vector<double> args);
private:
    std::string expr;
    std::vector<Token> tokens;
    std::vector<std::string> argNames;
    int pos;
    NodeBase* root = nullptr;

    Token& curToken();
    bool isDigitOrPoint(char c);
    bool isLetter(char c);
    bool isDelim(char c);
    bool tokenize();

    bool isOperator(int type);
    bool isOperand(int type);
    bool checkExpr();

    NodeBase* genExpr1();
    NodeBase* genExpr2();
    NodeBase* genExpr3();
    NodeBase* genExpr4();
    NodeBase* genParentheses();
};
};
