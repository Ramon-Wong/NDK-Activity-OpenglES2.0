#include "Function.h"



void setMatrixUniforms(struct engine * engine) {
	glUniformMatrix4fv(engine->GLData.pMatrixUniform,  1, false, engine->Matrices.Perspective);		checkGlError("glUniformMatrix4fv Perspective");
	glUniformMatrix4fv(engine->GLData.mvMatrixUniform, 1, false, engine->Matrices.Stacks[7]);		checkGlError("glUniformMatrix4fv Stacks");
}

	
void Render(struct engine * engine) {

	if (engine->EGL.display == NULL) {	
		return;						// No display.
	}
	
	Perspective( engine->Matrices.Perspective, 45.0, engine->Scr.width / engine->Scr.height, 0.1, 1000.0);
	
	GLfloat	Vertices[] = {   0.0f,  0.5f,  0.0f,
							-0.5f, -0.5f,  0.0f,
							 0.5f, -0.5f,  0.0f};	

	glViewport( 0, 0, engine->Scr.width, engine->Scr.height);
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);																//checkGlError("glClearColor");
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);												//checkGlError("glClear");
	glUseProgram(engine->GLData.pObject);																//checkGlError("glUseProgram");
	    
	glVertexAttribPointer( engine->GLData.gPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, Vertices);		//checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray( engine->GLData.gPositionAttribute);										//checkGlError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLES, 0, 3);
	    
	eglSwapBuffers(engine->EGL.display, engine->EGL.surface);
}	
	


