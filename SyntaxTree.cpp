#include "pch.h"
#include "SyntaxTree.h"
#include "afxdialogex.h"


SyntaxTree::SyntaxTree() {}

TreeNode* SyntaxTree::newStmtNode(StmtKind kind)
{
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL) {
        //内存不足
        
    }
    else {
        allNodes.push_back(t);
        for (i = 0; i < MAXCHILDLEN; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

TreeNode* SyntaxTree::newExpNode(ExpKind kind)
{
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL) {
        // 内存不足
        
    }
    else {
        allNodes.push_back(t);
        for (i = 0; i < MAXCHILDLEN; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

void SyntaxTree::syntaxError(const string& message, int l)
{
    Error = TRUE;
    errorList.push_back(make_pair("error C2143: 语法错误：" + message, l) );
}

char SyntaxTree::getNextChar()
{
    if (linepos >= bufsize) {
        if (lineno >= textLines) {
            EOF_flag = TRUE;
            return EOF;
        }
        buff = TextRows[lineno++].append("\n");
        int i = 0;
        for (auto c : buff) {
            if (c == '\t')i += 4;
            else if (c == ' ')i++;
            else break;
        }
        if (i < buff.size() && buff[i] == '{')inComment = TRUE;
        if (!inComment) {
            while (!retraction.empty() && i <= retraction.top() && !buff.find("else")) {
                retraction.pop();
                endf++;
            }
            if (buff.find("if") || buff.find("for")) {
                retraction.push(i);
            }
        }
        if (buff.empty()) {
            EOF_flag = TRUE;
            return EOF;
        }
        bufsize = buff.size();
        linepos = 0;
    }
    return buff[linepos++];
}

void SyntaxTree::ungetNextChar()
{
    if (!EOF_flag) linepos--;
}

void SyntaxTree::match(TokenTypeIn expected)
{
    if (token == expected) 
      { token = getToken(); }
    else {
        syntaxError("unexpected token："+ tokenString+"  it should be a "+ hashtoken[expected], lineno);
    }
}

TokenTypeIn SyntaxTree::reservedLookup(const string& s)
{
    if (reservedWords.count(s))return reservedWords[s];
    return ID;
}

TokenTypeIn SyntaxTree::getToken()
{
    TokenTypeIn currentToken;
    StateType state = START;
    int save;
    tokenString = "";

    while (state != DONE) {
        char c = getNextChar();
        char d;
        save = TRUE;
        switch (state) {
        case START:
            if (isdigit(c))
                state = INNUM;
            else if (isalpha(c))
                state = INID;
            else if ((c == ' ') || (c == '\t') || (c == '\n'))
                save = FALSE;
            else if (c == '{') {
                state = INCOMMENT;
                inComment = TRUE;
            }
            else if (c == ':')
                state = INASSIGN;
            
            else if (c == '<')
                state = INLE;
            else if (c == '>')
                state = INGE;
            else if (c == '+')
                state = INPE;
            else if (c == '-')
                state = INME;
            else {
                state = DONE;
                switch (c)
                {
                case EOF:
                    save = FALSE;
                    currentToken = ENDFILE;
                    break;
                case '*':
                    currentToken = TIMES;
                    break;
                case '/':
                    currentToken = OVER;
                    break;
                case '(':
                    currentToken = LPAREN;
                    break;
                case ')':
                    currentToken = RPAREN;
                    break;
                case ';':
                    currentToken = SEMI;
                    break;
                case '%':
                    currentToken = MOD;
                    break;
                case '^':
                    currentToken = POW;
                    break;
                case '|':
                    currentToken = ROR;
                    break;
                case '&':
                    currentToken = RAND;
                    break;
                case '#':
                    currentToken = RC;
                    break;
                case '?':
                    currentToken = RS;
                    break;
                case '=':
                    currentToken = EQ;
                default:
                    currentToken = ERROR_INP;
                    break;
                }
            }
            break;
        case INCOMMENT:
            if (c == EOF) {
                state = DONE;
                currentToken = ENDFILE;
            }
            else if (c == '}') {
                Text.append(tokenString).append("}\t\t注释\r\n");
                tokenString = "";
                inComment = FALSE;
                save = FALSE;
                state = START;
            }
            break;
        case INNUM:
            if (!isdigit(c)) {
                ungetNextChar();
                save = FALSE;
                state = DONE;
                currentToken = NUM;
            }
            break;
        case INID:
            if (!isalpha(c)) { //默认输入的标识符都是由字母组成的
                ungetNextChar();
                save = FALSE;
                state = DONE;
                currentToken = ID;
            }
            break;
        case INASSIGN:
            state = DONE;
            if (c == '=')
                currentToken = ASSIGN;
            else if((c == ':')&&( d = getNextChar()) && (d== '=')){
                currentToken = RE;
                ungetNextChar();
            }
            else {
                ungetNextChar();
                save = FALSE;
                currentToken = ERROR_INP;
            }
            break;
        
        case INLE:  //小于号<
            state = DONE;
            if (c == '>')  //<>
                currentToken = NE;
            else if (c == '=')   //<=
                currentToken = LE;
            else {
                ungetNextChar();
                save = FALSE;
                currentToken = LT;
            }
            break;
        case INGE:
            state = DONE;
            if (c == '=')
                currentToken = GE;//  >=
            else {
                ungetNextChar();
                save = FALSE;
                currentToken = GT;//  >
            }
            break;
        case INPE:  //判读是+还是++
            state = DONE;
            if (c == '+')
                currentToken = PP;
            else {
                ungetNextChar();
                save = FALSE;
                currentToken = PLUS;
            }
            break;
        case INME:  //判断是-还是--
            state = DONE;
            if (c == '-')
                currentToken = MM;
            else {
                ungetNextChar();
                save = FALSE;
                currentToken = MINUS;
            }
            break;
        case DONE:
        default: /* should never happen */
            state = DONE;
            currentToken = ERROR_INP;
            break;
        }
        if (save)
            tokenString.push_back(char(c));
        if (state == DONE) {
            if (currentToken == ID) {
                currentToken = reservedLookup(tokenString);
                if (currentToken == ID) {
                    Text.append(tokenString).append("\t\t标识符\r\n");
                }
                else Text.append(tokenString).append("\t\t保留字\r\n");
            }
        }
    }
    switch (currentToken) {
    case NUM://数字
        Text.append(tokenString).append("\t\t数字\r\n");
        break;
    case ASSIGN://赋值运算符
    case PP:
    case MM:
    case RE:
        Text.append(tokenString).append("\t\t赋值运算符\r\n");
        break;
    case EQ://比较运算符
    case LT:
    case GT:
    case LE:
    case GE:
    case NE:
        Text.append(tokenString).append("\t\t比较运算符\r\n");
        break;
    case PLUS://运算运算符
    case MINUS:
    case TIMES:
    case OVER:
    case MOD:
    case POW:
        Text.append(tokenString).append("\t\t运算运算符\r\n");
        break;
    case LPAREN:// 分隔符
    case RPAREN:
    case SEMI:
        Text.append(tokenString).append("\t\t分隔符\r\n");
        break;
    case ROR://正则表达式运算符
    case RAND:
    case RC:
    case RS:
        Text.append(tokenString).append("\t\t正则表达式运算符\r\n");
        break;
    default:
        break;
    }
    return currentToken;
}

void SyntaxTree::init()
{
    Text.clear();
    endf = 0;
    while (!retraction.empty()) {
        retraction.pop();
    }
    linepos = 0;
    lineno = 0;
    bufsize = 0;
    textLines = TextRows.size();
    tokenString = "";
    EOF_flag = FALSE;
}

TreeNode* SyntaxTree::stmt_sequence(void)
{
    TokenTypeIn tk = token;
    TreeNode* t = statement();
    TreeNode* p = t;
    //增加了缩进（endf）和ENDWHILE作为stmt_sequence的分割符
    while ((token != ENDFILE) && (token != ELSE) && (token != UNTIL) && (token != ENDWHILE)) {
        int f = -1;
        if (tk!=WHILE && tk!=IF &&tk!=FOR ) {
            if (endf > 0) {
                if (lineno <= textLines)syntaxError("you lose a \";\"", lineno - 1);
                break;
            }
            f = lineno;
            match(SEMI);
        }
        if (endf > 0) {
            if (f >= textLines)syntaxError("unexpected token：\";\",it should be deleted", f);
            break;
        }
        TreeNode* q;
        tk = token;
        q = statement();
        if (q != NULL) {
            if (t == NULL) t = p = q;
            else {
                p->sibling = q;
                p = q;
            }
        }
    }
    if (endf > 0)endf--;
    return t;
}

TreeNode* SyntaxTree::statement(void)
{
    TreeNode* t = NULL;
    switch (token) {
    case IF: t = if_stmt(); break;
    case REPEAT: t = repeat_stmt(); break;
    case WHILE: t = while_stmt(); break;
    case PP:
    case MM:
    case ID: t = assign_stmt(); break;
    case READ: t = read_stmt(); break;
    case WRITE: t = write_stmt(); break;
    case FOR: t = for_stmt(); break;
    default: {syntaxError("unexpected token：" + tokenString + ",it should be a reservedWord ", lineno);
        token = getToken();
        break; }
    } /* end case */
    return t;
}

TreeNode* SyntaxTree::if_stmt(void)
{
    TreeNode* t = newStmtNode(IfK);
    match(IF);
    match(LPAREN);
    if (t != NULL) t->child[0] = exp();
    match(RPAREN);
    if (t != NULL) t->child[1] = stmt_sequence();
    if (token == ELSE) {
        match(ELSE);
        if (t != NULL) t->child[2] = stmt_sequence();
    }
    return t;
}

TreeNode* SyntaxTree::repeat_stmt(void)
{
    TreeNode* t = newStmtNode(RepeatK);
    match(REPEAT);
    if (t != NULL) t->child[0] = stmt_sequence();
    match(UNTIL);
    if (t != NULL) t->child[1] = exp();
    return t;
}

TreeNode* SyntaxTree::assign_stmt(void)
{
    TreeNode* t = newStmtNode(AssignK);
    if ((t != NULL) && (token == ID)) {
        strcpy_s(t->attr.name, tokenString.c_str());
        match(token);
        switch (token)
        {
        case RE:   //::=正则表达式
            match(RE);
            if (t != NULL) t->child[0] = regex_Exp();
            break;
        case ASSIGN:  //:=
            match(ASSIGN);
            if (t != NULL) t->child[0] = simple_exp();
            break;
        default:
        {   syntaxError("unexpected token：" + tokenString+ ",it should be \":=\" or \"::=\"" , lineno);
             token = getToken();
            break;
        }
        }
    }
    else if ((t != NULL) && (token == PP || token == MM)) {
        TokenTypeIn op = token == PP ? PLUS : MINUS;    //PP ++ 相当于 x:=x+1  MM -- 相当于 x:=x-1
        match(token);
        if (t != NULL)
        {
            TreeNode* p = newExpNode(OpK);
            t->child[0] = p;
            if (p != NULL)
            {
                p->attr.op = op;
                TreeNode* q = newExpNode(IdK);
                if (q != NULL)
                {
                    strcpy_s(t->attr.name, tokenString.c_str());
                    strcpy_s(q->attr.name, tokenString.c_str());
                }
                p->child[0] = q;
                p->child[1] = newExpNode(ConstK);
                p->child[1]->attr.val = 1;
            }
        }
        match(ID);
    }
    return t;
}

TreeNode* SyntaxTree::read_stmt(void)
{
    TreeNode* t = newStmtNode(ReadK);
    match(READ);
    if ((t != NULL) && (token == ID))
        strcpy_s(t->attr.name, tokenString.c_str());
    match(ID);
    return t;
}

TreeNode* SyntaxTree::write_stmt(void)
{
    TreeNode* t = newStmtNode(WriteK);
    match(WRITE);
    if (t != NULL) t->child[0] = simple_exp();
    return t;
}

TreeNode* SyntaxTree::exp(void)
{
    TreeNode* t = simple_exp();
    //LT <  EQ =  GT >
    //LE <=  GE >=  NE <>
    if ((token == LT) || (token == EQ) || (token == GT)
        || (token == LE) || (token == GE) || (token == NE)) {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
        }
        match(token);
        if (t != NULL)t->child[1] = simple_exp();
    }
    return t;
}

TreeNode* SyntaxTree::simple_exp(void)
{
    TreeNode* t = term();
    while ((token == PLUS) || (token == MINUS))
    {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = term();
        }
    }
    return t;
}

TreeNode* SyntaxTree::term(void)
{
    TreeNode* t = power();
    while ((token == TIMES) || (token == OVER) || (token == MOD))
    {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = power();
        }
    }
    return t;
}

TreeNode* SyntaxTree::power(void)
{
    TreeNode* t = factor();
    while (token == POW)
    {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL) {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = factor();
        }
    }
    return t;
}

