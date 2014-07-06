#include "head.h"
#include <set>
#include <stack>
#include <algorithm>
#include <iterator>
using namespace std;
char token[TOKENSIZE]= {0};
int iden_num = 0;
char iden_table[NAMENUM][TOKENSIZE];
int int_num = 0;
int num_table[INTNUM];

ifstream f("test3.c",ios::in);
char buf[2][BUFSIZE]={0};
int lexeme_begining = 0;
int forward_ptr = -1;
int bufId = 0;
bool file_over = 0;
void copy_token(int head, int tail, int bufId) {
    //int len = strlen(token)
    int len = strlen(token);
	if(len + tail - head + 1 > 128){
		cout<<"Token is too long"<<endl;
		exit(0);
	}
    for(int i = head; i <=tail ; i++) {
        token[i-head+len] = buf[bufId][i];
    }
}
key_word determine_reservedword(char * token){
	if(strcmp(token,"if")==0){
		return IF;
	}else if(strcmp(token,"else")==0){
		return ELSE;
	}else if(strcmp(token,"while")==0){
		return WHILE;
	}else if(strcmp(token,"for")==0){
		return FOR;
	}else if(strcmp(token,"int")==0){
		return INT;
	}else if(strcmp(token,"void")==0){
		return VOID;
	}else if(strcmp(token,"printf")==0){
		return PRINTF;
	}else if(strcmp(token,"scanf")==0){
		return SCANF;
	}else if(strcmp(token,"return")==0){
		return RETURN;
	}else if(strcmp(token,"main")==0){
		return MAIN;
	}
	return IDEN;
}
/*
void install_iden(char* token){
    for(int i = 0;i < iden_num;i++){
        if(strcmp(iden_table[i],token)==0){
            return ;
        }
    }
    strcpy(iden_table[iden_num++],token);
    //return iden_num-1;
}
*/
void check_border(int &lexeme_begining,int &forward_ptr,int &bufId, bool &file_over){
	if(forward_ptr == BUFSIZE) {
		copy_token(lexeme_begining,forward_ptr-1,bufId);
		forward_ptr = 0;
		lexeme_begining = 0;
		bufId = (bufId+1)%2;
		memset(buf[bufId],0,sizeof(buf[bufId]));
		if(f.fail()){
			file_over = 1;
		}
		f.read(buf[bufId],BUFSIZE);
	}	
}
wordItem lexcialAnalysis() {
    memset(token,0,sizeof(token));
	key_word resType;
	wordItem res;
	res.token=(char*)malloc(TOKENSIZE);
	res.id=-1;
	//if(f.fail())file_over = 1;
    while(forward_ptr < BUFSIZE) {
		if(file_over)break;
        lexeme_begining = forward_ptr + 1;
        forward_ptr = lexeme_begining;
		if(forward_ptr == BUFSIZE) {
			forward_ptr = 0;
			lexeme_begining = 0;
			bufId = (bufId+1)%2;
			memset(buf[bufId],0,sizeof(buf[bufId]));
			if(f.fail()){
				file_over = 1;
			}
			f.read(buf[bufId],BUFSIZE);     
		}
        char ch = buf[bufId][forward_ptr];  
        if(isalpha(ch)||ch=='_') {
            while(isalnum(ch)||ch=='_') {			     
				forward_ptr++;
				check_border(lexeme_begining,forward_ptr,bufId,file_over);
				ch = buf[bufId][forward_ptr];
            }
            forward_ptr--;
            copy_token(lexeme_begining,forward_ptr,bufId);
            key_word r=determine_reservedword(token);
            //if(r==IDEN)	install_iden(token);
			res.id = r;
            //cout<<token<<": "<<r<<endl;
			strcpy(res.token,token);
			return res;
        } else if(isdigit(ch)) {
            while(isdigit(ch)) {
                forward_ptr++;
                check_border(lexeme_begining,forward_ptr,bufId,file_over);
                ch = buf[bufId][forward_ptr];
            }
            forward_ptr--;
            copy_token(lexeme_begining,forward_ptr,bufId);
            //cout<<token<<": "<<CONSTINT<<","<<endl;
			//install_num(token);
            res.id = NUM;
			strcpy(res.token,token);
			return res;
        } else {
            switch(ch) {
            case '+':
                forward_ptr ++;
                check_border(lexeme_begining,forward_ptr,bufId,file_over);
                if((ch = buf[bufId][forward_ptr])=='+') {
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<PPLUS<<endl;
					res.id = PPLUS;
					strcpy(res.token,token);
					return res;
                } else {
                    forward_ptr --;
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<PLUS<<endl;
					res.id = PLUS;
					strcpy(res.token,token);
					return res;
                }
            case '-':
                forward_ptr ++;
                check_border(lexeme_begining,forward_ptr,bufId,file_over);
                if((ch = buf[bufId][forward_ptr])=='-') {
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<MMINUS<<endl;
					res.id = MMINUS;
					strcpy(res.token,token);
					return res;
                } else {
                    forward_ptr --;
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<MINUS<<endl;
					res.id = MINUS;
					strcpy(res.token,token);
					return res;
                }
            case '*':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<MULTI<<endl;
				res.id = MULTI;
				strcpy(res.token,token);
				return res;
            case '/':
				forward_ptr ++;
                check_border(lexeme_begining,forward_ptr,bufId,file_over);
                if((ch = buf[bufId][forward_ptr])=='*') {
                    forward_ptr ++;
					check_border(lexeme_begining,forward_ptr,bufId,file_over);
					ch = buf[bufId][forward_ptr];
					while(1){
						if(ch=='*'){
							forward_ptr ++;
							check_border(lexeme_begining,forward_ptr,bufId,file_over);
							if(buf[bufId][forward_ptr]=='/'){
								copy_token(lexeme_begining,forward_ptr,bufId);
								//cout<<token<<": "<<"This is comment"<<endl;
								res.id = IGNORED;
								strcpy(res.token,token);
								return res;
							}
						}else{
							forward_ptr ++;
							check_border(lexeme_begining,forward_ptr,bufId,file_over);
							ch = buf[bufId][forward_ptr];
						}
					}
                } else {
                    forward_ptr --;
                    copy_token(lexeme_begining,forward_ptr,bufId);
					//cout<<token<<": "<<DIVIDE<<endl;
					res.id = DIV;
					strcpy(res.token,token);
					return res;
                }
            case '=':
				forward_ptr ++;
                check_border(lexeme_begining,forward_ptr,bufId,file_over);
				if((ch = buf[bufId][forward_ptr])=='=') {
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<EQUAL<<endl;
					res.id = EQUAL;
					strcpy(res.token,token);
					return res;
                } else {
                    forward_ptr --;
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<ASSIGN<<endl;
					res.id = ASSIGN;
					strcpy(res.token,token);
					return res;
                }
            case ',':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<COMMA<<endl;
				res.id = COMMA;
				strcpy(res.token,token);
				return res;
            case ';':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<SEMI<<endl;
				res.id = SEMI;
				strcpy(res.token,token);
				return res;
            case '{':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<FBBRACE<<endl;
				res.id = BL_BRACE;
				strcpy(res.token,token);
				return res;
            case '}':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<SBBRACE<<endl;
				res.id = BR_BRACE;
				strcpy(res.token,token);
				return res;
            case '[':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<FMBRACE<<endl;
				res.id = ML_BRACE;
				strcpy(res.token,token);
				return res;
            case ']':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<SMBRACE<<endl;
                res.id = MR_BRACE;
				strcpy(res.token,token);
				return res;
            case '(':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<FSBRACE<<endl;
                res.id = SL_BRACE;
				strcpy(res.token,token);
				return res;
            case ')':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<SSBRACE<<endl;
                res.id = SR_BRACE;
				strcpy(res.token,token);
				return res;
            case '<':
                forward_ptr ++;
                check_border(lexeme_begining,forward_ptr,bufId,file_over);
                if((ch = buf[bufId][forward_ptr])=='=') {
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<LE<<endl;
					res.id = LE;
					strcpy(res.token,token);
					return res;
                } else {
                    forward_ptr --;
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<LESS<<endl;
					res.id = LESS;
					strcpy(res.token,token);
					return res;
                }
            case '>':
                forward_ptr ++;
                check_border(lexeme_begining,forward_ptr,bufId,file_over);
                if((ch = buf[bufId][forward_ptr])=='=') {
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<GE<<endl;
					res.id = GE;
					strcpy(res.token,token);
					return res;
                } else {
                    forward_ptr --;
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<MORE<<endl;
					res.id = GREATER;
					strcpy(res.token,token);
					return res;
                }
            case '#':
				while(ch!='\n'){
					forward_ptr++;
				    check_border(lexeme_begining,forward_ptr,bufId,file_over);
					ch = buf[bufId][forward_ptr];
				}
				forward_ptr --;
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<SHARP<<endl;
                res.id = SHARP;
				strcpy(res.token,token);
				return res;
            case '%':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<PERCENT<<endl;
				res.id = PERCENT;
				strcpy(res.token,token);
				return res;
            case '"':
                copy_token(lexeme_begining,forward_ptr,bufId);
                lexeme_begining++;
                forward_ptr++;
				check_border(lexeme_begining,forward_ptr,bufId,file_over);
                ch = buf[bufId][forward_ptr];
                while(ch!='"'){
                    forward_ptr++;
					check_border(lexeme_begining,forward_ptr,bufId,file_over);
                    ch = buf[bufId][forward_ptr];
                }
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<STR<<endl;
                res.id = STR;
				strcpy(res.token,token);
				return res;
            case '.':
                copy_token(lexeme_begining,forward_ptr,bufId);
                //cout<<token<<": "<<POINT<<endl;
				res.id = POINT;
				strcpy(res.token,token);
				return res;
            case '!':
				forward_ptr ++;
                check_border(lexeme_begining,forward_ptr,bufId,file_over);
                if((ch = buf[bufId][forward_ptr])=='=') {
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<NE<<endl;
					res.id = NEQ;
					strcpy(res.token,token);
					return res;
                } else {
                    forward_ptr --;
                    copy_token(lexeme_begining,forward_ptr,bufId);
                    //cout<<token<<": "<<NOT<<endl;
					res.id = NOT;
					strcpy(res.token,token);
					return res;
                }
            case ':':
            case '^':
            case '&':
            case '|':
            case '\\':
            case '~':
            case '?':
            case '\'':
            default:
                break;
            }
        }
    }
	if(res.id==-1){
		res.token[0]='#';
		res.token[1]='\0';
	}
	return res;
}
int synNum = 0;
int num_ter = 0;
int num_non_ter = 0;
char* Terminals[TER_NUM];
char* NonTerminals[NON_TER_NUM];
Syntax syntax[SYN_NUM];
void loadSymbols(){
	fstream fsym("Symbols.txt",ios::in);
	char buf[128];
	while(!fsym.eof()){
		fsym.getline(buf,128);
		if(strlen(buf)==0)break;
		NonTerminals[num_non_ter] = (char*)malloc(strlen(buf)+1);
		strcpy(NonTerminals[num_non_ter],buf);
		num_non_ter++;
	}
	while(!fsym.eof()){
		fsym.getline(buf,128);
		if(strlen(buf)==0)break;
		Terminals[num_ter] = (char*)malloc(strlen(buf)+1);
		strcpy(Terminals[num_ter],buf);
		num_ter++;
	}
	fsym.close();
}
void outputSymbols(){
	for(int i = 0 ; i < num_ter; i++){
		cout<<i<<" "<<Terminals[i]<<endl;
	}
	cout<<endl<<endl;
	for(int i = 0 ; i < num_non_ter; i++){
		cout<<i<<" "<<NonTerminals[i]<<endl;
	}
}
int Action[MAX_STATE][TER_NUM];
int Goto[MAX_STATE][NON_TER_NUM];
void loadTables(){
	for(int i = 0; i < MAX_STATE; i++){
		for(int j = 0; j < TER_NUM; j++){
			Action[i][j] = ERR_NUM;
		}
		for(int j = 0; j < NON_TER_NUM; j++){
			Goto[i][j] = ERR_NUM;
		}
	}
	fstream fT("tables.txt",ios::in);
	char buf[128];
	while(!fT.eof()){
		int action[3];
		int n=0;
		fT.getline(buf,128);
		if(strlen(buf)==0)break;
		int st = 0;
		for(int i = 0; i < strlen(buf); i++){
			if(buf[i]==' '){
				buf[i] = '\0';
				action[n++] = atoi(&buf[st]);
				buf[i] = ' ';
				st = i+1;
			}
		}
		action[n++] = atoi(&buf[st]);
		//cout<<action[0]<<" "<<Terminals[action[1]]<<" "<<action[2]<<endl;
		Action[action[0]][action[1]] = action[2];
	}
	//cout<<endl;
	while(!fT.eof()){
		int goTo[3];
		int n=0;
		fT.getline(buf,128);
		if(strlen(buf)==0)break;
		int st = 0;
		for(int i = 0; i < strlen(buf); i++){
			if(buf[i]==' '){
				buf[i] = '\0';
				goTo[n++] = atoi(&buf[st]);
				buf[i] = ' ';
				st = i+1;
			}
		}
		goTo[n++] = atoi(&buf[st]);
		//cout<<goTo[0]<<" "<<NonTerminal[goTo[1]]<<" "<<goTo[2]<<endl;
		Goto[goTo[0]][goTo[1]] = goTo[2];
	}

}
void loadSyntax(){
	memset(syntax,0,sizeof(syntax));
	fstream fsyn("pySyntax.txt",ios::in);
	char buf[128];
	while(!fsyn.eof()){
		fsyn.getline(buf,128);
		if(strlen(buf)==0)continue;
		int i = 0;
		for(i = 0; i < strlen(buf); i++){
			if(buf[i]==':'){
				buf[i] = '\0';
				break;
			}
		}
		strcpy(syntax[synNum].leftname,buf);
		buf[i] = ':';
		Node* ptr = syntax[synNum].right;
		i++;
		int st = ++ i;
		while(i<strlen(buf) && buf[i]!='\0'){
			if(ptr == NULL){
				syntax[synNum].right = new Node();
				ptr = syntax[synNum].right;
			}else{
				ptr->next = new Node();
				ptr = ptr->next;
			}
			while(buf[i]!=' ' && buf[i]!='\0'){
				i++;
			}
			if(i==strlen(buf)){
				strcpy(ptr->name,&buf[st]);
				break;
			}else{
				buf[i] = '\0';
				strcpy(ptr->name,&buf[st]);
				buf[i] = ' ';
			}			
			st = ++ i;
		}
		synNum++;
	}
}
bool isTerminal(char* str){
	for(int i = 0; i < TER_NUM; i++){
		if(strcmp(str,Terminals[i])==0){
			return true;
		}
	}
	return false;
}
void outputSyntax(){
	for(int i = 0; i < synNum; i++){
		Node* ptr = syntax[i].right;
		cout<<syntax[i].leftname<<":";
		cout<<ptr->name;
		ptr = ptr->next;
		while(ptr){
			cout<<" "<<ptr->name;
			ptr = ptr->next;
		}
		cout<<endl;
	}
}
wordItem getNextToken(){
	wordItem n = lexcialAnalysis();
	while(n.id==SHARP || n.id==IGNORED){
		n = lexcialAnalysis();
	}
	return n;
}
int indexOfNonTer(char* name){
	for(int i = 0; i < num_non_ter; i++){
		if(strcmp(name,NonTerminals[i])==0){
			return i;
		}
	}
	error("cannot find the correspoding NonTerminal index.");
}
int indexOfTer(char* name){
	for(int i = 0; i < num_ter; i++){
		if(strcmp(name,Terminals[i])==0){
			return i;
		}
	}
	error("cannot find the correspoding Terminla index.");
}
fstream re("result.txt",ios::out);
int semAction[MAX_GRAM];
void outPutOneSyn(int id){
	Node* ptr = syntax[id].right;
	re<<syntax[id].leftname<<": ";
	ptr = syntax[id].right;
	while(ptr!=NULL){
		re<<ptr->name<<" ";
		ptr = ptr->next;
	}
	re<<semAction[id]+1<<endl;
}
stack<int>stateID;
stack<SA*>prefix;
void buildTerStruct(wordItem t,SA* iS){
	iS->isTer = true;
	switch (t.id){
	case NUM:
		iS->val = atoi(t.token);
		iS->type=CONSTANT;
		break;
	case STR:
		strcpy(iS->sval,t.token);
		iS->type = SSTR;
		break;
	case IDEN:
		strcpy(iS->name,t.token);
		iS->type = SIDEN;
		break;
	//case VOID:
	case INT:
		iS->type = SINT;
		break;
	case FOR:
		iS->type = SFOR;
		break;
	case WHILE:
		iS->type = SWHILE;
		break;
	case IF:
		iS->type = SIF;
		break;
	default:
		iS->type = NONE;
	}
}
void buildNonTerStruct(int A,SA* iA){
	iA->id =A;
	iA->isTer = false;
	iA->type = NONE;
}
tableNode* makeTableNode(){
	tableNode* t = new tableNode();
	t->type = CONSTANT;
	return t;
}

