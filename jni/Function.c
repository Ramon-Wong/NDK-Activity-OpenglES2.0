#include "Function.h"



void setMatrixUniforms(struct engine * engine) {
	glUniformMatrix4fv(engine->GLData.u_Matrix, 1, false, engine->Matrices.mvpMat);			checkGlError("glUniformMatrix4fv Stacks");	
	glUniformMatrix4fv(engine->GLData.p_Matrix,  1, false, engine->Matrices.pMatrix);		checkGlError("glUniformMatrix4fv Perspective");
}

	
void Render(struct engine * engine) {

	if (engine->EGL.display == NULL) {	
		return;						// No display.
	}
		
	GLfloat	Vertices[] = {   0.0f,  0.5f,  0.0f,
							-0.5f, -0.5f,  0.0f,
							 0.5f, -0.5f,  0.0f};	
							 
	glViewport( 0, 0, engine->Scr.width, engine->Scr.height);
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);																//checkGlError("glClearColor");
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);												//checkGlError("glClear");
	glUseProgram(engine->GLData.pObject);																//checkGlError("glUseProgram");
	    
	LoadIdentity( engine->Matrices.mMatrix);
	// rotate, translate scale or whatever shit. everything into Model	
	
	glVertexAttribPointer( engine->GLData.gPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, Vertices);		//checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray( engine->GLData.gPositionAttribute);										//checkGlError("glEnableVertexAttribArray");
	
	MMultiply( engine->Matrices.mvpMat, engine->Matrices.cMatrix, engine->Matrices.mMatrix);			// Camera + Model => mvpMat
//	MMultiply( engine->Matrices.mvpMat, engine->Matrices.pMatrix, engine->Matrices.mvpMat);				// projection => mvpMat
	
	setMatrixUniforms(engine);																			// Push it!	
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	    
	eglSwapBuffers(engine->EGL.display, engine->EGL.surface);
}	
	