TreeNode* SyntaxTree::factor(void)
{
    TreeNode* t = NULL;
    switch (token) {
    case NUM:
        t = newExpNode(ConstK);
        if ((t != NULL) && (token == NUM))
            t->attr.val = std::stoi(tokenString);
        match(NUM);
        break;
    case ID:
        t = newExpNode(IdK);
        if ((t != NULL) && (token == ID))
            strcpy_s(t->attr.name, tokenString.c_str());
        match(ID);
        break;
    case LPAREN:
        match(LPAREN);
        t = simple_exp();//modified
        match(RPAREN);
        break;
    default:
        syntaxError("unexpected token：" + tokenString+ ", it should be a \"number\" or \"identifier\" or \"(\"", lineno);
        token = getToken();
        break;
    }
    return t;
}

TreeNode* SyntaxTree::for_stmt(void)
{
    TreeNode* t = newStmtNode(ForK);
    match(FOR);
    match(LPAREN);
    if (t != NULL) t->child[0] = assign_stmt();//id:=simple-exp
    match(SEMI);
    if (t != NULL) t->child[1] = exp();// simple_exp comparision_op simple_exp
    match(SEMI);
    if (t != NULL) t->child[2] = assign_stmt();//(++|--)id
    match(RPAREN);
    if (t != NULL) t->child[3] = stmt_sequence();
    return t;
}

