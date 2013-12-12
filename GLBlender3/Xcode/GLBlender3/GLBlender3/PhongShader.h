//
//  PhongShader.h
//  GLBlender3
//
//  Created by Nick Gallimore on 12/12/13.
//  Copyright (c) 2013 Ricardo Rendon Cepeda. All rights reserved.
//

#import "Shader.h"

@interface PhongShader : Shader

// Program Handle
@property (readwrite) GLint program;

// Attribute Handles
@property (readwrite) GLint aPosition;
@property (readwrite) GLint aNormal;

// Uniform Handles
@property (readwrite) GLint uProjectionMatrix;
@property (readwrite) GLint uModelViewMatrix;
@property (readwrite) GLint uNormalMatrix;
@property (readwrite) GLint uDiffuse;
@property (readwrite) GLint uSpecular;

@end