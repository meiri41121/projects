//
//  game.h
//  chess
//
//  Created by מאירי פיניש on 19/12/2021.
//

#ifndef game_h
#define game_h

#include "piece.h"
#include <cstdlib>
#include <ctime>
#include "movemant.h"

using namespace std;

class game{
    int value;
    board b;
    list<piece> piecesW;
    list<piece> piecesB;
    list<piece> eaten;
    list<movemant> moves;
    list<Move> validMovesList;
    color col=WHITE;
    int index;
    
public:
    color getCol(){return col;}
    game(){
        for(int i=0;i<8;i++)
            piecesW.push_front(piece(pos(i+1,2), PAWN, WHITE));
        piecesW.push_front(piece(pos(1,1), ROOK, WHITE));
        piecesW.push_front(piece(pos(8,1), ROOK, WHITE));
        piecesW.push_front(piece(pos(2,1), KNIGHT, WHITE));
        piecesW.push_front(piece(pos(7,1), KNIGHT, WHITE));
        piecesW.push_front(piece(pos(3,1), BISHOF, WHITE));
        piecesW.push_front(piece(pos(6,1), BISHOF, WHITE));
        piecesW.push_front(piece(pos(4,1), QUEEN, WHITE));
        piecesW.push_front(piece(pos(5,1), KING, WHITE));
        for(int i=0;i<8;i++)
            piecesB.push_front(piece(pos(i+1,7), PAWN, BLACK));
        piecesB.push_front(piece(pos(1,8), ROOK, BLACK));
        piecesB.push_front(piece(pos(8,8), ROOK, BLACK));
        piecesB.push_front(piece(pos(2,8), KNIGHT, BLACK));
        piecesB.push_front(piece(pos(7,8), KNIGHT, BLACK));
        piecesB.push_front(piece(pos(3,8), BISHOF, BLACK));
        piecesB.push_front(piece(pos(6,8), BISHOF, BLACK));
        piecesB.push_front(piece(pos(4,8), QUEEN, BLACK));
        piecesB.push_front(piece(pos(5,8), KING, BLACK));
        value=0;
        allValidMoves();
        int n=1;
        while(n)
        {
            cout<<"enter 1 to play against friend or 2 to play against the computer, or 0 to exit\n";
            cin>>n;
            if(n==1)
                playTwoPlayers();
            if(n==2){
                cout<<"chosse difficulty: [1-5]";
                cin>>n;
                while(n<1 || n>5) cin>>n;
                srand(time(NULL));
                int random_number = rand()%2;
                color pcColor=random_number?BLACK:WHITE;
                cout<<"you are "<<(pcColor?"white":"black")<<" player\n";
                if(pcColor)
                    play();
                playAgainstPc(n+1);
            }
        }
        
    }
    
    void playAgainstPc(int n)
    {
        if(checkMat()) return;
        doBestMove(n);
        col = col?WHITE:BLACK;
        if(checkMat()) return;
        play(true);
        playAgainstPc(n);
    }

    Move doRandomMove()
    {
        srand(time(NULL));
        int random_number = rand()%(validMovesList.size());
        list<Move>::iterator it=validMovesList.begin();
        while(random_number--)
            it++;
        move(it->getTarget().posX, it->getTarget().posY, it->getSource().posX, it->getSource().posY,true);
        return Move(it->getSource(),it->getTarget());
    }
    
