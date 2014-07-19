//
//  vector3D.h
//  vector3D
//
//  Created by 川口大志 on 2014/07/16.
//
//

#ifndef __vector3D__vector3D__
#define __vector3D__vector3D__

#include "ofMain.h"

class vector3D {
public:
    float x, y, z;
    
    vector3D() {}
    vector3D(const vector3D &a) : x(a.x), y(a.y), z(a.z) {}
    vector3D(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}
    
    vector3D &operator = (const vector3D &a) {
        x = a.x; y = a.y; z = a.z;
        return *this;
    }
    
    bool operator == (const vector3D &a) const {
        return x==a.x && y==a.y  && z==a.z;
    }
    
    bool operator != (const vector3D &a) const {
        return x!=a.x || y!=a.y  || z!=a.z;
    }
    
    
    void zero() { x = y = z = 0.0f; }
    
    vector3D operator -() const {return vector3D(-x, -y, -z);}
    
    vector3D operator +(const vector3D &a) {
        return vector3D(x + a.x, y + a.y, z + a.z);
    }
    
    vector3D operator -(const vector3D &a) {
        return vector3D(x - a.x, y - a.y, z - a.z);
    }
    
};



#endif /* defined(__vector3D__vector3D__) */
