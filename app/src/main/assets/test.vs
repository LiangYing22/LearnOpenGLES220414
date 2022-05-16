attribute vec4 position;
uniform mat4 U_ModelMatrix;
uniform mat4 U_ViewMatrix;
uniform mat4 U_ProjectionMatrix;
void main(){
    gl_Position = U_ProjectionMatrix * U_ViewMatrix * U_ModelMatrix * position;
}