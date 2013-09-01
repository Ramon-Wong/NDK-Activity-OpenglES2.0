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
