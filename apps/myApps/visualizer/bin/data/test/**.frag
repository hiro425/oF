/**
* Original code by Paulo Falcao
* adapted for beginners by Jaksa Vuckovic
* Amiga rules!
*/

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;

vec3 light1 = vec3(20,20,0);
vec3 light2 = vec3(-20,0,0);

struct sphere {
    vec3 origin;
    float radius;
};

sphere createSphere(vec3 origin, float radius) {
    sphere s;
    s.origin = origin;
    s.radius = radius;
    return s;
}

sphere spheres[16];
int numSpheres;


// given a point returns the distance from the floor object
float obj_floor(in vec3 p)
{
  return p.y+10.0;
}

// give a point returns the distance from the sphere
float obj_sphere(in vec3 p, sphere s) {
  return length(p - s.origin) - s.radius + cos(time*1.64+p.x+pow(p.y,1.4))*20.; // hidden space
}

float smin( float a, float b, float k )
{
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

float distance_to_blob(vec3 p) {
    float min_dist = 99999.0;
    for (int i = 0; i < 16; i++) {
        float dist = obj_sphere(p, spheres[i]);
        min_dist = smin(min_dist, dist, 4.0);
    }
    return min_dist;
}

// given a point returns the distance to the nearest object
float distance_to_obj(in vec3 p)
{
  return min(distance_to_blob(p), obj_floor(p));
}

// procedural definition of Floor Color (checkerboard)
// given a point in the 3d space returns the texture color of the floor at that point
// this implementation ignores the y axis
vec3 floor_color(in vec3 p)
{
    if ((fract(p.z * 0.2) > 0.5) ^^ (fract(p.x * 0.2) > 0.5))
      return vec3(.8,0,0);
    else
      return vec3(.8,.8,.8);
}

vec3 calc_normal(vec3 p) {
    vec3 e = vec3(0.01, 0.0, 0.0);
    vec3 n;
    n.x = distance_to_obj(p + e.xyy) - distance_to_obj(p - e.xyy);
    n.y = distance_to_obj(p + e.yxy) - distance_to_obj(p - e.yxy);
    n.z = distance_to_obj(p + e.yyx) - distance_to_obj(p - e.yyx);
    return normalize(n);
}


void initSpheres(void) {
    for (int i = 0; i < 15; i++) {
        spheres[i] = createSphere(vec3(15.0*cos(float(i)/8.0+time/8.0), 0, 15.0*sin(float(i)/8.0+time)), sin(float(i)/4.0)+.6);
    }
    
}


void main( void ) {
    initSpheres();
    vec2 position = ( gl_FragCoord.xy / resolution.xy ); // position in range (0,0)..(1,1)
    vec2 screen_pos = (2.0 * position - 1.0); // position in range (-1,-1)..(1,1)
    vec2 mouseOnScreen = 2.0 * mouse -1.0;
    vec3 camera_up = vec3(0,1,0);
    vec3 scr_world_pos=vec3(cos(mouse.x*6.0)*50.0, mouse.y * 49.0, sin(mouse.x*6.0)*50.0); // position of the center of the screen in the 3d world
    vec3 camera_pos = vec3(cos(mouse.x*6.0)*51.0, mouse.y * 50.0, sin(mouse.x*6.0)*51.0);
    
    vec3 camera_dir = normalize(scr_world_pos - camera_pos);
    vec3 u = normalize(cross(camera_up, camera_dir));
    vec3 v = cross(camera_dir,u) * (resolution.y/resolution.x);
    vec3 scr_world_coord = scr_world_pos + screen_pos.x*u + screen_pos.y*v; // the point on the screen in the 3d world
    vec3 rayDirection=normalize(scr_world_coord-camera_pos);

    
    // Raymarching.
    const vec3 e = vec3(0.02,0,0);
    const float max_dist = 100.0; // Max depth
    float d = 0.02; // initial step
    vec3 p;
    
    float distance_from_camera = 1.0;
    for (int i=0; i<64; i++) {
        if ((abs(d) < .001) || (distance_from_camera > max_dist)) break;
        
        distance_from_camera += d;
        p = camera_pos + rayDirection*distance_from_camera;
        d = distance_to_obj(p);
    }
    
    if (distance_from_camera < max_dist) {
        
        vec3 c = floor_color(p);
        if (length(mouseOnScreen - screen_pos) < .1) c += (.5+c)*10.0*(.1-length(mouseOnScreen-screen_pos));
                
        vec3 normal = calc_normal(p);
        float lightAngle1 = max(0.01, dot(normalize(light1 - p), normal));
        float lightAngle2 = max(0.01, dot(normalize(light2 - p), normal));
            
        //simple phong lighting, LightPosition = CameraPosition
        gl_FragColor = vec4((lightAngle1*c + pow(lightAngle1,2.0) + 1.5*lightAngle2*c + pow(lightAngle2,2.0) + 0.2*c) * (1.0-distance_from_camera*.01), 1.0);
        
    } else {
        // we didn't hit anything, so paint the background
        gl_FragColor = vec4( 0,0,0, 1.0 );
    }

}
