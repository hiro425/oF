#include "ofApp.h"
#include "GLFW/glfw3.h"
using std::ifstream;

const char* loadShaderAsString(const char* file);

//--------------------------------------------------------------
void ofApp::setup(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "cant open Window" << std::endl;
        glfwTerminate();
        
        return -1;
    }
    getVersion();
    
    shader1.load("basic.vert", "basic.frag");
    
    // --------------------
    GLuint vertShader = glCreateShader( GL_VERTEX_SHADER );
    GLchar *shaderCode;
    std::ifstream shader_file("/Users/kawaguchihiroshi/Developer/practice/OpenGL/practiceGLSL2/practiceGLSL2/basic.vert", std::ifstream::in);
    //std::string str((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());
    
    shaderCode = (char *)malloc(10000);
    int i = 0;
    while (shader_file.good()) {
        int c = shader_file.get();
        shaderCode[i++] = c;
    }
    shader_file.close();
    shaderCode[i++] = '\0';

    
    const GLchar *codeArray[] = {shaderCode};
    glShaderSource( vertShader, 1, codeArray, NULL);
    free(shaderCode);
    
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );
    const GLchar *shaderCode2 = loadShaderAsString("/Users/kawaguchihiroshi/Developer/practice/OpenGL/practiceGLSL2/practiceGLSL2/basic.frag");
    const GLchar *codeArray2[] = {shaderCode2};
    glShaderSource( fragShader, 1, codeArray2, NULL);
    
    glCompileShader(fragShader);
    
    GLint result;
    glGetShaderiv( vertShader, GL_COMPILE_STATUS, &result );
    if(GL_FALSE==result) {
        fprintf(stderr, "miss\n");
        
        GLint logLen;
        glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char *log = (char *)malloc(logLen);
            GLsizei written;
            glGetShaderInfoLog(vertShader, logLen, &written, log);
            fprintf(stderr, "shader log: \n%s", log);
            free(log);
        }
    }
    
    GLint result2;
    glGetShaderiv( fragShader, GL_COMPILE_STATUS, &result2 );
    if(GL_FALSE==result2) {
        fprintf(stderr, "miss2\n");
        
        GLint logLen;
        glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char *log = (char *)malloc(logLen);
            GLsizei written;
            glGetShaderInfoLog(fragShader, logLen, &written, log);
            fprintf(stderr, "shader log: \n%s", log);
            free(log);
        }
    }
    
    GLuint programHandle = glCreateProgram();
    
    if(0==programHandle){
        fprintf(stderr, "object error\n");
    }
    
    glBindAttribLocation(programHandle, 0, "VertexPosition");
    glBindAttribLocation(programHandle, 1, "VertexColor");
    
    glAttachShader(programHandle, vertShader);
    glAttachShader(programHandle, fragShader);
    
    glLinkProgram(programHandle);
    GLint status;
    glGetProgramiv(programHandle,GL_LINK_STATUS, &status);
    if(GL_FALSE==status ) {
        fprintf(stderr, "error\n");
        
        GLint logLen;
        glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
        if(logLen > 0) {
            char *log = (char*)malloc(logLen);
            GLsizei written;
            glGetProgramInfoLog(programHandle, logLen, &written, log);
            fprintf(stderr, "program log: \n%s", log);
            free(log);
        }
        
    }
    else {
        glUseProgram(programHandle);
    }
}

const char* loadShaderAsString(const char* file)
{
    std::ifstream shader_file(file, std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());
    return str.c_str();
}

void ofApp::getVersion() {
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW err: %s\n", glewGetErrorString(err));
    }
    glm::vec4 position = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, 90.0f,  glm::vec3(0.0f, 1.0f, 0.0));
    
    glm::mat4 mv = view * model;
    
    glm::vec4 transformed = mv * position;
    
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *vendor   = glGetString(GL_VENDOR);
    const GLubyte *version  = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    
    printf("GL Vendor : %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version(string) : %s\n", version);
    printf("GL Version(integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);
    
    GLint nExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
    /*
     for (int i = 0; i<nExtensions; i++) {
     printf("here %s\n", glGetStringi(GL_EXTENSIONS, i));
     }*/
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
