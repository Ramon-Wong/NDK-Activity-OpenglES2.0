#include "Function.h"



void Render(struct engine* engine) {
	
	if (engine->display == NULL) {	
		return;						// No display.
	}

	GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
							-0.5f, -0.5f, 0.0f,
							 0.5f, -0.5f, 0.0f };
     
	// Set the viewport
	glViewport( 0, 0, engine->width, engine->height );					checkGlError(" glViewport");
   
	// Clear the color buffer
	glClear( GL_COLOR_BUFFER_BIT );										checkGlError(" glClear");

	//glLoadIdentity();
	
	// Cam Position
	gluLookAt( 0.0f, 0.5f, 6.0f,   0.0f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0);


	// Use the program object
	glUseProgram ( engine->programObject );								checkGlError(" glUseProgram");

	// Load the vertex data
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );	checkGlError(" glVertexAttribPointer");
	glEnableVertexAttribArray( 0 );										checkGlError(" glEnableVertexAttribArray");
	glDrawArrays( GL_TRIANGLES, 0, 3 );									checkGlError(" glDrawArrays");
	eglSwapBuffers(engine->display, engine->surface);
}





GLuint displayInit(struct engine* engine){
	
	const EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //important
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};

	EGLint attribList[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	eglChooseConfig(display, attribs, &config, 1, &numConfigs);
	
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, NULL, attribList);

	if(eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return 0;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display		= display;
	engine->context		= context;
	engine->surface		= surface;
	engine->width		= w;
	engine->height		= h;

	LOGI("EGL Info");
	LOGI("Width:  %i", w);
	LOGI("Height: %i", h);
	
	// Initialize GL state.
	return 1;
}

