#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <string>
#define TOKENSIZE 128
#define BUFSIZE 8
#define RENUM 128
#define NAMENUM 512
#define INTNUM 512
using namespace std;
//ID包括函数名，变量名
void error(char * msg){
	cout<<msg<<endl;
	system("pause");
	exit(0);
}
/*
enum key_word {IDEN,SL_BRACE,SR_BRACE,BL_BRACE,BR_BRACE,RETURN,SEMI,NUM,INT,VOID,PPLUS,MMINUS,ML_BRACE,MR_BRACE,ASSIGN,COMMA,PLUS,MINUS,
MULTI,DIV,STR,FOR,ANDAND,OROR,NEQ,EQUAL,GE,LE,GREATER,LESS,WHILE,IF,ELSE,PRINTF,STRING,PRINT,SCANF,AND,SHARP,PERCENT,POINT,NOT,NOT_RESEVERED};
*/

enum key_word {IDEN,SL_BRACE,SR_BRACE,NUM,ML_BRACE,MR_BRACE,PPLUS,MMINUS,MINUS,MULTI,DIV,PERCENT,PLUS,LESS,GREATER,LE,GE,EQUAL,NEQ,ANDAND,OROR,
ASSIGN,ME,DE,PE,ADDE,MINUSE,COMMA,SEMI,INT,BL_BRACE,BR_BRACE,IF,ELSE,WHILE,FOR,CONTINUE,BREAK,RETURN,SCANF,STR,PRINTF,AND,MAIN,VOID,SHARP,NOT,POINT,IGNORED};


extern int reserved_num;
extern char reserved[RENUM][TOKENSIZE];
extern char token[TOKENSIZE];
extern int iden_num;
extern char iden_table[NAMENUM][TOKENSIZE];
extern int int_num;
extern int num_table[INTNUM];
struct wordItem{
	char* token;
	int id;
};
extern ifstream f;
extern char buf[2][BUFSIZE];
extern int lexeme_begining ;
extern int forward_ptr ;
extern int bufId;
extern bool file_over;
wordItem lexcialAnalysis();

/****************Grammar Analysis********************/
#define SYN_NUM 200
#define MAX_STATE 600
#define MAX_NON_TER 100
#define MAX_TER 100
#define TER_NUM 50
#define NON_TER_NUM 50
#define ERR_NUM 0x80000000
#define ACC 99999
#define MAX_NAME_LENGTH 32
struct Node{
	char name[MAX_NAME_LENGTH];
	Node* next;
	Node(){
		next = NULL;
	}
};

typedef struct Syn{
	char leftname[32];
	Node* right;
}Syntax;

/************Semantic Analysis*************/
#define MAX_SYMBOL 200
#define MAX_GRAM 200
#define MAX_PARA_NUM 20
#define MAX_STR_LEN 1024
extern int inhVal[MAX_SYMBOL];
extern int synVal[MAX_SYMBOL];
//enum nodeType{CONSTANT,SSTR,FUNCTION,SIDEN,REGISTER,SVOID,SPECIFIER};
enum nodeType{SIDEN,SSTR,SPECIFIER,SINT,SCHAR,SVOID,CONSTANT,SFOR,SWHILE,SIF,NONE};
/*struct semSymbol{
	char name[MAX_NAME_LENGTH];
	int val;
	char sval[TOKENSIZE];
	symbolType type;
};
*/
typedef struct symbolWithAttribution{
	char name[TOKENSIZE];
	bool isTer;
	int id;
	int val;  //传常数的值用
	char sval[MAX_STR_LEN];
	int numv; 
	nodeType type;
	char* code[1024];
	bool showAlready;
	int numLine;
	int mwidth;
	symbolWithAttribution(int i){
		id = i;
		memset(name,0,sizeof(name));
		memset(sval,0,sizeof(sval));
		numLine = 0;
		mwidth=0;
		val = 0;
		showAlready=0;
	}
}SA;
stack<char*>parameter;


struct tableNode{
	char name[TOKENSIZE];
	nodeType type;
	int width;
	int offset;
	int val;
	char str[1024];
	int arrayDim;
	tableNode(){
		memset(name,0,sizeof(name));
		memset(str,0,sizeof(str));
	}
};

class codeGenerator{
public:
	char codeLine[1024][1024];
	char code[1024];
	int numLine;
	int labelNum;
	int slabelNum;
	codeGenerator(){
		labelNum = 0;
		slabelNum = 20;
		numLine = 0;
		memset(codeLine,0,sizeof(codeLine));
		regF1=false;
		regF2=false;
	}
	
