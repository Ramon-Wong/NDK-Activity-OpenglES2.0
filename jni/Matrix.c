#include "Matrix.h"



void LoadIdentity(GLfloat * Mat){
	GLfloat M[16] = { 1.0, 0.0, 0.0, 0.0, 
					  0.0, 1.0, 0.0, 0.0,
					  0.0, 0.0, 1.0, 0.0,
					  0.0, 0.0, 0.0, 1.0 };
	memcpy( Mat, M, sizeof(GLfloat[16]));
}


void Perspective(GLfloat * Mat, GLfloat fov, GLfloat aspect, GLfloat znear, GLfloat zfar){
	GLfloat	F	= (double)(1.0 / tan(fov * 0.5));
	GLfloat NF	= (double)(1.0 / (znear - zfar));

	Mat[0]  = F / aspect;	
	Mat[1]  = 0.0;	
	Mat[2]  = 0.0;	
	Mat[3]  = 0.0;	
	Mat[4]  = 0.0;	
	Mat[5]  = F;
	Mat[6]  = 0.0;
	Mat[7]  = 0.0;	
	Mat[8]  = 0.0;	
	Mat[9]  = 0.0;
	Mat[10] = (zfar + znear) * NF;
	Mat[11] = -1.0;					
	Mat[12] = 0.0;
	Mat[13] = 0.0;	
	Mat[14] = (2 * zfar * znear) * NF;	
	Mat[15] = 0.0;		
}


void Translate( GLfloat X, GLfloat Y, GLfloat Z, GLfloat * Mat){
	
}