TreeNode* SyntaxTree::while_stmt(void) {
    TreeNode* t = newStmtNode(WhileK);
    match(WHILE);
    match(LPAREN);
    if (t != NULL) t->child[0] = exp();
    match(RPAREN);
    if (t != NULL) t->child[1] = stmt_sequence();
    match(ENDWHILE);
    return t;
}

TreeNode* SyntaxTree::regex_Exp(void)
{
    TreeNode* t = regex1();
    while (token == ROR)
    {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL)
        {
            p->attr.op = token;
            p->child[0] = t;
            t = p;
        }
        match(token);
        if (t != NULL) t->child[1] = regex1();
    }
    return t;
}

TreeNode* SyntaxTree::regex1(void)
{
    TreeNode* t = regex2();
    while (token == RAND)
    {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL)
        {
            p->attr.op = token;
            p->child[0] = t;
            t = p;
        }
        match(token);
        if (t != NULL)
            t->child[1] = regex2();
    }
    return t;
}

TreeNode* SyntaxTree::regex2(void)
{
    TreeNode* t = regex3();
    if (token == RC || token == RS)
    {
        TreeNode* p = newExpNode(OpK);
        if (p != NULL)
        {
            p->attr.op = token;
            p->child[0] = t;
            t = p;
        }
        match(token);
    }
    return t;
}

