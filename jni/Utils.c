/*
//
//	other lowly 'Utily' functions and stuffz
//
*/

#include "Function.h"



void checkGlError(const char* op) {
	GLint error;
    for (error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}



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
			LOGI(" Error Compiling Shader: %s ", infoLog);      
			free ( infoLog );
		}
		glDeleteShader ( shader );									checkGlError(" glDeleteShader");
		return 0;
	}
	return shader;
}


//
//	Original gluLookAt 
//	http://www.graphicsgroups.com/6-opengl/fa8a173c5765cbb1.htm#.Ugwq86xR2Cg
//
//  It seems like OpenglES 2.0 doesn't support matrices
//  so I have to go dig up the old Matrix Lib
//




void		gluLookAt( GLfloat eyex,    GLfloat eyey,    GLfloat eyez,
                       GLfloat centerx, GLfloat centery, GLfloat centerz,
                       GLfloat upx,     GLfloat upy,     GLfloat upz )
{
   GLfloat m[16];
   GLfloat x[3], y[3], z[3];
   GLfloat mag;

   z[0] = eyex - centerx;
   z[1] = eyey - centery;
   z[2] = eyez - centerz;
   mag = sqrt( z[0]*z[0] + z[1]*z[1] + z[2]*z[2] );
   if (mag) {  
      z[0] /= mag;
      z[1] /= mag;
      z[2] /= mag;
   }

   y[0] = upx;
   y[1] = upy;
   y[2] = upz;

   x[0] =  y[1]*z[2] - y[2]*z[1];
   x[1] = -y[0]*z[2] + y[2]*z[0];
   x[2] =  y[0]*z[1] - y[1]*z[0];

   y[0] =  z[1]*x[2] - z[2]*x[1];
   y[1] = -z[0]*x[2] + z[2]*x[0];
   y[2] =  z[0]*x[1] - z[1]*x[0];

   mag = sqrt( x[0]*x[0] + x[1]*x[1] + x[2]*x[2] );
   if (mag) {
      x[0] /= mag;
      x[1] /= mag;
      x[2] /= mag;
   }

   mag = sqrt( y[0]*y[0] + y[1]*y[1] + y[2]*y[2] );
   if (mag) {
      y[0] /= mag;
      y[1] /= mag;
      y[2] /= mag;
   }

	#define M(row,col)  m[col*4+row]
		M(0,0) = x[0];  M(0,1) = x[1];  M(0,2) = x[2];  M(0,3) = 0.0;
		M(1,0) = y[0];  M(1,1) = y[1];  M(1,2) = y[2];  M(1,3) = 0.0;
		M(2,0) = z[0];  M(2,1) = z[1];  M(2,2) = z[2];  M(2,3) = 0.0;
		M(3,0) = 0.0;   M(3,1) = 0.0;   M(3,2) = 0.0;   M(3,3) = 1.0;
	#undef M
	
	
	glMultMatrixf( m );

	glTranslatef( -eyex, -eyey, -eyez );
	
} 

