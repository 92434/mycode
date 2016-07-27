
import sys
from OpenGL.GLUT import *
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GL import shaders
plane_v = """
        //#version 330 compatibility
        #version 120
        uniform sampler2D tex1;       
        void main() {            
            gl_TexCoord[0] = gl_MultiTexCoord0; 
		    vec4 v = vec4(gl_Vertex);		
		    //v.y = texture2D(tex1, gl_TexCoord[0].st).r/256.0;
		    gl_Position = gl_ModelViewProjectionMatrix * v;
        }"""

plane_f = """
        //#version 330 compatibility
        #version 120
        uniform sampler2D tex0;
        void main() {            
            vec4 color = texture2D(tex0, gl_TexCoord[0].st);//gl_TexCoord[0].st);vec2(0.3,0.3)
            gl_FragColor = color;//vec4( 0, 1, 0, 1 );//
        }"""
        
update_v = """        
        //#version 330 compatibility
        #version 130
        uniform sampler2D tex0;
        uniform float xw;
        uniform float yw;
        uniform float height;
        //the location of center of the sphere
        uniform vec2 xz;          
        uniform float sphereRadius; 
        uniform mat4 mMatrix;
        uniform mat4 vMatrix;
        uniform mat4 pMatrix;
        out vec4 o_color;          
        void main() {      
		    vec4 pos = vec4(gl_Vertex);              
            vec2 uv = vec2(xz/vec2(xw,yw) + vec2(0.5,0.5));
            uv.y = 1.0 - uv.y;
            vec3 rgb =  texture2D(tex0, uv).rgb;
		    pos.y = pos.y + sphereRadius + rgb.r;//height;//
            o_color = vec4(uv.x, uv.y, rgb.r, 1);
            gl_Position = pMatrix * vMatrix * mMatrix * pos;

            //vec4 v = vec4(gl_Vertex);
            //vec2 uv = vec2(xz/vec2(xw,yw) + vec2(0.5,0.5)).xy;
            //uv.y = 1.0 - uv.y;	
            //v.x = v.x + xz.x;
            //v.z = v.z + xz.y;	
		    //v.y = v.y + texture2D(tex0, uv).r+ sphereRadius;
            //o_color = vec4(uv.x,uv.y, 0, 1 );
		    //gl_Position = gl_ModelViewProjectionMatrix * v;
        }"""

update_f = """
        //#version 330 compatibility
        #version 130
        in vec4 o_color;  
        void main() {            
            //vec4 color = texture2D(tex1, gl_TexCoord[0].st);
            gl_FragColor = o_color;// vec4( 0, 1, 0, 1 );
        }"""

gpgpu_v = """
        //#version 330 compatibility
        #version 130
        out vec4 pos;
        void main() {  
            pos = vec4(gl_Vertex);
            //The following coding must be in fragment shader
            //vec2 xy = v.xy;
            //vec2 uv = vec2(xy/vec2(xw,yw)).xy;
            //o_color = texture2D(tex0, uv);
		    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        }"""

gpgpu_f = """
        //#version 330 compatibility
        #version 130
        in vec4 pos;
        uniform sampler2D tex0; 
        uniform float xw;
        uniform float yw;          
        void main() {             
            vec2 xy = pos.xy;
            vec2 uv = vec2(xy/vec2(xw,yw)).xy;            
            vec4 o_color = texture2D(tex0, uv);// vec4(uv.x,uv.y, 0, 1 );//   
            o_color = o_color + vec4(0.2);  
            gl_FragColor = o_color;
        }"""

class allshader:
    def __init__(this):
        this.planeProgram = shaders.compileProgram(shaders.compileShader(plane_v, GL_VERTEX_SHADER),
            shaders.compileShader(plane_f, GL_FRAGMENT_SHADER)) 
        #the parameter tex0 must be use in shaders,otherwise the
        #glGetUniformLocation get -1
        this.planeProgram.tex0 = glGetUniformLocation(this.planeProgram,"tex0")
        this.planeProgram.tex1 = glGetUniformLocation(this.planeProgram,"tex1")        
        print this.planeProgram.tex0,this.planeProgram.tex1

        this.updateProgram = shaders.compileProgram(shaders.compileShader(update_v, GL_VERTEX_SHADER),
            shaders.compileShader(update_f, GL_FRAGMENT_SHADER)) 
        this.updateProgram.xl = glGetUniformLocation(this.updateProgram,"xw")
        this.updateProgram.yl = glGetUniformLocation(this.updateProgram,"yw")  
        this.updateProgram.height = glGetUniformLocation(this.updateProgram,"height")
        this.updateProgram.sphereRadius = glGetUniformLocation(this.updateProgram,"sphereRadius")   
        this.updateProgram.tex0 = glGetUniformLocation(this.updateProgram,"tex0")
        this.updateProgram.xz = glGetUniformLocation(this.updateProgram,"xz")
        this.updateProgram.hight = glGetUniformLocation(this.updateProgram,"hight")
        this.updateProgram.mMatrix = glGetUniformLocation(this.updateProgram,"mMatrix")
        this.updateProgram.vMatrix = glGetUniformLocation(this.updateProgram,"vMatrix")
        this.updateProgram.pMatrix = glGetUniformLocation(this.updateProgram,"pMatrix")

        this.gpgpuProgram = shaders.compileProgram(shaders.compileShader(gpgpu_v, GL_VERTEX_SHADER),
            shaders.compileShader(gpgpu_f, GL_FRAGMENT_SHADER)) 
        this.gpgpuProgram.tex0 = glGetUniformLocation(this.gpgpuProgram,"tex0")
        this.gpgpuProgram.xl = glGetUniformLocation(this.gpgpuProgram,"xw")
        this.gpgpuProgram.yl = glGetUniformLocation(this.gpgpuProgram,"yw")          

