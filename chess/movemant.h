//
//  movemant.h
//  chess
//
//  Created by מאירי פיניש on 25/12/2021.
//

#ifndef movemant_h
#define movemant_h

#include "position.h"

class Move{
    pos source;
    pos target;
public:
    Move(pos s, pos t):source(s),target(t){}
    pos getSource(){return source;}
    pos getTarget(){return target;}
};

class movemant:public Move{
    bool eat;
    bool hazraha;
    bool firstMove;
    bool change;
public:
    movemant(pos s, pos t, bool e, bool h, bool fm):eat(e),hazraha(h),firstMove(fm),change(false),Move(s,t){}
//  movemant(pos s, pos t):source(s),target(t),eat(false),hazraha(false),firstMove(false),change(false){}
//    pos getSource(){return source;}
//    pos getTarget(){return target;}
    bool getEat(){return eat;}
    bool getHazraha(){return hazraha;}
    bool getFM(){return firstMove;}
    bool getChange(){return change;}
    void setChange(bool c){change=c;}
};

#endif /* movemant_h */