	char* genTextSection(){
		strcpy(code,".section .text\n.globl _main\n_main:\n\tpushl %ebp\n\tmovl %esp, %ebp\n");
		return code;
	}
	void genStr(char*str){
		sprintf(codeLine[numLine],"L%d:	.ascii ",labelNum++);
		strcat(codeLine[numLine],"\"");
		strcat(codeLine[numLine],str);
		strcat(codeLine[numLine],"\0");
		strcat(codeLine[numLine],"\"\n");
		numLine++;
	}
	char* gencode(int v1,int v2){
		sprintf(code,"\tmovl $%d, %d(%%ebp)\n",v1,-1*v2);
		return code;
	}
	char* genInsAssign(int v,int v2){
		sprintf(code,"\tmovl $%d, %d(%%ebp)\n",v,-1*v2);
		return code;
	}
	char* genAssign(int v){
		sprintf(code,"\tmovl %%eax, %d(%%ebp)\n",-1*v);
		return code;
	}
	void conEsp(int v,SA* iA){
		if(v==0)return;
		sprintf(code,"\tsubl $%d, %%esp\n",v);
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],code);
		iA->numLine++;
	}
	void outputCode(SA* iA){
		if(iA->showAlready)return;
		if(iA->numLine==0)return;
		for(int i = 0 ;i < iA->numLine; i++){
			cout<<iA->code[i];
		}
		iA->numLine=0;
		iA->showAlready=true;
	}
	void genLable(SA* iA){
		sprintf(code,"L%d:\n",labelNum++);
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],code);
		iA->numLine++;
	}
	char* genCmp(int v1,int v2){
		char t[1024];
		sprintf(t,"\tcmpl $%d, %d(%%ebp)\n",v1,-1*v2);
		return t;
	}
	char* genJmp(int type){
		if(type==0){
			sprintf(code,"\tjge	L%d\n",labelNum+1);
		}else if(type==1){
			sprintf(code,"\tje	L%d\n",labelNum+1);
		}else if(type==2){
			sprintf(code,"\tjl	L%d\n",labelNum);
		}else if(type==3){
			sprintf(code,"\tjmp L%d\n",labelNum+1);
		}		
		return code;
	}
	char* genIncDec(bool dir,int v){
		sprintf(code,"\tleal %d(%%ebp), %%eax\n",-1*v);
		if(dir)
			strcat(code,"\tincl (%eax)\n");
		else 
			strcat(code,"\tdecl (%eax)\n");
		char tt[100];
		sprintf(tt,"\tjmp L%d\n",labelNum);
		strcat(code,tt);
		return code;
	}
	bool regF1;
	bool regF2;
	char* genIndex(int v2){
		if(regF1==0){
			sprintf(code,"\tmovl %d(%%ebp), %%eax\n",-1*v2);
		}else{
			sprintf(code,"\tmovl %d(%%ebp), %%ebx\n",-1*v2);
		}
		regF1=!regF1;
		return code;
	}
	char* genOp(int v,int v2){
		if(regF2==0){
			sprintf(code,"\tmovl %d(%%ebp,%%eax,4), %%eax\n",-1*v);
		}			
		else{
			sprintf(code,"\tmovl %d(%%ebp,%%ebx,4), %%ebx\n",-1*v);
		}
		regF2=!regF2;
		return code;
	}
	char* genMulti(){
		sprintf(code,"\timull %%ebx, %%eax\n");
		return code;
	}
	char* genDiv(int v1,int v2){
		sprintf(code,"\tmovl %d(%%ebp), %%eax\n\tcltd\n\tidivl %d(%%ebp)\n",-1*v1,-1*v2);
		return code;
	}
	char* genAdd(int v1){
		sprintf(code,"\taddl %d(%%ebp), %%eax\n",-1*v1);
		return code;
	}
	char* genMinus(int v,int v1,int type){
		char t2[1024];
		if(type==0){
			sprintf(code,"\tmovl %d(%%ebp), %%eax\n",-1*v);
			sprintf(t2,"\tsubl $%d, %%eax\n",v1);
		}else{
			sprintf(code,"\tmovl $%d, %%eax\n",v1);
			sprintf(t2,"\tsubl %d(%%ebp), %%eax\n",-1*v);
		}		
		strcat(code,t2);
		return code;
	}
	char* genPrintf(int v){
		sprintf(code,"\tmovl %d(%%ebp), %%eax\n\tpushl %%eax\n\tpushl $L%d\n\t\call _printf\n",-1*v,slabelNum-1);
		return code;
	}
	char* genPrintString(){
		sprintf(code,"\tpushl $L%d\n\t\call _printf\n",slabelNum-1);
		return code;
	}
	char* genRet(int v){
		sprintf(code,"\tmovl $%d, %%eax\n\tleave\n\tret\n",v);
		return code;
	}
	
}codeGen;
/*
class forBlock:public codeGenerator{
public:
	char beginLine[1024];
	char endLine[10][1024];
	int endNum;
	void setEndLine(bool dir,int i){
		sprintf(endLine[endNum++],"leal %d(%%ebp), %eax\n",i);
		if(dir)
			strcpy(endLine[endNum++],"incl %%eax\n");
		else 
			strcpy(endLine[endNum++],"decl %%eax\n");
	}
	void gencodeFor(int v1,int v2){
		sprintf(codeLine[numLine++],"\tcmpl $%d, %d(%%ebp)\n",-1*v2);
		sprintf(codeLine[numLine++],"\tjge L%d\n",labelNum++);
	}
}forGen;
*/
class symbolTable{
public:
	symbolTable* outSide;
	symbolTable* inside;
	int cnt;
	tableNode* table[MAX_SYMBOL];
	
