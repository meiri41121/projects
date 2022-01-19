//
//  board.h
//  chess
//
//  Created by מאירי פיניש on 19/12/2021.
//

#ifndef board_h
#define board_h

class board{
    int b[8][8];
public:
    board(){
        for (int i=0; i<2; i++)
            for (int j=0; j<8; j++)
                b[i][j]=1;//mark of WHITE
        for (int i=2; i<6; i++)
            for (int j=0; j<8; j++)
                b[i][j]=0;
        for (int i=6; i<8; i++)
            for (int j=0; j<8; j++)
                b[i][j]=2;//mark of BLACK
    }
    bool empty(int x, int y){
        return b[y-1][x-1]==0;
    }
    bool samePlayer(int sourceX, int sourceY, int targetX, int targetY){
        return b[targetY-1][targetX-1]==b[sourceY-1][sourceX-1];
    }
    void move(int sourceX, int sourceY, int targetX, int targetY)
    {
        b[targetY-1][targetX-1]=b[sourceY-1][sourceX-1];
        b[sourceY-1][sourceX-1]=0;
    }
    int getVal(int x, int y){return b[y-1][x-1];}
    void setVal(int x, int y, int val){b[y-1][x-1]=val;}
    void getBack(int x, int y, int color){b[y-1][x-1]=color;}
    
};


#endif /* board_h */