void loadReflect(){
	fstream fin("reflect.txt",ios::in);
	while(!fin.eof()){
		int l,r;
		fin>>l>>r;
		semAction[l]=r;
	}
}
void passCode(SA* r,SA* iA){
	if(r->numLine==0)return;
	for(int i = 0;i < r->numLine; i++){
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],r->code[i]);
		iA->numLine++;
	}
	iA->mwidth=r->mwidth;
	//r->numLine = 0;
	//memset(r->code,0,sizeof(r->code));
}

void doSemantics(int gid, SA* parameter[], int paraNum,SA* iA,symbolTable* curTable, SA* STop){
	int aid = semAction[gid]+1;
	SA *r0,*r1,*r2,*r3,*r;
	tableNode *t,*t2;
	tableNode* nstr;
	switch (aid){
	case 1:
		r = parameter[0];
		t = curTable->search(r->name);
		iA->val = t->val;
		strcpy(iA->name,t->name);
		iA->type = t->type;
		break;
	case 2:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 3:
		r = parameter[1];
		iA->val = r->val;
		iA->type=r->type;
		passCode(r,iA);
		break;
	case 4:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 5:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 6:
		r2 = parameter[1];
		r1 = parameter[3];
		iA->val = r1->val;
		iA->type=r1->type;
		passCode(r1,iA);
		t = curTable->search(r1->name);
		t2 = curTable->search(r2->name);
		iA->code[iA->numLine] = new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genIndex(t2->offset));
		iA->numLine++;
		passCode(r2,iA);
		iA->code[iA->numLine] = new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genOp(t->offset+t->width,t2->offset));//err:应分开
		iA->numLine++;
		break;
	case 7:
		r1 = parameter[0];
		r0 = parameter[1];
		iA->val = r0->val;
		//forGen.setEndLine(true,r0->val);
		iA->type=r0->type;
		strcpy(iA->name,r0->name);
		t = curTable->search(iA->name);
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genIncDec(true,t->offset));
		iA->numLine++;
		//curTable->update();
		//curTable->update(iA);
		break;
	case 8:
		r1 = parameter[0];
		r0 = parameter[1];
		iA->val = r0->val;
		r0->val --;
		iA->type=r0->type;
		strcpy(iA->name,r0->name);
		t = curTable->search(iA->name);
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genIncDec(false,t->offset));
		iA->numLine++;
		//curTable->update(r0);
		break;
	case 9:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 10:
		r = parameter[0];
		iA->val = -1 * r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 11:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 12:
		break;
	case 13:
		break;
	case 14:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 15:
		r1 = parameter[0];
		r0 = parameter[2];
		iA->val = r0->val;
		iA->type=r0->type;
		passCode(r0,iA);
		passCode(r1,iA);
		strcpy(iA->name,r0->name);
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genMulti());
		iA->numLine++;
		break;
	case 16:
		r0=parameter[2];
		r1=parameter[0];
		iA->val = r0->val;
		iA->type=r0->type;
		passCode(r0,iA);
		passCode(r1,iA);
		strcpy(iA->name,r0->name);
		iA->code[iA->numLine]=new char[1024];
		t = curTable->search(r0->name);
		t2 = curTable->search(r1->name);
		strcpy(iA->code[iA->numLine],codeGen.genDiv(t->offset,t2->offset));
		iA->numLine++;
		break;
	case 17:
		break;
	case 18:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 19:
		r1 = parameter[0];
		r0 = parameter[2];
		iA->val = r0->val;
		iA->type=r0->type;
		passCode(r1,iA);
		passCode(r0,iA);
		strcpy(iA->name,r0->name);
		iA->code[iA->numLine]=new char[1024];
		t=curTable->search(r0->name);
		strcpy(iA->code[iA->numLine],codeGen.genAdd(t->offset));
		iA->numLine++;
		break;
	case 20:
		r1 = parameter[0];
		r0 = parameter[2];
		if(r1->type==CONSTANT){
			iA->val = r0->val;
			iA->type=r0->type;
			strcpy(iA->name,r0->name);
			t=curTable->search(r0->name);
			iA->code[iA->numLine]=new char[1024];
			strcpy(iA->code[iA->numLine],codeGen.genMinus(t->offset,r1->val,0));
			iA->numLine++;
		}else{
			iA->val = r1->val;
			iA->type = r1->type;
			strcpy(iA->name,r1->name);
			t=curTable->search(r1->name);
			iA->code[iA->numLine]=new char[1024];
			strcpy(iA->code[iA->numLine],codeGen.genMinus(t->offset,r0->val,1));
			iA->numLine++;
		}
		break;
	case 21:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 22:
		r2 = parameter[0];
		r1 = parameter[2];
		iA->val = r1->val < r2->val;
		t=curTable->search(r1->name);
		strcpy(iA->name,r1->name);
		iA->code[iA->numLine] = new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genCmp(r2->val,t->offset));
		strcat(iA->code[iA->numLine],codeGen.genJmp(0));
		iA->numLine++;
		break;
	case 25:
		r2 = parameter[0];
		r1 = parameter[2];
		iA->val = r1->val < r2->val;
		t=curTable->search(r1->name);
		strcpy(iA->name,r1->name);
		iA->code[iA->numLine] = new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genCmp(r2->val,t->offset));
		strcat(iA->code[iA->numLine],codeGen.genJmp(2));
		iA->numLine++;
		break;
	case 26:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 28:
		r2=parameter[0];
		r1=parameter[2];
		t=curTable->search(r1->name);
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genCmp(r2->val,t->offset));//r2-val err	
		strcat(iA->code[iA->numLine],codeGen.genJmp(1));
		iA->numLine++;
		break;
	case 29:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 31:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 33:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 34:
		r1 = parameter[2];
		r2 = parameter[0];
		r1->val = r2->val;
		//r1->type = r2->type;
		t = curTable->search(r1->name);
		t->val = r1->val;
		passCode(r2,iA);
		strcpy(iA->name,r1->name);
		iA->code[iA->numLine]=new char[1024];
		if(r2->type==CONSTANT){
			strcpy(iA->code[iA->numLine++],codeGen.genInsAssign(r1->val,t->offset));//立即数mov
		}else{
			strcpy(iA->code[iA->numLine++],codeGen.genAssign(t->offset));
		}
		//err:gencode
		iA->mwidth = t->width;
		break;
	case 35:
		printf("");
		break;
	case 41:
		r=parameter[0];
		//t = curTable->search(r->name);
		//iA->val=t->val;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 43:
		r0 = parameter[2];
		r1 = parameter[1];
		iA->type = r0->type;
		//strcpy(iA->sval,r->sval);
		//strcpy(iA->name,r->name);
		//iA->numv=r->numv;
		//iA->val=r->val;
		switch (STop->type){
		case SINT:
			codeGen.conEsp(r1->numv*4,iA);
			break;
		default:
			break;
		}
		passCode(r1,iA);
		//codeGen.outputCode(iA);
		break;
	case 44:
		//iA->type = SPECIFIER;
		iA->type = SINT;
		break;
	
	case 45:
		r = parameter[0];
		strcpy(iA->name,r->name);
		iA->type = r->type;
		t = curTable->search(iA->name);
		iA->numv = r->numv;
		r->mwidth = t->width;
		//strcpy(t->name,iA->name);
		//t->type = iA->type;
		//iA->mwidth=1;
		passCode(r,iA);
		//curTable->update(t);
		break;
	case 46:
		r1 = parameter[2];
		r2 = parameter[0];
		strcpy(iA->name,r2->name);
		iA->type = STop->type;
		iA->numv = r1->numv+r2->numv;
		passCode(r1,iA);
		passCode(r2,iA);
		//iA->mwidth = r1->mwidth+1;
		/*t = new tableNode();
		strcpy(t->name,iA->name);
		t->type = iA->type;
		curTable->update(t);*/
		break;
	case 47:
		r = parameter[0];
		iA->type = r->type;
		iA->numv=1;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 48:
		r1 = parameter[0];
		r0 = parameter[2];
		iA->type = r0->type;
		strcpy(iA->name,r0->name);
		iA->val = r0->val;
		iA->numv = r1->numv;
		//codeGen.conEsp(r0,iA);
		passCode(r1,iA);
		iA->mwidth = r0->mwidth;
		break;
	case 49:
		r = parameter[0];
		strcpy(iA->name,r->name);
		t = new tableNode();
		strcpy(t->name,r->name);
		t->type=STop->type;
		iA->type = STop->type;
		iA->val = curTable->insertTable(t);	
		break;
	case 50:
		r1 = parameter[3];
		r2 = parameter[1];
		strcpy(iA->name,r1->name);
		iA->type=STop->type;
		iA->val = r1->val;
		iA->numv = r2->val;
		curTable->modifyRecentWidth(iA->numv,iA->type);
		t = curTable->search(iA->name);
		iA->mwidth = t->width;
		break;
	case 51:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		iA->numv=1;
		passCode(r,iA);
		break;
	case 52:
		r = parameter[1];
		strcpy(iA->name,r->name);
		iA->numv = r->numv;
		passCode(r,iA);
		break;
	case 53:
		r0 = parameter[2];
		iA->type = r0->type;
		iA->numv = r0->numv;
		passCode(r0,iA);
		break;
	case 54:
		r1 = parameter[0];
		r0 = parameter[2];
		iA->type = r1->type;
		iA->numv = r0->numv+1;
		iA->val = r1->val;
		passCode(r0,iA);
		passCode(r1,iA);
		curTable->outSide->addToRecentArray(iA);
		break;
	case 55:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		iA->numv=1;
		passCode(r,iA);
		curTable->outSide->addToRecentArray(iA);
		break;
	case 56:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		passCode(r,iA);
		break;
	case 57:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		passCode(r,iA);
		break;
	case 58:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		passCode(r,iA);
		break;
	case 59:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		passCode(r,iA);
		break;
	case 60:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		passCode(r,iA);
		break;
	case 61:
		r = parameter[0];
		iA->val = r->val;
		iA->type=r->type;
		passCode(r,iA);
		break;
	case 62:
		r=parameter[1];
		iA->type = r->type;
		strcpy(iA->name,r->name);
		iA->numv=r->numv;
		iA->val=r->val;
		passCode(r,iA);
		break;
	case 63:
		r = parameter[0];
		iA->type = r->type;
		strcpy(iA->name,r->name);
		iA->numv=r->numv;
		iA->val=r->val;
		passCode(r,iA);
		//codeGen.conEsp(r,iA);
		//codeGen.outputCode(iA);
		//iA->numLine = 0;
		break;
	case 64:
		r2 = parameter[0];
		r1= parameter[1];
		iA->val = r2->val;
		iA->type = r2->type;
		passCode(r1,iA);
		passCode(r2,iA);
		//codeGen.conEsp(r,iA);
		//codeGen.outputCode(iA);
		//iA->numLine = 0;
		break;
	case 65:
		r = parameter[0];
		iA->type = r->type;
		strcpy(iA->name,r->name);
		iA->numv=r->numv;
		iA->val=r->val;
		passCode(r,iA);
		break;
	case 66:
		r = parameter[0];
		iA->val = r->val;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 67:
		r = parameter[1];
		iA->val = r->val;
		strcpy(iA->name,r->name);
		passCode(r,iA);
		break;
	case 68:
		break;
	case 69:
		r1= parameter[4];
		r2=parameter[2];
		r3=parameter[0];
		passCode(r1,iA);
		passCode(r2,iA);
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genJmp(3));
		iA->numLine++;
		codeGen.genLable(iA);
		passCode(r3,iA);
		codeGen.genLable(iA);
		break;
	case 70:
		r1 = parameter[2];
		r2 = parameter[0];
		codeGen.genLable(iA);
		//codeGen.outputCode(r1);
		passCode(r1,iA);
		//codeGen.outputCode(r2);
		passCode(r2,iA);
		codeGen.genLable(iA);
		break;
	case 71:
		r3 = parameter[0];
		r2 = parameter[2];
		r1 = parameter[4];
		r = parameter[6];
		//codeGen.conEsp(r->mwidth,iA);
		//codeGen.outputCode(r);
		passCode(r,iA);
		codeGen.genLable(iA);
		//codeGen.outputCode(r1);
		passCode(r1,iA);
		//codeGen.outputCode(r3);
		passCode(r3,iA);
		//codeGen.outputCode(r2);
		passCode(r2,iA);
		codeGen.genLable(iA);
		break;
	case 74:
		r=parameter[1];
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genRet(r->val));
		iA->numLine++;
		break;
	case 77:
		r1=parameter[4];
		r2=parameter[2];
		t=curTable->search(r2->name);
		nstr=new tableNode();
		strcpy(nstr->str,r1->sval);
		char label[10];
		sprintf(label,"L%d:",codeGen.slabelNum++);
		strcpy(nstr->name,label);
		nstr->str[strlen(nstr->str)-1]='\0';
		strcat(nstr->str,"\\0\"\0");
		nstr->type=SSTR;
		globalTable->insertTable(nstr);
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genPrintf(t->offset));
		iA->numLine++;
		break;
	case 78:
		r0=parameter[2];
		nstr=new tableNode();
		strcpy(nstr->str,r0->sval);
		//char label[10];
		sprintf(label,"L%d:",codeGen.slabelNum++);
		strcpy(nstr->name,label);
		nstr->str[strlen(nstr->str)-1]='\0';
		strcat(nstr->str,"\\0\"\0");
		nstr->type=SSTR;
		globalTable->insertTable(nstr);
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genPrintString());
		iA->numLine++;

		break;
	case 85:
		r = parameter[0];
		iA->type = r->type;
		strcpy(iA->name,r->name);
		iA->numv=r->numv;
		iA->val=r->val;
		passCode(r,iA);
		break;
	case 83:
		r = parameter[0];
		iA->type = r->type;
		strcpy(iA->name,r->name);
		iA->numv=r->numv;
		iA->val=r->val;
		passCode(r,iA);
		break;
	case 86:
		r=parameter[0];
		iA->code[iA->numLine]=new char[1024];
		strcpy(iA->code[iA->numLine],codeGen.genTextSection());
		iA->numLine++;
		passCode(r,iA);
		codeGen.outputCode(iA);
		globalTable->outputDataSeg();
		break;
	default:
		break;
	}
}
void LR1Analysis(){
	stateID.push(0);
	SA* end = new SA(SHARP);
	prefix.push(end);
	wordItem t = getNextToken();
	globalTable = new symbolTable();
	symbolTable* curTable=globalTable;
	//cout<<t.token<<" "<<t.id<<endl;
	
	while(!stateID.empty()){
		SA* S = new SA(stateID.top());
		int ip = t.id;
		if(ip==-1)ip=indexOfTer("#");
		int Si = Action[S->id][ip];
		if(Si==ERR_NUM){
			//cout<<t.token<<endl;
			error("This is not a valid input");
		}else if(Si == ACC ){
			//cout<<"Done"<<endl;
			return ;
		}else if(Si >= 0){						//移进
			stateID.push(Si);
			SA *iS=new SA(t.id);
			buildTerStruct(t,iS);
			prefix.push(iS);
			if(t.id==BL_BRACE){
				symbolTable* tTable = new symbolTable();
				curTable->inside = tTable;
				tTable->outSide = curTable;
				curTable = tTable;
			}else if(t.id == BR_BRACE){
				curTable = curTable->outSide;
				//delete(curTable->inside);
				//curTable->inside = NULL;
			}
			
			t = getNextToken();
			
			//cout<<t.token<<" "<<t.id<<endl;
			//ip = getNextToken();
		}else if(Si < 0){						//归约
			int tS = -Si - 1;
			Node* ptr = syntax[tS].right;
			SA *para[MAX_PARA_NUM];

			int numPara=0;
			while(ptr!=NULL){
				para[numPara++]=prefix.top();
				stateID.pop();
				prefix.pop();
				ptr = ptr->next;
			}
			int SS = stateID.top();
			int A = indexOfNonTer(syntax[tS].leftname);
			stateID.push(Goto[SS][A]);
			SA * iA = new SA(A);
			buildNonTerStruct(A,iA);
			SA* top;
			stack<SA*>tempS;
			while(prefix.top()->type==NONE){				
				tempS.push(prefix.top());
				prefix.pop();
			}
			top =  prefix.top();
			while(!tempS.empty()){
				prefix.push(tempS.top());
				tempS.pop();
			}
			doSemantics(tS,para,numPara,iA,curTable,top);
			prefix.push(iA);
			outPutOneSyn(tS);
		}
	}
}
void outputTables(){
	for(int i = 0; i < MAX_STATE; i++){
		for(int j = 0; j < TER_NUM; j++){
			if(Action[i][j]!=ERR_NUM)
				cout<<i<<" "<<j<<" "<<Action[i][j]<<endl;;
		}		
	}
	cout<<endl<<endl;
	for(int i = 0; i < MAX_STATE; i++){
		for(int j = 0; j < NON_TER_NUM; j++){
			if(Goto[i][j]!=ERR_NUM)
				cout<<i<<" "<<j<<" "<<Goto[i][j]<<endl;;
		}		
	}
}
void testGet(){
	while(1){
		wordItem t =getNextToken();
		cout<<t.token<<" "<<t.id<<endl;
		if(t.id==-1)break;
	}
}
int main(){
	//testGet();	
	loadSymbols();
	loadSyntax();
	loadTables();
	loadReflect();
	freopen("asm.S","w",stdout);
	LR1Analysis();
	//system("pause");
	return 0;
}