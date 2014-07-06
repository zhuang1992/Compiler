# -*- coding: cp936 -*-
import os
from operator import itemgetter, attrgetter  
D={}
nonTerminal=[]
Terminal=[]
allS=[]
def getTerminal():
    for item in allS:
        if item not in nonTerminal:
            Terminal.append(item)

def loadSyntax():
    fp = open("in.txt",'r+')
    for line in fp:
        line = line.rstrip('\n \t');
        if len(line)==0:
            continue
        left = line.split(':')[0]
        right = line.split(':')[1]
        #print 'right:',right
        for r in right.split(' '):
            if r not in allS:
                allS.append(r)
        gen = tuple(right.split(' '))
        if(left not in D.keys()):
            nonTerminal.append(left)
            if left not in allS:
                allS.append(left)
            D[left] = [gen]
        else:
            p = D[left]
            p.append(gen)
            D[left] = p
    allS.append('#')
    getTerminal()
    fp.close()

    
FIRST={}
def getFirst ():
    for X in allS:
        first_single(X)
    isOver = False
    while isOver==False :
        isOver = True
        for X in nonTerminal :
            gen = D[X]
            for t in gen:
                if t[0] in nonTerminal:
                    for item in FIRST[t[0]]:
                        if item not in FIRST[X]:
                            FIRST[X].append(item);
                            isOver = False
                        
                        
                            
                            

def first_single(X): #求单个文法符号X的FIRST集合。这里文法右边都没有epsilon
    FIRST[X]=[]
    if X in Terminal:
        FIRST[X].append(X)
    else:
        right = D[X]
        for t in right:
            if t[0] in Terminal:
                FIRST[X].append(t[0])
    
        
   
def first_alpha(alpha):
    #print alpha
    return FIRST[alpha[0]]
    

#对项目求闭包。项目用如下形式表示：
#   一个产生式左端，一个整数记录是第几个产生式，一个整数记录状态点的位置，一个展望符a
#   以上内容用一个元组来表示(A,1,2,a)。一个状态是这样的类的list
#   注意点的位置为0时，表示在产生式最前面
#   当点的位置为-1时，表示已经到产生式的最后了

def closure(I):
     C = I
     for c in C:
         genName,genId,statePos,forwardChar = c
         beta=[]
         if statePos == -1:
             continue
         B = D[genName][genId][statePos]
         if B in Terminal:
             continue 
         statePos = statePos+1
         if statePos==len(D[genName][genId]):
             statePos = -1
         else:
             beta.append(D[genName][genId][statePos])
             #print "beta",beta
         beta.append(forwardChar)
         bGen = D[B]
         for i in range(0,len(bGen)):
             for t in first_alpha(beta):
                 newc = (B,i,0,t)
                 if newc not in C:
                     C.append(newc)
     return C         
         
stateI = [] #所有状态存在一个list里
def go(I,X):
    J = []
    for item in I:
        genName,genId,statePos,forwardChar = item
        if(statePos==-1):
            continue
        if D[genName][genId][statePos]==X :
            statePos = statePos+1
            if statePos==len(D[genName][genId]):
                statePos = -1
            J.append((genName,genId,statePos,forwardChar))
    return closure(J)

def findState():
    fout = open('state.txt','w');
    core0 = ('Sprime',0,0,'#')
    t=[]
    t.append(core0)
    stateI.append(closure(t))
    for si in stateI:
        for x in allS:
            l = go(si,x)
            if len(l)>0:
                l = sorted(l,key=itemgetter(0,1,3,2))
                #print 'sorted',l
                if l not in stateI:
                    print >>fout, l
                    stateI.append(l)
                    #print x
                    #print l
                    print len(stateI)
                    #os.system("pause")
    fout.close()

synList=[]
def changeSyn2List():
    for k in D.keys():
        l = [k]
        for v in D[k]:
            t = [k]
            t.extend(v)
            synList.append(t)
    #for item in synList:
      # print item

Action={}
Goto={}
#用j表示Sj，用-j-1表示rj(j>=0)
def makeTable():
    #print stateI[0]
    for item in stateI:
        for B in nonTerminal:
            temp = go(item,B)
            temp = sorted(temp,key=itemgetter(0,1,3,2))
            if temp in stateI:
                #print 'it worked',stateI.index(temp)
                Goto[stateI.index(item),nonTerminal.index(B)] = stateI.index(temp)
        for s in item:
            genName,genId,statePos,forwardChar = s
            f=''
            if statePos != -1:
                f = D[genName][genId][statePos]
                if f in Terminal:
                    temp = go(item,f)
                    temp = sorted(temp,key=itemgetter(0,1,3,2))
                    if temp in stateI:
                        Action[stateI.index(item),Terminal.index(f)] = stateI.index(temp)
            elif statePos == -1:
                syn = []
                syn.append(genName)
                syn.extend(D[genName][genId])
                Action[stateI.index(item),Terminal.index(forwardChar)] = -1 * synList.index(syn)-1;
            if genName == 'Sprime' and statePos == -1:
                Action[stateI.index(item),Terminal.index('#')] = 99999

#以下函数为输出到文件，供C++程序读取        
def outputSymbols():
    fout = open("Symbols.txt",'w')
    for item in nonTerminal:
        print >>fout, item
    print >>fout, '\n',
    for item in allS:
        if item not in nonTerminal:
            print >>fout, item
    fout.close()

def outputTables():
    fout = open("tables.txt",'w')
    for k in Action.keys():
        print >>fout, "%s %s"  % k, Action[k]
    print >>fout, '\n',
    for k in Goto.keys():
        print >>fout, "%s %s"  % k, Goto[k]
    fout.close()
    
def outputSyntax():
    fout = open("pySyntax.txt",'w')
    for k in D.keys():
        for v in D[k]:
            print >>fout, "%s:" % k,
            for s in v:
                print >>fout, "%s" % s,
            print >>fout, '\n',
    fout.close()

def outputState():
    fout = open("state.txt",'w')
    for item in stateI:
        print >>fout, item
    fout.close()
        
loadSyntax()
changeSyn2List()
getFirst()
outputSyntax()
outputSymbols()
findState()
print 'a'
makeTable()
print 'b'
#outputState()
outputTables()


#for k,v in FIRST.items():
#    print k,v
#print '\n\n\n'

#print D['assignment-expression']
#print FIRST['assignment-expression']
#print len(stateI)
#print '\n\n\n'
#outputState()
#outputTables()

#outputSymbols()
#print allS

#print D.values()


#for item in Goto.items():
#    print item