    void doBestMove(int n)
    {
        print();
        Move p = getBestMove(n);
        move(p.getTarget().posX,p.getTarget().posY,p.getSource().posX,p.getSource().posY,true);
        cout<<"pc move:"<<char(p.getSource().posX+'a'-1)<<p.getSource().posY<<" -> "<<char(p.getTarget().posX+'a'-1)<<p.getTarget().posY<<endl;
    }
    
//    Move getBestMove(int n=1)
//    {
//        index=0;
//        Move p=doRandomMove();
//        int v=BM(n-1),current=value;
//        undo(col?WHITE:BLACK);
//        if(n==0) return p;
//        allValidMoves();
//        list<Move> updateValidMovesList(validMovesList);
//        for(list<Move>::iterator it=updateValidMovesList.begin(); it!=updateValidMovesList.end();it++)
//        {
//            move(it->getTarget().posX, it->getTarget().posY, it->getSource().posX, it->getSource().posY,true);
//            int val = BM(n-1);
//            if((col && val<v) || (!col && val>v))
//            {
//                current=value;
//                v=val;
//                p=Move(it->getSource(),it->getTarget());
//            }
//            undo(col?WHITE:BLACK);
//        }
//        cout<<index<<endl;
//        return p;
//    }
//
//    int BM(int n)
//    {
//        index++;
//        if (n==0) return value;
//        int v=value;
//        col=col?WHITE:BLACK;
//        allValidMoves();
//        if(validMovesList.empty() && check()) v= col? 1000:-1000;
//        list<Move> updateValidMovesList(validMovesList);
//        for(list<Move>::iterator it=updateValidMovesList.begin(); it!=updateValidMovesList.end();it++)
//        {
//            move(it->getTarget().posX, it->getTarget().posY, it->getSource().posX, it->getSource().posY,true);
//            int val=BM(n-1);
//            if((col && val<v) || (!col && val>v))
//                v=val;
//            undo(col?WHITE:BLACK);
//        }
//        col=col?WHITE:BLACK;
//        return v;
//    }
    
    Move getBestMove(int n=1)
    {
        index=0;
        Move p=doRandomMove();
        int a=-5000,b=5000,v=BM(n-1,a,b),current=value;
        undo(col?WHITE:BLACK);
        if(n==0) return p;
        allValidMoves();
        list<Move> updateValidMovesList(validMovesList);
        for(list<Move>::iterator it=updateValidMovesList.begin(); it!=updateValidMovesList.end();it++)
        {
            move(it->getTarget().posX, it->getTarget().posY, it->getSource().posX, it->getSource().posY,true);
            int val = BM(n-1,a,b);
            if((col && val<v) || (!col && val>v))
            {
                current=value;
                v=val;
                p=Move(it->getSource(),it->getTarget());
            }
            undo(col?WHITE:BLACK);
            a = (!col && v>a)? v:a;
            b = (col && v<b)? v:b;
            if(a>b) break;
        }
        cout<<index<<endl;
        return p;
    }
    
    int BM(int n,int a,int b)
    {
        index++;
        if (n==0) return value;
        int v=value,i=0;
        col=col?WHITE:BLACK;
        allValidMoves();
        if(validMovesList.empty())
            v= check()? (col? 1000:-1000): 0;
        list<Move> updateValidMovesList(validMovesList);
        for(list<Move>::iterator it=updateValidMovesList.begin(); it!=updateValidMovesList.end();it++)
        {
            move(it->getTarget().posX, it->getTarget().posY, it->getSource().posX, it->getSource().posY,true);
            int val=BM(n-1,a,b);
            if((col && val<v) || (!col && val>v))
                v=val;
            undo(col?WHITE:BLACK);
            a = (!col && v>a)? v:a;
            b = (col && v<b)? v:b;
            if(a>b) break;
        }
        col=col?WHITE:BLACK;
        return v;
    }
    
    void playTwoPlayers()
    {
        if(checkMat()) return;
        play();
        playTwoPlayers();
    }
    
