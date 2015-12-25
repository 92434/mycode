# -*- coding: utf-8 -*-
import math
from OpenGL.GL import *
from OpenGL.arrays import vbo
from OpenGL.GLU import *
from OpenGL.GLUT import *
#import OpenGL.GLUT as glut
import numpy as ny
#Python Imaging Library (PIL)
import common

#球的实现
class sphere(common.common):
    def __init__(this,rigns,segments,radius):
        this.rigns = rigns
        this.segments = segments
        this.radius = radius
    def createVAO(this):
        vdata = []
        vindex = []
        for y in range(this.rigns):
            phi = (float(y) / (this.rigns - 1)) * math.pi
            for x in range(this.segments):
                theta = (float(x) / float(this.segments - 1)) * 2 * math.pi
                vdata.append(this.radius * math.sin(phi) * math.cos(theta))
                vdata.append(this.radius * math.cos(phi))
                vdata.append(this.radius * math.sin(phi) * math.sin(theta))
                vdata.append(math.sin(phi) * math.cos(theta))
                vdata.append(math.cos(phi))
                vdata.append(math.sin(phi) * math.sin(theta))
        for y in range(this.rigns - 1):
            for x in range(this.segments - 1):
                vindex.append((y + 0) * this.segments + x)
                vindex.append((y + 1) * this.segments + x)
                vindex.append((y + 1) * this.segments + x + 1)
                vindex.append((y + 1) * this.segments + x + 1)
                vindex.append((y + 0) * this.segments + x + 1)
                vindex.append((y + 0) * this.segments + x)
        #this.vboID = glGenBuffers(1)
        #glBindBuffer(GL_ARRAY_BUFFER,this.vboID)
        #glBufferData (GL_ARRAY_BUFFER, len(vdata)*4, vdata, GL_STATIC_DRAW)
        #this.eboID = glGenBuffers(1)
        #glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this.eboID)
        #glBufferData (GL_ELEMENT_ARRAY_BUFFER, len(vIndex)*4, vIndex,
        #GL_STATIC_DRAW)
        this.vbo = vbo.VBO(ny.array(vdata,'f'))
        this.ebo = vbo.VBO(ny.array(vindex,'H'),target = GL_ELEMENT_ARRAY_BUFFER)
        this.vboLength = this.segments * this.rigns
        this.eboLength = len(vindex)
        this.bCreate = True
    def drawShader(this,vi,ni,ei):
        if this.bCreate == False:
            this.createVAO()
        #glBindBuffer(GL_ARRAY_BUFFER,this.vboID)
        #glVertexAttribPointer(vi,3,GL_FLOAT,False,24,0)
        #glEnableVertexAttribArray(vi)
        #glVertexAttribPointer(ni,3,GL_FLOAT,False,24,12)
        #glEnableVertexAttribArray(ni)
        #glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this.eboID)
        #glDrawElements(GL_TRIANGLES,this.eboLength,GL_UNSIGNED_INT,0)
        this.vbo.bind()
    def draw(this):
        if this.bCreate == False:
            this.createVAO()
        #glBindBuffer(GL_ARRAY_BUFFER,this.vboID)
        #glInterleavedArrays(GL_N3F_V3F,0,None)
        #glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this.eboID)
        #glDrawElements(GL_TRIANGLES,this.eboLength,GL_UNSIGNED_INT,None)
        this.vbo.bind()
        glInterleavedArrays(GL_N3F_V3F,0,None)
        this.ebo.bind()
        glDrawElements(GL_TRIANGLES,this.eboLength,GL_UNSIGNED_SHORT,None)  
