#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;



float sdTorus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float sdFloor(vec3 p, vec3 b) {
  return length(max(abs(p)-b,0.0));
}

float sdCylinder( vec3 p, vec3 c )
{
  return length(p.xz-c.xy)-c.z;
}

void main() {
    vec2 coords = gl_FragCoord.xy/resolution;

    vec3 ray_dir = normalize(vec3(coords.x-sin(time*0.5)*0.1, coords.y-.5, -1.+sin(time*.5)*.1));
    vec3 ray_orig = vec3(cos(time*.05)*50.,sin(time*.5)*50.-25.,100);
    float offs = 0.;
    float j;
    for(float i=0.; i<500.; i+=1.) {
        vec3 pos = vec3(ray_orig+ray_dir*offs);
            
        vec3 c = vec3(100);
        vec3 q = mod(pos,c)-0.5*c;
        
        float dist = sdCylinder(q, vec3(0,0,5));
        
        dist = min(dist, sdFloor(q, vec3(10,0,50)));
        dist = min(dist, sdFloor(q, vec3(50,0,10)));
        
        offs+=dist;
            j = i+(i*sin(time))/50.;
        if(abs(dist)<1e-3) break;
    }
    
    float c=j/50.;
    gl_FragColor=vec4(vec3(c*pow(cos(time),2.),c*pow(sin(time),2.),c*sin(sqrt(time))), 8.0);
    
}