    void play(bool pc=false){
        print();
        string move, turn=col?"BLACK":"WHITE",chec=check()?" [check!!]":"";
        bool flag=false;
        cout<<turn<<" move:"<<chec<<"\n";
        cin>>move;
        char * moveInChar=new char[move.length()];
        for (int i=0; i<move.length(); i++)
            moveInChar[i]=move[i];
        if(move=="print")
            for(list<Move>::iterator it=validMovesList.begin(); it!= validMovesList.end();++it)
                cout<<it->getSource().posX<<","<<it->getSource().posY<<" to "<<it->getTarget().posX<<","<<it->getTarget().posY<<endl;
        else if(move=="help"){
            Move p=getBestMove(4);
            cout<<char(p.getSource().posX+'a'-1)<<p.getSource().posY<<" -> "<<char(p.getTarget().posX+'a'-1)<<p.getTarget().posY<<endl;}
//        else if(move=="help1"){
//            Move p=getBestMove1(4);
//            cout<<char(p.getSource().posX+'a'-1)<<p.getSource().posY<<" -> "<<char(p.getTarget().posX+'a'-1)<<p.getTarget().posY<<endl;}
        else if(move.length()==2 && validTarget(moveInChar[0], moveInChar[1]))//PAWN move [example 'd4']
            flag = this->move(moveInChar[0]-'a'+1, moveInChar[1]-'1'+1, PAWN, 0, 0);
        else if(move.length()==3 && validTarget(moveInChar[1], moveInChar[2]) && what(moveInChar[0]))//other piece move [example 'Nf3']
            flag = this->move(moveInChar[1]-'a'+1, moveInChar[2]-'1'+1, what(moveInChar[0]), 0, 0);
        else if(move.length()==4 && moveInChar[1]=='x' && validTarget(moveInChar[0],'2') && validTarget(moveInChar[2], moveInChar[3])) //PAWN attack [example 'dxe5']
            flag = this->move(moveInChar[2]-'a'+1, moveInChar[3]-'1'+1, PAWN, moveInChar[0]-'a'+1, 0);
        else if(move.length()==5 && what(moveInChar[0]) && moveInChar[0]!='K' &&validTarget(moveInChar[1], moveInChar[2]) && validTarget(moveInChar[3], moveInChar[4]))//when there is several options to move [example 'Nf3d4']
            flag = this->move(moveInChar[3]-'a'+1, moveInChar[4]-'1'+1, what(moveInChar[0]), moveInChar[1]-'a'+1, moveInChar[2]-'1'+1);
        else if(move=="undo")
            {if(!pc) flag = undo();
                else if(moves.size()>1) {undo();undo(col?WHITE:BLACK);}}
        delete []moveInChar;
        if(!flag || (pc && move=="undo")) {allValidMoves(); play(pc);}
        else col = (col?WHITE:BLACK);
    }
    
    bool validTarget(char letter, char num)
    { return letter>='a' && letter<='h' && num>='1' && num<='8'; }
    
    player what(char c)
    {
        if(c=='N') return KNIGHT;
        else if(c=='B') return BISHOF;
        else if(c=='R') return ROOK;
        else if(c=='Q') return QUEEN;
        else if(c=='K') return KING;
        return PAWN;
    }
    
    bool move(int targetX, int targetY, player name, int sourceX, int sourceY, bool pcTurn=false)
    { return move(targetX,targetY,name,sourceX,sourceY,col,pcTurn);}
    bool move(int targetX, int targetY, int sourceX, int sourceY, bool pcTurn=false)
    {
        for(list<piece>::iterator it=col?piecesB.begin():piecesW.begin(); it!= (col?piecesB.end():piecesW.end());++it)
            if(it->getPos().equal(pos(sourceX,sourceY)))
                return move(targetX,targetY,it->getName(),sourceX,sourceY,col,pcTurn);
        return false;
    }
    bool move(int targetX, int targetY, player name, int sourceX, int sourceY, color col, bool pcTurn=false)
    {
        for(list<piece>::iterator it=col?piecesB.begin():piecesW.begin(); it!= (col?piecesB.end():piecesW.end());++it){
            //piece p(it->getPos(),it->getName(),it->getCol());
            if(it->getName()==name && (sourceX==0 || it->getPos().posX==sourceX) && (sourceY==0 || it->getPos().posY==sourceY))
                if(validMove(it, pos(targetX,targetY)))
                {
                    if(abs(it->getPos().posX-targetX)==2 && name==KING){//hazraha..
                        if(!(underthreat(it->getPos().posX,targetY, col) || underthreat(targetX,targetY, col) || underthreat((targetX+it->getPos().posX)/2,targetY, col)))
                            for(list<piece>::iterator ro=col?piecesB.begin():piecesW.begin(); ro!= (col?piecesB.end():piecesW.end());++ro)
                                if(ro->getName()==ROOK && ro->getInit() && (ro->getPos().posX==targetX+1 || ro->getPos().posX==targetX-2))
                                {
                                    moves.push_front(movemant(pos(ro->getPos().posX,targetY),pos(ro->getPos().posX==8?6:4,targetY),0,0,ro->getInit()));
                                    b.move(ro->getPos().posX,targetY,ro->getPos().posX==8?6:4, targetY);
                                    value += ro->move(ro->getPos().posX==8?6:4, targetY);
                                    moves.push_front(movemant(pos(it->getPos().posX,targetY),pos(targetX, targetY),0,1,it->getInit()));
                                    b.move(it->getPos().posX,targetY,targetX, targetY);
                                    value += it->move(targetX, targetY);
                                    return true;
                                }
                        return false;
                    }
        
                    bool flagP = false, flag = false;
                    if(it->getName()==PAWN && it->getPos().posX!=targetX && b.empty(targetX, targetY))
                        flagP = true;
                    if(!b.empty(targetX, targetY) || flagP)
                        for(list<piece>::iterator die=col?piecesW.begin():piecesB.begin();!flag && die!= (col?piecesW.end():piecesB.end());++die)
                            if((die->getPos().posX==targetX && die->getPos().posY==targetY) || (flagP && die->getPos().equal(moves.front().getTarget())))
                                {kill(die); flag=true;}
                    moves.push_front(movemant(it->getPos(),pos(targetX, targetY),flag,0,it->getInit()));
                    b.move(it->getPos().posX,it->getPos().posY,targetX, targetY);
                    value += it->move(targetX, targetY);
                    if(((!col && targetY==8) || (col && targetY==1)) && name==PAWN){
                        char c='K';
                        if(!pcTurn){
                            cout<<"choose change\n";
                            while(c!='B' && c!='N' && c!='Q' && c!='R') cin>>c;}
                        value += it->become(what(pcTurn?'Q':c));
                        moves.front().setChange(true);
                    }
                    return true;
                }}
        return false;
    }
    
