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
#define MAXCHILDLEN 4 // �������  
#define MAXTOKENLEN 40 // ��󵥴ʳ���  

 

typedef enum
/* book-keeping tokens */
{
    ENDFILE, ERROR_INP,
    /* reserved words */
    IF, ELSE, REPEAT, UNTIL, READ, WRITE, // -then -end  
    /* multicharacter tokens */
    ID, NUM,
    /* special symbols */
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI,  //�ֺ�
    /*
       * ������
       * MOD: %
       * POW: ^
       * PP: ++����
       * MM: --�Լ�
       * RE: ::=������ʽ
       * GT�� >����
       * LE��<=С�ڵ���
       * GE: >=���ڵ���
       * NE: <>������
       * ROR/RAND/RC/RS����(|)������(&)���հ�(#)����ѡ(?)
       * FOR/WHILE/ENDWHILE��for���/while���
       */
    FOR, WHILE, ENDWHILE, MOD, POW, PP, MM, RE, GT, LE, GE, NE, ROR, RAND, RC, RS
} TokenTypeIn;

typedef enum { StmtK, ExpK } NodeKind;

typedef enum { IfK, RepeatK, AssignK, ReadK, WriteK, ForK, WhileK } StmtKind; // +fork +whilek  

typedef enum { OpK, ConstK, IdK } ExpKind;

typedef enum { Void, Integer, Boolean } ExpType;

/*
 ����״̬:
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
    vector<string> TextRows; // �ı��м���  

    TokenTypeIn token;

    int textLines;           // �ı�����  
    int lineno;              // ��ǰ��  
    int linepos;             // ��ǰ�еĵ�ǰ��  
    int bufsize;             // ��ǰ�еĳ���  
    stack<int> retraction; // ��ǰif/for��������  
    int endf;               // �����Ĵ���  

    bool inComment;          // ����ɨ��ע��  
    bool EOF_flag;           // �ı�ĩβ��־  
    bool Error;              // �����־  

    string buff;        // ��ǰ���ı�  
    string tokenString; // ��ǰɨ��ĵ���  
    string Text;        // �ʷ������ı�  
    string TreeText; // ���ṹ�����

    /* �������ֵ� */
    unordered_map<string, TokenTypeIn> reservedWords = {
        {"if", IF}, {"else", ELSE},
        {"repeat", REPEAT}, {"until", UNTIL},
        {"read", READ}, {"write", WRITE},
        {"for", FOR}, {"while", WHILE}, {"endwhile", ENDWHILE}
    };

    /* token�ֵ� */
    vector<string> hashtoken = {
        "endfile", "error", "if", "else", "repeat", "until",
        "read", "write", "identifier", "number", ":=", "::=",
        "<", "+", "-", "*", "/", "(", ")", ";", "for", "while",
        "endwhile", "%", "^", "++", "--", "=", ">", ">=",
        "<=", "<>", "|", "&", "#", "?"
    };

    vector<TreeNode*> allNodes;             // �洢���нڵ㣬�����ͷ��ڴ�  
    vector<pair<string, int>> errorList; // �洢���д���  

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