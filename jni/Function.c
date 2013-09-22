#include "Function.h"



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
	LoadIdentity( mMatrix);

	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);												//checkGlError("glClear");
	glUseProgram(engine->GLData.pObject);																//checkGlError("glUseProgram");
	    
	LoadIdentity( mMatrix);
	LoadIdentity( engine->Matrices.mvpMat);
	
	MTranslate( mMatrix, 0.0, 0.0, 2.0);
	// rotate, translate scale or whatever shit. everything into Model	
	
	glVertexAttribPointer( engine->GLData.gPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, Vertices);		//checkGlError("glVertexAttribPointer Vertexs");
	glVertexAttribPointer( engine->GLData.gColorsAttribute, 3, GL_FLOAT, GL_FALSE, 0, Colors);			//checkGlError("glVertexAttribPointer Colors");
	
	glEnableVertexAttribArray( engine->GLData.gPositionAttribute);										//checkGlError("glEnableVertexAttribArray");
	glEnableVertexAttribArray( engine->GLData.gColorsAttribute);										//checkGlError("glEnableVertexAttribArray");	
	
	MMultiply( engine->Matrices.mvpMat, engine->Matrices.cMatrix, mMatrix);								// Camera + Model => mvpMat
	
	setMatrixUniforms(engine);																			// Push it!	
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableVertexAttribArray( engine->GLData.gPositionAttribute);										//checkGlError("glDisableVertexAttribArray Vertex");    
	glDisableVertexAttribArray( engine->GLData.gColorsAttribute);										//checkGlError("glDisableVertexAttribArray Colors");	
	eglSwapBuffers(engine->EGL.display, engine->EGL.surface);
}	
	