    void kill(list<piece>::iterator it)
    {
        value-=it->getValue();
        eaten.push_front(piece(it->getPos(),it->getName(),it->getCol(),it->getInit()));
        b.setVal(it->getPos().posX, it->getPos().posY, 0);
        it->getCol()?piecesB.erase(it):piecesW.erase(it);
    }
    
    string printPieceByPosition(int x,int y, color co)
    {
        string s="";
        for (auto i: (co?piecesB:piecesW))
            if(i.getPos().posX==x && i.getPos().posY==y)
                switch (i.getName()) {
                    case PAWN:
                        s = "p";
                        break;
                    case KNIGHT:
                        s = "n";
                        break;
                    case BISHOF:
                        s = "b";
                        break;
                    case ROOK:
                        s = "r";
                        break;
                    case QUEEN:
                        s = "q";
                        break;
                    case KING:
                        s = "k";
                        break;
                }
        if(s=="")
            cout<<"error\n";
        s+=co?"d":"l";
        return s;
    }
    
    void print()
    {
        //cout<<"    | A  | B  | C  | D  | E  | F  | G  | H  |\n";
        cout<<(col?"1:  | ":"8:  | ");
        for (int i=(col?1:8),z=0; z<8; z++,  cout<<endl<<char((z!=8?(col?z+'1':'8'-z):' '))<<(z!=8?":  | ":""))
            for (int j=1; j<9; j++,cout<<" | ")
                cout<<((b.getVal(j,(col?(i+z):(i-z)))!=0)?printPieceByPosition(j,(col?(i+z):(i-z)),b.getVal(j,(col?(i+z):(i-z)))==1?WHITE:BLACK):"  ");
        cout<<"   | A  | B  | C  | D  | E  | F  | G  | H  |\n";
    }
    
    void printB()
    {
        cout<<"\n| ";
        for (int i=8; i>0; i--,cout<<endl<<"| ")
            for (int j=1; j<9; j++,cout<<" | ")
                cout<<b.getVal(j,i);
    }
    
    bool underthreat(int targetX, int targetY, color col)
    {
        for(list<piece>::iterator it=(col?piecesW.begin():piecesB.begin());it!= (col?piecesW.end():piecesB.end());++it)
            if(it->validMove(b, pos(targetX,targetY)))
                return true;
        return false;
    }

    bool check(){return check(col);}
    bool check(color col)
    {
        for(list<piece>::iterator it=col?piecesB.begin():piecesW.begin(); it!= (col?piecesB.end():piecesW.end());++it)
            if(it->getName()==KING)
                return underthreat(it->getPos().posX,it->getPos().posY, col);
        return false;
    }

