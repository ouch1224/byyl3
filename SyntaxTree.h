#ifndef SYNTAXTREE_H  
#define SYNTAXTREE_H  

#include <vector>  
#include <string>  
#include <unordered_map>  
#include <stack>  
#include <utility> 
#include<set>
#include<sstream>
#include<fstream>
#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>

#ifndef FALSE  
#define FALSE 0  
#endif  

#ifndef TRUE  
#define TRUE 1  
#endif  
using namespace std;
#define MAXCHILDLEN 4 // 最大孩子数  
#define MAXTOKENLEN 40 // 最大单词长度  

 

typedef enum
/* book-keeping tokens */
{
    ENDFILE, ERROR_INP,
    /* reserved words */
    IF, ELSE, REPEAT, UNTIL, READ, WRITE, // -then -end  
    /* multicharacter tokens */
    ID, NUM,
    /* special symbols */
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI,  //分号
    /*
       * 新增：
       * MOD: %
       * POW: ^
       * PP: ++自增
       * MM: --自减
       * RE: ::=正则表达式
       * GT： >大于
       * LE：<=小于等于
       * GE: >=大于等于
       * NE: <>不等于
       * ROR/RAND/RC/RS：或(|)，连接(&)，闭包(#)，可选(?)
       * FOR/WHILE/ENDWHILE：for语句/while语句
       */
    FOR, WHILE, ENDWHILE, MOD, POW, PP, MM, RE, GT, LE, GE, NE, ROR, RAND, RC, RS
} TokenTypeIn;

typedef enum { StmtK, ExpK } NodeKind;

typedef enum { IfK, RepeatK, AssignK, ReadK, WriteK, ForK, WhileK } StmtKind; // +fork +whilek  

typedef enum { OpK, ConstK, IdK } ExpKind;

typedef enum { Void, Integer, Boolean } ExpType;

/*
 增加状态:
  INEQ  =
  INLE   <=,<,<>
  INGE   >,>=
  INPE   +,++
  INME   -,--
 */
typedef enum {
    START, INCOMMENT, INNUM, INID, DONE, INASSIGN, INLE, INGE, INPE, INME
} StateType;

typedef struct treeNode {
    struct treeNode* child[MAXCHILDLEN];
    struct treeNode* sibling;
    int lineno;
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp; } kind;
    union { TokenTypeIn op; int val; char name[MAXTOKENLEN]; } attr;
    ExpType type;
} TreeNode;

class SyntaxTree {
public:
    SyntaxTree();

private:
    // globals  
    vector<string> TextRows; // 文本行集合  

    TokenTypeIn token;

    int textLines;           // 文本行数  
    int lineno;              // 当前行  
    int linepos;             // 当前行的当前列  
    int bufsize;             // 当前行的长度  
    stack<int> retraction; // 当前if/for语句块缩进  
    int endf;               // 缩进的次数  

    bool inComment;          // 正在扫描注释  
    bool EOF_flag;           // 文本末尾标志  
    bool Error;              // 出错标志  

    string buff;        // 当前行文本  
    string tokenString; // 当前扫描的单词  
    string Text;        // 词法分析文本  
    string TreeText; // 树结构的输出

    /* 保留字字典 */
    unordered_map<string, TokenTypeIn> reservedWords = {
        {"if", IF}, {"else", ELSE},
        {"repeat", REPEAT}, {"until", UNTIL},
        {"read", READ}, {"write", WRITE},
        {"for", FOR}, {"while", WHILE}, {"endwhile", ENDWHILE}
    };

    /* token字典 */
    vector<string> hashtoken = {
        "endfile", "error", "if", "else", "repeat", "until",
        "read", "write", "identifier", "number", ":=", "::=",
        "<", "+", "-", "*", "/", "(", ")", ";", "for", "while",
        "endwhile", "%", "^", "++", "--", "=", ">", ">=",
        "<=", "<>", "|", "&", "#", "?"
    };

    vector<TreeNode*> allNodes;             // 存储所有节点，方便释放内存  
    vector<pair<string, int>> errorList; // 存储所有错误  

private:
    // util function  
    TreeNode* newStmtNode(StmtKind kind);
    TreeNode* newExpNode(ExpKind kind);
    void syntaxError(const string& message, int l);
    void init();
    // scan function  
    char getNextChar(void);
    void ungetNextChar(void);
    void match(TokenTypeIn expected);
    TokenTypeIn reservedLookup(const string& s);
    TokenTypeIn getToken(void);
    

    // parse function  
    TreeNode* stmt_sequence(void);
    TreeNode* statement(void);
    TreeNode* if_stmt(void);
    TreeNode* repeat_stmt(void);
    TreeNode* assign_stmt(void);
    TreeNode* read_stmt(void);
    TreeNode* write_stmt(void);
    TreeNode* for_stmt(void);
    TreeNode* while_stmt(void);
    TreeNode* exp(void);
    TreeNode* simple_exp(void);
    TreeNode* term(void);
    TreeNode* power(void);
    TreeNode* factor(void);
    TreeNode* regex_Exp(void);
    TreeNode* regex1(void);
    TreeNode* regex2(void);
    TreeNode* regex3(void);

public:
    TreeNode* parse(const vector<string>& TextRows);
    CString getAllError();
    string getText();
    void deleteTree();
    void printToken(TokenTypeIn token, const char* tokenString);
    void printTree(TreeNode* tree);
    string getTreeText();
    
};

#endif // SYNTAXTREE_H