	int insertTable(tableNode* t){
		for(int i = 0; i < cnt; i++){
			if(strcmp(t->name,table[i]->name)==0){
				error("duplicated item");
			}
		}
		table[cnt]=new tableNode();
		strcpy(table[cnt]->name,t->name);
		
		strcpy(table[cnt]->str , t->str);
		table[cnt]->val = t->val;
		switch (t->type){
		case SINT:
			table[cnt]->width = 4;
			table[cnt]->type = SIDEN;
			if(cnt>=1)
				table[cnt]->offset = table[cnt-1]->offset+table[cnt-1]->width ;
			else table[cnt]->offset = 0;
			break;
		case SSTR:
			table[cnt]->type = SSTR;
			table[cnt]->width = strlen(t->str)+1;
			if(cnt>=1)
				table[cnt]->offset = table[cnt-1]->offset + table[cnt-1]->width;
			else table[cnt]->offset = 0;
			break;
		}
		cnt++;
		return table[cnt-1]->offset;
	}
	tableNode* search(char * name){
		for(int i = 0; i < cnt; i++){
			if(strcmp(name,table[i]->name)==0){
				return table[i];
			}
		}
		if(this->outSide!=NULL){
			return this->outSide->search(name);
		}else{
			cout<<name<<endl;
			error("undefined symbol");
		}
	}
	/*
	int update(tableNode* t){
		for(int i = 0; i < cnt; i++){
			if(strcmp(t->name,table[i]->name)==0){
				table[i]->val = t->val;
				strcpy(table[i]->str,t->str);
				return table[i]->offset;
			}
		}
		if(this->outSide!=NULL){
			return this->outSide->update(t);
		}else{
			error("undefined symbol");
		}
	}*/
	void modifyRecentType(nodeType t){
		table[cnt-1]->type = t;
	}
	void modifyRecentNodeName(char*name){

		strcpy(table[cnt-1]->name,name);
	}
	void modifyRecentWidth(int len,nodeType type){
		if(type==SINT){
			table[cnt-1]->width = len*4;
			table[cnt-1]->arrayDim = len;
			//initArray(len);
		}else{
			table[cnt-1]->width = len;
		}
	}
	/*void initArray(int dim){
		//printf("\tpushl %%ebp\n");
		//printf("\tmovl %%esp, %%ebp\n");
		printf("\tsubl $%d, %%esp\n",dim*4);
	}*/
	void addToRecentArray(SA* iA){
		char* t = codeGen.gencode(iA->val,table[cnt-1]->arrayDim*4+table[cnt-1]->offset);
		iA->code[iA->numLine] = new char[strlen(t)];
		strcpy(iA->code[iA->numLine],t);
		iA->numLine++;
		table[cnt-1]->arrayDim --;
	}
	void outputDataSeg(){
		printf(".section .data\n");
		for(int i = 0; i < cnt;i++){
			if(table[i]->type==SSTR){
				printf("%s .ascii ",table[i]->name);
				puts(table[i]->str);
			}
		}
	}
}*globalTable;
