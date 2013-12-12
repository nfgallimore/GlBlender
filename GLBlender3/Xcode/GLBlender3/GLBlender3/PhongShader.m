//
//  PhongShader.m
//  GLBlender3
//
//  Created by Nick Gallimore on 12/12/13.
//  Copyright (c) 2013 Ricardo Rendon Cepeda. All rights reserved.
//

#import "PhongShader.h"

// 1
// Shaders
#define STRINGIFY(A) #A
#include "Phong.vsh"
#include "Phong.fsh"

@implementation PhongShader

- (id)init
{
    if(self = [super init])
    {
        // 2
        // Program
        self.program = [self BuildProgram:PhongVSH with:PhongFSH];
        
        // 3
        // Attributes
        self.aPosition = glGetAttribLocation(self.program, "aPosition");
        self.aNormal = glGetAttribLocation(self.program, "aNormal");
        
        // 4
        // Uniforms
        self.uProjectionMatrix = glGetUniformLocation(self.program, "uProjectionMatrix");
        self.uModelViewMatrix = glGetUniformLocation(self.program, "uModelViewMatrix");
        self.uNormalMatrix = glGetUniformLocation(self.program, "uNormalMatrix");
        self.uDiffuse = glGetUniformLocation(self.program, "uDiffuse");
        self.uSpecular = glGetUniformLocation(self.program, "uSpecular");
    }
    
    return self;
}

@end