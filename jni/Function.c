#include "Function.h"



GLfloat Pyramid_Vertices[] = { 0.0,  1.0,  0.0,	// Front
							  -1.0, -1.0,  1.0,
							   1.0, -1.0,  1.0,

							   0.0,  1.0,  0.0,	// Right
							   1.0, -1.0,  1.0,
							   1.0, -1.0, -1.0,

							   0.0,  1.0,  0.0,	// Back
							   1.0, -1.0, -1.0,
							  -1.0, -1.0, -1.0,
							
							   0.0,  1.0,  0.0,	// Left
							  -1.0, -1.0, -1.0,
							  -1.0, -1.0,  1.0};


GLfloat Pyramid_Colors[] = { 1.0, 0.0, 0.0, 1.0,	// Front
						     0.0, 1.0, 0.0, 1.0,
						     0.0, 0.0, 1.0, 1.0,
						   
						     1.0, 0.0, 0.0, 1.0,	// Right
							 0.0, 0.0, 1.0, 1.0,
							 0.0, 1.0, 0.0, 1.0,
						   
							 1.0, 0.0, 0.0, 1.0,	// Back
							 0.0, 1.0, 0.0, 1.0,
							 0.0, 0.0, 1.0, 1.0,
						   
							 1.0, 0.0, 0.0, 1.0,	// Left
							 0.0, 0.0, 1.0, 1.0,
							 0.0, 1.0, 0.0, 1.0};
						 
GLfloat Cube_Vertices[] = {	-1.0, -1.0,  1.0,	// Front
							 1.0, -1.0,  1.0,
							 1.0,  1.0,  1.0,
							-1.0,  1.0,  1.0,

							-1.0, -1.0, -1.0,	// Back
							-1.0,  1.0, -1.0,
							 1.0,  1.0, -1.0,
							 1.0, -1.0, -1.0,

							-1.0,  1.0, -1.0,	// Top
							-1.0,  1.0,  1.0,
							 1.0,  1.0,  1.0,
							 1.0,  1.0, -1.0,

							-1.0, -1.0, -1.0,	// Bottom
							 1.0, -1.0, -1.0,
							 1.0, -1.0,  1.0,
							-1.0, -1.0,  1.0,

							 1.0, -1.0, -1.0,	// Right
							 1.0,  1.0, -1.0,
							 1.0,  1.0,  1.0,
							 1.0, -1.0,  1.0,
							 
							-1.0, -1.0, -1.0,
							-1.0, -1.0,  1.0,
							-1.0,  1.0,  1.0,
							-1.0,  1.0, -1.0};

GLfloat Cube_Colors[6][4] ={{ 1.0, 0.0, 0.0, 1.0}, // Front
							{ 1.0, 1.0, 0.0, 1.0}, // Back
							{ 0.0, 1.0, 0.0, 1.0}, // Top
							{ 1.0, 0.5, 0.5, 1.0}, // Bottom
							{ 1.0, 0.0, 1.0, 1.0}, // Right
							{ 0.0, 0.0, 1.0, 1.0}};// Left

GLubyte Cube_Indices[] = {  0, 1, 2,		0, 2, 3,		// Front
							4, 5, 6,		4, 6, 7,		// Back face
							8, 9, 10,		8, 10, 11,		// Top face
							12, 13, 14,		12, 14, 15,		// Bottom face
							16, 17, 18,		16, 18, 19, 	// Right face
							20, 21, 22,		20, 22, 23};	// Left face
						 
						 
static GLfloat	pRot_Z	= 0.0f;	
GLfloat		FrameInterval	= 0.0f;						 
						 
void	CalculateFPS(){

}						 
						 
						 
						 
						 
void setMatrixUniforms(struct engine * engine) {
	glUniformMatrix4fv(engine->GLData.u_Matrix, 1, false, engine->Matrices.mvpMat);			checkGlError("glUniformMatrix4fv Model Stacks");	
	glUniformMatrix4fv(engine->GLData.p_Matrix,  1, false, engine->Matrices.pMatrix);		checkGlError("glUniformMatrix4fv Perspective");
}

	
void Render(struct engine * engine) {

	if (engine->EGL.display == NULL) {	
		return;						// No display.
	}
	
	GLfloat	Vertices[] = {   0.0f,  0.5f,  0.0f,
							-0.5f, -0.5f,  0.0f,
							 0.5f, -0.5f,  0.0f};	
							 
	const GLfloat Colors[3][3] = {	{ 1, 0, 0 },
									{ 0, 1, 0 },
									{ 0, 0, 1 }};
											
	GLfloat	mMatrix[16];
	
	pRot_Z += 5.1;
	
//	glViewport(0, 0, engine->Scr.width, engine->Scr.height);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);												//checkGlError("glClear");
	glUseProgram(engine->GLData.pObject);																//checkGlError("glUseProgram");

	
	GLfloat rMatrix[16];
	
	LoadIdentity( mMatrix);
	LoadIdentity( rMatrix);

	MTranslate( mMatrix, -1.75, 0.0, -3.0);
	MRotate( rMatrix, DegToRad(pRot_Z), 0, 1, 0);
	MMultiply( mMatrix, mMatrix, rMatrix);

	glVertexAttribPointer( engine->GLData.gPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, Pyramid_Vertices);
	glVertexAttribPointer( engine->GLData.gColorsAttribute, 3, GL_FLOAT, GL_FALSE, 0, Pyramid_Colors);
	
	glEnableVertexAttribArray( engine->GLData.gPositionAttribute);										//checkGlError("glEnableVertexAttribArray");
	glEnableVertexAttribArray( engine->GLData.gColorsAttribute);										//checkGlError("glEnableVertexAttribArray");	

	MMultiply( engine->Matrices.mvpMat, engine->Matrices.cMatrix, mMatrix);								// Camera + Model => mvpMat
	setMatrixUniforms(engine);																			// Push it!	
	glDrawArrays(GL_TRIANGLES, 0, 12);	// 12 because: 4 sides, each side has 3 vectors..

	glDisableVertexAttribArray( engine->GLData.gPositionAttribute);										//checkGlError("glDisableVertexAttribArray Vertex");    
	glDisableVertexAttribArray( engine->GLData.gColorsAttribute);										//checkGlError("glDisableVertexAttribArray Colors");	
		
	LoadIdentity( mMatrix);
	LoadIdentity( rMatrix);

	MTranslate( mMatrix, 1.75, 0.0, -3.0);
	MRotate( rMatrix, DegToRad(pRot_Z), 1, 1, 1);
	MMultiply( mMatrix, mMatrix, rMatrix);

	glVertexAttribPointer( engine->GLData.gPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, Cube_Vertices);
	glVertexAttribPointer( engine->GLData.gColorsAttribute, 3, GL_FLOAT, GL_FALSE, 0, Cube_Colors);

	glEnableVertexAttribArray( engine->GLData.gPositionAttribute);										//checkGlError("glEnableVertexAttribArray");
	glEnableVertexAttribArray( engine->GLData.gColorsAttribute);										//checkGlError("glEnableVertexAttribArray");	

	MMultiply( engine->Matrices.mvpMat, engine->Matrices.cMatrix, mMatrix);
	setMatrixUniforms(engine);		
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, Cube_Indices);
	
	glDisableVertexAttribArray( engine->GLData.gPositionAttribute);										//checkGlError("glDisableVertexAttribArray Vertex");    
	glDisableVertexAttribArray( engine->GLData.gColorsAttribute);										//checkGlError("glDisableVertexAttribArray Colors");	
	
	eglSwapBuffers(engine->EGL.display, engine->EGL.surface);
}	
	


