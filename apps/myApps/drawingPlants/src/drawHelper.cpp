//
//  drawHelper.cpp
//  drawingPlants
//
//  Created by 川口大志 on 2014/09/21.
//
//

#include "drawHelper.h"

drawHelper &drawHelper::getInstance() {
    static drawHelper instance;
    return instance;
}