TreeNode* SyntaxTree::regex3(void)
{
    TreeNode* t = NULL;
    switch (token) {
    case ID:
        t = newExpNode(IdK);
        if ((t != NULL) && (token == ID))
            strcpy_s(t->attr.name, tokenString.c_str());
        match(ID);
        break;
    case LPAREN:
        match(LPAREN);
        t = regex_Exp();//modified
        match(RPAREN);
        break;
    default:
    {   syntaxError("unexpected token: " + tokenString + ", it should be a \"identifier\" or \"(\"", lineno);
        token = getToken();
        break; }
    }
    return t;
}

TreeNode* SyntaxTree::parse(const vector<string>& TextRows)
{
    this->TextRows = TextRows;
    init();//每次解析前做一次初始化
    TreeNode* t;
    token = getToken();
    t = stmt_sequence();
    if (token != ENDFILE)
        syntaxError("Code ends before file\n", lineno);
    return t;
}

CString SyntaxTree::getAllError()
{
    CString result;
    for (const auto& error : errorList) {
         
        CString formattedError;
        formattedError.Format(_T("(%d) %s\r\n"), error.second, CString(error.first.c_str()));
        result += formattedError;
    }
    return result;
}

string SyntaxTree::getText()
{
    return Text;
}

void SyntaxTree::deleteTree()
{
    for (TreeNode* t : allNodes)delete t;
    allNodes.clear();
    errorList.clear();
}





string SyntaxTree::getTreeText()
{
    return TreeText;
}

