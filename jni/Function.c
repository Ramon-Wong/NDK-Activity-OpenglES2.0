#include "Function.h"



void checkGlError(const char* op) {
	GLint error;
    for (error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc ){
	GLuint shader;
	GLint compiled;
   
	// Create the shader object
	shader = glCreateShader(type );									checkGlError(" glCreateShader");
	if( shader == 0)return 0;

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL );					checkGlError(" glShaderSource");
   
	// Compile the shader
	glCompileShader(shader );										checkGlError(" glCompileShader");

	// Check the compile status
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );			checkGlError(" glGetShaderiv");

	if(!compiled ){
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );		checkGlError(" glGetShaderiv");
		if ( infoLen > 1 ){
			char* infoLog = malloc (sizeof(char) * infoLen );

			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );	checkGlError(" glGetShaderInfoLog");
			//esLogMessage ( "Error compiling shader:\n%s\n", infoLog );                
			free ( infoLog );
		}
		glDeleteShader ( shader );									checkGlError(" glDeleteShader");
		return 0;
	}
	return shader;
}





















int init_display(struct engine* engine){
	
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

	/* Here, the application chooses the configuration it desires. In this
	* sample, we have a very simplified selection process, where we pick
	* the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, NULL, attribList);

	if(eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	// Grab the width and height of the surface
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

	return 0;
}

