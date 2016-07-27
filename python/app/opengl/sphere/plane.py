# -*- coding: utf-8 -*-
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.arrays import vbo
import numpy as ny
import common

class plane(common.common):
    def __init__(this,xres,yres,xscale,yscale):
        this.xr,this.yr,this.xc,this.yc = xres - 1,yres - 1,xscale,yscale
    def createVAO(this):
        helfx = this.xr * this.xc * 0.5
        helfy = this.yr * this.yc * 0.5
        vdata = []
        vindex = []
        for y in range(this.yr):
            for x in range(this.xr):
                vdata.append(this.xc * float(x) - helfx)
                vdata.append(0.)
                vdata.append(this.yc * float(y) - helfy)
        for y in range(this.yr - 1):
            for x in range(this.xr - 1):
                vindex.append((y + 0) * this.xr + x)
                vindex.append((y + 1) * this.xr + x)
                vindex.append((y + 0) * this.xr + x + 1)
                vindex.append((y + 0) * this.xr + x + 1)
                vindex.append((y + 1) * this.xr + x)
                vindex.append((y + 1) * this.xr + x + 1)
        print len(vdata),len(vindex)
        this.vbo = vbo.VBO(ny.array(vdata,'f'))
        this.ebo = vbo.VBO(ny.array(vindex,'H'),target = GL_ELEMENT_ARRAY_BUFFER)
        this.eboLength = len(vindex)
        this.bCreate = True
    def draw(this):
        if this.bCreate == False:
            this.createVAO()
        this.vbo.bind()
        glInterleavedArrays(GL_V3F,0,None)
        this.ebo.bind()
        glDrawElements(GL_TRIANGLES,this.eboLength,GL_UNSIGNED_SHORT,None)   
