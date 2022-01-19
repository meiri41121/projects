//
//  position.h
//  chess
//
//  Created by מאירי פיניש on 19/12/2021.
//

#ifndef position_h
#define position_h

class pos{
public:
    int posX;
    int posY;
    pos():posX(0),posY(0){}
    pos(int x,int y):posX(x),posY(y){}
    bool operator == (pos& p) {return posX==p.posX && posY==p.posY;}
    void move(int targetX,int targetY) {posX=targetX; posY=targetY;}
    bool valid(){return (posX>0 && posX<9 && posY>0 && posY<9)? true:false;}
    bool equal(pos p){return posX==p.posX && posY==p.posY;}
};

#endif /* position_h */
