
from OpenGL.GL import *
from OpenGL.arrays import vbo
from OpenGL.GLU import *
from OpenGL.GLUT import *
from OpenGL.GL.framebufferobjects import *
from Image import *
import common
import numpy as ny
class particle(object):
    """description of class"""
    #def __init__(this,col,row,data=[0.1,0.2,0.3,0.4]):
    def __init__(this,*args):
        this.imageFormat = GL_FLOAT
        if len(args) == 1 :
            if isinstance(args[0],Image):
                image = args[0]
                this.width = image.size[0]
                this.height = image.size[1]
                this.data = image.im
                this.imageFormat = GL_UNSIGNED_BYTE
            if isinstance(args[0],list):
                listf = args[0]
                this.width = len(listf) if len(listf) > 0 else 1
                this.height = 1
                this.data = listf        
        if len(args) == 3:
            this.width = int(args[0])
            this.height = int(args[1])
            this.data = args[2]  
        this.imagedata = []
        for i in range(this.height):
            for j in range(this.width):
                index = i * this.width + j
                f4 = this.data[index] if index < len(this.data) else float(j)/float(this.width)
                r,g,b,a = 0.0,0.0,0.0,0.0
                if isinstance(f4,tuple):
                    if len(f4) == 2:
                        r,g = f4
                    elif len(f4) == 3:
                        r,g,b = f4
                    else:
                        r,g,b,a = f4                           
                else:
                    r = f4
                this.imagedata.append(r)                          
                this.imagedata.append(g)
                this.imagedata.append(b)
                this.imagedata.append(a)
        #this.imagedata = ny.array(this.imagedata,dtype=ny.float)
        #print this.imagedata
    def createFBO(this):
        this.fbo = glGenFramebuffers(1) 
        glBindFramebuffer(GL_FRAMEBUFFER,this.fbo)
        #create texture for fbotext
        this.fbotext = glGenTextures(1)
        glBindTexture(GL_TEXTURE_2D,this.fbotext)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this.width, this.height,0,GL_RGBA,GL_FLOAT,None)#GL_FLOAT
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE)
        #relation fbo and fbotext(FBO and Texture2D)
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,this.fbotext,0)                                                                                                     #glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,this.fbotext,0)
        glBindFramebuffer(GL_FRAMEBUFFER,0)    
        #save data to texture.
        this.fbodata = glGenTextures(1)
        glBindTexture(GL_TEXTURE_2D,this.fbodata)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE)
        print this.imageFormat
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this.width, this.height, 0, GL_RGBA,this.imageFormat, this.imagedata)
    #cpu to gpu,and gpu compute result in fbo and fbotext.(cpu -(fbodata)> gpu -(fbotext)> cpu)
    def renderFBO(this,shader):
        glDisable(GL_DEPTH_TEST)
        glBindFramebuffer(GL_FRAMEBUFFER,this.fbo)
        glPushAttrib(GL_VIEWPORT_BIT)#| GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)        
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()  
        gluOrtho2D(0.0,this.width,0.0,this.height)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity() 
        glViewport(0,0,this.width,this.height)
        glActiveTexture(GL_TEXTURE0)
        glBindTexture(GL_TEXTURE_2D, this.fbodata)
        glUseProgram(shader)
        glUniform1i(shader.tex0, 0) 
        glUniform1f(shader.xl, this.width) 
        glUniform1f(shader.yl, this.height)
        glBegin(GL_QUADS)
        glVertex2f(0.0, 0.0)
        glVertex2f(this.width, 0.0)
        glVertex2f(this.width, this.height)
        glVertex2f(0.0, this.height)
        glEnd()
        glUseProgram(0) 
        glBindFramebuffer(GL_FRAMEBUFFER,0)

        glBindFramebuffer(GL_FRAMEBUFFER,this.fbo)
        glReadBuffer(GL_COLOR_ATTACHMENT0_EXT)
        data = glReadPixels(0, 0, this.width, this.height,GL_RGBA,GL_FLOAT)  
        print "fbo data:",type(data),len(data),data[0],data[1]#,data[2]
        glPopAttrib()
        glBindFramebuffer(GL_FRAMEBUFFER,0)   
        #close fbo
        glBindTexture(GL_TEXTURE_2D, 0)        
        glEnable(GL_DEPTH_TEST)       

    def render(this,shader):
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()  
        gluOrtho2D(0.0,this.width*2,0.0,this.height*2)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity() 
        glViewport(0,0,this.width,this.height)

        glColor4f(1.0, 1.0, 1.0, 0.5)
        glActiveTexture(GL_TEXTURE0)
        glEnable(GL_TEXTURE_2D)
        glBindTexture(GL_TEXTURE_2D, this.fbodata)
        glBegin(GL_QUADS)
        glTexCoord2f(0.0, 0.0)
        glVertex2f(0.0, 0.0)
        glTexCoord2f(1.0, 0.0)
        glVertex2f(this.width, 0.0)
        glTexCoord2f(1.0, 1.0)
        glVertex2f(this.width, this.height)
        glTexCoord2f(0.0, 1.0)
        glVertex2f(0.0, this.height)
        glEnd()

        glTranslatef(this.width+10, 0.0, 0.0)        
        glColor4f(1.0, 1.0, 1.0, 0.5)
        glActiveTexture(GL_TEXTURE0)
        glEnable(GL_TEXTURE_2D)
        glBindTexture(GL_TEXTURE_2D, this.fbotext)
        glBegin(GL_QUADS)
        glTexCoord2f(0.0, 0.0)
        glVertex2f(0.0, 0.0)
        glTexCoord2f(1.0, 0.0)
        glVertex2f(this.width, 0.0)
        glTexCoord2f(1.0, 1.0)
        glVertex2f(this.width, this.height)
        glTexCoord2f(0.0, 1.0)
        glVertex2f(0.0, this.height)
        glEnd()   
               
        #glBindFramebuffer(GL_FRAMEBUFFER,this.fbo)
        #glReadBuffer(GL_COLOR_ATTACHMENT0_EXT)
        #data = glReadPixels(0, 0, this.width, this.height,GL_RGBA,GL_FLOAT)
        ##print data[0],data[1]   
        #glBindFramebuffer(GL_FRAMEBUFFER,0)     
        ##print data
        #glBindFramebuffer(GL_FRAMEBUFFER,0)
        #glRasterPos2iv((0,0))
        #glDrawPixels(this.width, this.height,GL_RGBA,GL_FLOAT,data)
        #glFlush()
     
        #this.render = glGenRenderbuffers(1)
        #glBindRenderbuffer(GL_RENDERBUFFER,this.render)
        #glRenderbufferStorage(GL_RENDERBUFFER,GL_COLOR_ATTACHMENT0,this.width,this.height)
        #glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,this.render)