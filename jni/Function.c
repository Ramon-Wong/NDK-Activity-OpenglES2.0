#include "Function.h"


	
void Render(struct engine * engine) {

	if (engine->display == NULL) {	
		return;						// No display.
	}
	
	GLfloat	Vertices[] = {   0.0f,  0.5f,  0.0f,
							-0.5f, -0.5f,  0.0f,
							 0.5f, -0.5f,  0.0f};	

	glViewport( 0, 0, engine->width, engine->height);
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);									checkGlError("glClearColor");
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);					checkGlError("glClear");
	glUseProgram(engine->pObject);											checkGlError("glUseProgram");
	    
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, Vertices);			checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(0);											checkGlError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLES, 0, 3);
	    
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
	
	// Initialize GL state.
	return 1;
}



GLuint	CreateGLProgram(const char * vShader, const char * fShader){
	GLuint vertexShader	= LoadShader(GL_VERTEX_SHADER,	 vShader);
	GLuint pixelShader	= LoadShader(GL_FRAGMENT_SHADER, fShader);

	GLuint program		= glCreateProgram();
	if(!program){
		LOGI(" Problems with ProgramObject");
		return 0;
	}
	glAttachShader(program, vertexShader );				checkGlError(" glAttachShader: VertexShader");
	glAttachShader(program, pixelShader );				checkGlError(" glAttachShader: FragmentShader");
	
	glLinkProgram(program);								checkGlError(" glLinkProgram");

	// Check the link status
	GLint	linked = GL_FALSE;
	glGetProgramiv( program, GL_LINK_STATUS, &linked );	
	
        if (linked != GL_TRUE) {
		GLint bufLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
		if(bufLength) {
			char* buf = (char*) malloc(bufLength);
			if (buf) {
				glGetProgramInfoLog(program, bufLength, NULL, buf);
				LOGE("Could not link program:\n%s\n", buf);
				free(buf);
			}
		}
		
		glDeleteProgram(program);
		program = 0;
        }	
	
	return program;
}


GLuint SetupGraphics(struct engine * engine){
	
	const char vShader[] = 
	"attribute vec4 vPosition;\n"
	"void main() {\n"
	"  gl_Position = vPosition;\n"
	"}\n";

	const char fShader[] = 
	"precision mediump float;\n"
	"void main() {\n"
	"  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
	"}\n";	
		
	LOGI("Init Graphics");
	
	printGLString("Version",	GL_VERSION);
	printGLString("Vendor",		GL_VENDOR);
	printGLString("Renderer",	GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);
	
	LOGI("setupGraphics(%d, %d)", engine->width, engine->height);

	engine->pObject = CreateGLProgram(vShader, fShader);
	
	if(!engine->pObject) {
		LOGE("Could not create program.");
		return 0;
	}
		
	engine->gvPositionHandle = glGetAttribLocation( engine->pObject, "vPosition");			checkGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"vPosition\") = %d\n", engine->gvPositionHandle);

	glViewport(0, 0, engine->width, engine->height);										checkGlError("glViewport");		
		
	return 1;
}
