//
//  Cell.h
//  cellAutomaton
//
//  Created by 川口大志 on 2014/04/29.
//
//

#ifndef __cellAutomaton__Cell__
#define __cellAutomaton__Cell__

#include <iostream>

class Cell {
    float x, y;
    float state;
    float nextState;
    float lastState = 0;
    Cell *neighbours;
public:
    Cell(float w, float h);
    void addNeighbour(Cell cell);
    void calcNextState();
    void drawMe();
};

#endif /* defined(__cellAutomaton__Cell__) */
