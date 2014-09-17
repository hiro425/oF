//
//  createAction.h
//  visualBand_fork
//
//  Created by 川口大志 on 2014/09/11.
//
//

#ifndef __visualBand_fork__createAction__
#define __visualBand_fork__createAction__

#include "ofMain.h"
#include "defines.h"


class createAction {
public:
    createAction() {}
    void setup(property &p);
    void update(property &p);
    void callback(property &p);
    void draw();
    
    property prop;
    ActionType aType;
    

};
#endif /* defined(__visualBand_fork__createAction__) */