    bool checkMat()
    {
        allValidMoves();
        if(!check() && checkPat()) return true;
        if(!(validMovesList.empty())) return false;
        print();
        cout<<(col?"\nWHITE":"\nBLACK")<<" player win!!!\n";
        return true;
    }
    
    bool checkPat()
    {
        if(validMovesList.empty()){
            print();
            cout<<"Pat!! game over\n";
            return true;
        }
        //if()//if its repeat himself
        return false;
    }
    
    bool validMove(list<piece>::iterator player, pos target){return validMove(player, target, col);}
    bool validMove(list<piece>::iterator player, pos target, color col)
    {
        bool flagP=false, flag=false;
        if(player->getName()==PAWN && moves.front().getFM() && moves.front().getTarget().posY==(player->getCol()?4:5) && moves.front().getSource().posY==(player->getCol()?2:7) && pos(moves.front().getTarget().posX,(player->getCol()?3:6))==target && abs(player->getPos().posX-target.posX)==1 && player->getPos().posY==(player->getCol()?4:5))
            {b.setVal(target.posX, target.posY, col?1:2); flagP=true;}
        if(player->validMove(b, target))
        {
            if(player->getName()==KING && abs(player->getPos().posX-target.posX)==2){
                if(!(underthreat(player->getPos().posX,player->getPos().posY, col) || underthreat(target.posX,target.posY, col) || underthreat((target.posX+player->getPos().posX)/2,target.posY, col)))
                    for(list<piece>::iterator ro=col?piecesB.begin():piecesW.begin(); ro!= (col?piecesB.end():piecesW.end());++ro)
                        if(ro->getName()==ROOK && ro->getInit() && (ro->getPos().posX==target.posX+1 || ro->getPos().posX==target.posX-2))
                            return true;
                return false;}
            if(!b.empty(target.posX, target.posY))
                for(list<piece>::iterator it=player->getCol()?piecesW.begin():piecesB.begin();!flag && it!= (player->getCol()?piecesW.end():piecesB.end());++it)
                    if(it->getPos().posX==target.posX && (it->getPos().posY==target.posY || (flagP && it->getPos().posY==(getCol()?4:5))))
                    {kill(it); flag=true;}
            b.move(player->getPos().posX,player->getPos().posY,target.posX, target.posY);
            moves.push_front(movemant(player->getPos(),target,flag,0,player->getInit()));
            value += player->move(target.posX, target.posY);
            flag=check(player->getCol());
            undo(player->getCol()?WHITE:BLACK);
            return !flag;
        }
        return false;
    }
    
    bool undo(){return undo(col);}
    bool undo(color col)
    {
        if(moves.empty()) return false;
        movemant m(moves.front());
        moves.pop_front();
        for(list<piece>::iterator it = (col?piecesW.begin():piecesB.begin()); it!= (col?piecesW.end():piecesB.end()); ++it)
            if(it->getPos().posX==m.getTarget().posX && it->getPos().posY==m.getTarget().posY)
                {value += it->move(m.getSource().posX, m.getSource().posY,m.getFM());
                if(m.getChange()) value += it->become(PAWN);}
        b.move(m.getTarget().posX, m.getTarget().posY, m.getSource().posX, m.getSource().posY);
        if(m.getEat())
        {
            col? piecesB.push_front(piece(eaten.front())) : piecesW.push_front(piece(eaten.front()));
            b.getBack(eaten.front().getPos().posX,eaten.front().getPos().posY, col?2:1);
            value += eaten.front().getValue();
            eaten.pop_front();
        }
        return m.getHazraha()? undo(col):true;
    }

    void allValidMoves(){allValidMoves(col);}
    void allValidMoves(color col)
    {
        validMovesList.clear();
        for(list<piece>::iterator it = (col?piecesB.begin():piecesW.begin()); it!= (col?piecesB.end():piecesW.end()); ++it)
            for(int i=1;i<9;i++)
                for(int j=1;j<9;j++){
                    //piece p(it->getPos(),it->getName(),it->getCol());
                    if(validMove(it, pos(i,j)))
                        validMovesList.push_front(Move(it->getPos(),pos(i,j)));}
    }
};

#endif /* game_h */
