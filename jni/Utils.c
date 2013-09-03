#include "Function.h"



void checkGlError(const char* op) {
	GLint error;
    for (error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
    // http://www.opengl.org/wiki/OpenGL_Error
	//	GL_INVALID_ENUM​						0x0500
	//	GL_INVALID_VALUE​					0x0501
	//	GL_INVALID_OPERATION​				0x0502
	//	GL_STACK_OVERFLOW​					0x0503
	//	GL_STACK_UNDERFLOW​					0x0504
	//	GL_OUT_OF_MEMORY​					0x0505	
	//	GL_INVALID_FRAMEBUFFER_OPERATION​	0x0506
	//	GL_TABLE_TOO_LARGE​1					0x8031    
}


void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}



GLuint LoadShader ( GLenum type, const char *shaderSrc ){
	GLuint shader;
	GLint compiled;
   
	// Create the shader object
	shader = glCreateShader(type );												checkGlError(" glCreateShader");
	if( shader == 0)return 0;

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL );								checkGlError(" glShaderSource");
   
	// Compile the shader
	glCompileShader(shader );													checkGlError(" glCompileShader");

	// Check the compile status
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );						checkGlError(" glGetShaderiv");

	if(!compiled ){
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );					checkGlError(" glGetShaderiv");
		if ( infoLen > 1 ){
			char* infoLog = malloc (sizeof(char) * infoLen );

			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );				checkGlError(" glGetShaderInfoLog");
			LOGI(" Error Compiling Shader: %s ", infoLog);      
			free ( infoLog );
		}
		glDeleteShader ( shader );												checkGlError(" glDeleteShader");
		return 0;
	}

	return shader;
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

	engine->EGL.display		= display;
	engine->EGL.context		= context;
	engine->EGL.surface		= surface;
	engine->Scr.width		= w;
	engine->Scr.height		= h;
	
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
	"attribute vec4 vPosition;			\n"
	"									\n"
	"void main() {\n"
	"  gl_Position = vPosition;	\n"
	"}\n";

	const char vShader2[] =
	"attribute vec3 vPosition;												\n"

    "uniform mat4 uMVMatrix;												\n"
    "uniform mat4 uPMatrix;													\n"

    "void main(void) {														\n"
    "    gl_Position = uPMatrix * uMVMatrix * vec4(vPosition, 1.0);			\n"
    "} 																		\n";


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
	
	LOGI("setupGraphics(%d, %d)", engine->Scr.width, engine->Scr.height);

	engine->GLData.pObject = CreateGLProgram(vShader2, fShader);
	
	if(!engine->GLData.pObject) {
		LOGE("Could not create program.");
		return 0;
	}
		
	engine->GLData.gPositionAttribute	= glGetAttribLocation( engine->GLData.pObject, "vPosition");	checkGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"vPosition\") = %d\n", engine->GLData.gPositionAttribute);
	glEnableVertexAttribArray(engine->GLData.gPositionAttribute);										checkGlError("glenableVertexAttribArray");

	LOGI("Get Uniform Locations");
    engine->GLData.pMatrixUniform	= glGetUniformLocation(engine->GLData.pObject, "uPMatrix");			checkGlError("glgetUniformLocation uPMatrix");
    engine->GLData.mvMatrixUniform	= glGetUniformLocation(engine->GLData.pObject, "uMVMatrix");		checkGlError("glgetUniformLocation uMVMatrix");


	glViewport(0, 0, engine->Scr.width, engine->Scr.height);											checkGlError("glViewport");		
		
	return 1;
}