// 宏定义不变  
#define INDENT indentno+=2  
#define UNINDENT indentno-=2  

static int indentno = 0;

static void printSpaces(string& output)
{
    for (int i = 0; i < indentno; i++)
        output.append("  "); // 添加空格  
}


void SyntaxTree::printToken(TokenTypeIn token, const char* tokenString)
{
    switch (token)
    {
    case IF:
    case ELSE:
    case REPEAT:
    case UNTIL:
    case READ:
    case WRITE:
    case FOR:
    case WHILE:
    case ENDWHILE:
        TreeText.append("reserved word: " );
        TreeText.append(tokenString);
        TreeText.append("\r\n");
        break;
    case ASSIGN: TreeText.append(":=\r\n"); break;
    case LT: TreeText.append("<\r\n"); break;
    case GT: TreeText.append(">\r\n"); break;
    case EQ: TreeText.append("=\r\n"); break;
    case LPAREN: TreeText.append("(\n"); break;
    case RPAREN: TreeText.append(")\r\n"); break;
    case SEMI: TreeText.append(";\r\n"); break;
    case PLUS: TreeText.append("+\r\n"); break;
    case MINUS: TreeText.append("-\r\n"); break;
    case TIMES: TreeText.append("*\r\n"); break;
    case OVER: TreeText.append("/\r\n"); break;
    case MOD: TreeText.append("%\r\n"); break;
    case ENDFILE: TreeText.append("EOF\r\n"); break;
    case POW: TreeText.append("^\r\n"); break;
    case PP: TreeText.append("++\r\n"); break;
    case MM: TreeText.append("--\r\n"); break;
    case RE: TreeText.append("::=\r\n"); break;
    case NE: TreeText.append("<>\r\n"); break;
    case LE: TreeText.append("<=\r\n"); break;
    case GE:TreeText.append(">=\r\n"); break;
    case ROR: TreeText.append("|\r\n"); break;
    case RAND: TreeText.append("&\r\n"); break;
    case RC:TreeText.append("#\r\n"); break;
    case RS:TreeText.append("?\r\n"); break;
    case NUM:
        TreeText.append("NUM, val= ");
        TreeText += tokenString;
        TreeText.append("\r\n");
        break;
    case ID:
        TreeText.append("ID, name= ");
        TreeText += tokenString;
        TreeText.append("\r\n");
        break;
    case ERROR_INP:
        TreeText.append("ERROR: ");
        TreeText += tokenString;
        TreeText.append("\r\n");
        break;
    default: /* should never happen */
        TreeText.append("Unknown token: ");
        TreeText += tokenString;
        TreeText.append("\r\n");
    }
}


// procedure printTree prints a syntax tree to the TreeText string  
void SyntaxTree::printTree(TreeNode* tree)
{
    int i;
    INDENT;
    while (tree != NULL) {
        printSpaces(TreeText);
        if (tree->nodekind == StmtK)
        {
            switch (tree->kind.stmt) {
            case IfK:
                TreeText.append("If\r\n");
                break;
            case RepeatK:
                TreeText.append("Repeat\r\n");
                break;
            case AssignK:
                TreeText.append("Assign to: ");
                TreeText += tree->attr.name;
                TreeText.append("\r\n");
                break;
            case ReadK:
                TreeText.append("Read: ");
                TreeText += tree->attr.name;
                TreeText.append("\r\n");
                break;
            case WriteK:
                TreeText.append("Write\r\n");
                break;
            case WhileK:
                TreeText.append("While\r\n");
                break;

            case ForK:
                TreeText.append("For\r\n");
                break;
            default:
                TreeText.append("Unknown ExpNode kind\r\n");
                break;
            }
        }
        else if (tree->nodekind == ExpK)
        {
            switch (tree->kind.exp) {
            case OpK:
                TreeText.append("Op: ");
                printToken(tree->attr.op, "\0");
                break;
            case ConstK:
                TreeText.append("Const: ");
                TreeText += to_string(tree->attr.val);
                TreeText += "\r\n";
                break;
            case IdK:
                TreeText.append("Id: ");
                TreeText += tree->attr.name;
                TreeText.append("\r\n");
                break;
            default:
                TreeText.append("Unknown ExpNode kind\r\n");
                break;
            }
        }
        else TreeText.append("Unknown node kind\r\n");
        for (i = 0; i < 4; i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}

