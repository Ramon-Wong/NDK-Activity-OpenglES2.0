#include "Matrix.h"





void LoadIdentity(GLfloat * Mat){
	GLfloat M[16] = { 1.0, 0.0, 0.0, 0.0, 
					  0.0, 1.0, 0.0, 0.0,
					  0.0, 0.0, 1.0, 0.0,
					  0.0, 0.0, 0.0, 1.0 };
	memcpy( Mat, M, sizeof(GLfloat[16]));
}





void Translate( GLfloat X, GLfloat Y, GLfloat Z, GLfloat * Mat){
	Mat[12] = X;
	Mat[13] = Y; 
	Mat[14] = Z;
}


GLfloat * MMultiply(GLfloat * Mat1, GLfloat * Mat2, GLfloat * Result){
	
	GLuint i;
	
	for(i = 0; i < 4; i++){
		Result[i * 4]	  = (Mat2[i*4] * Mat1[0]) + (Mat2[i*4+1] * Mat1[4]) + (Mat2[i*4+2] * Mat1[8])  + (Mat2[i*4]+3 * Mat1[12]);
		Result[i * 4 + 1] = (Mat2[i*4] * Mat1[1]) + (Mat2[i*4+1] * Mat1[5]) + (Mat2[i*4+2] * Mat1[9])  + (Mat2[i*4]+3 * Mat1[13]);
		Result[i * 4 + 2] = (Mat2[i*4] * Mat1[2]) + (Mat2[i*4+1] * Mat1[6]) + (Mat2[i*4+2] * Mat1[10]) + (Mat2[i*4]+3 * Mat1[14]);
		Result[i * 4 + 3] = (Mat2[i*4] * Mat1[3]) + (Mat2[i*4+1] * Mat1[7]) + (Mat2[i*4+2] * Mat1[11]) + (Mat2[i*4]+3 * Mat1[15]);
	}
	
	return Result;
}


void MTranslate(GLfloat * Mat1, GLfloat x, GLfloat y, GLfloat z){
	int i;
	
	for(i = 0; i < 4; i++){
		Mat1[12 + i] += ( Mat1[i]*x + Mat1[4+i]*y + Mat1[8+i] *z);
    }
}


void MFrustum(GLfloat * tMatrix, GLfloat Left, GLfloat Right, GLfloat Bottom, GLfloat Top, GLfloat zNear, GLfloat zFar){
	
	GLfloat rWidth	= 1.0f / (Right - Left);
	GLfloat rHeight = 1.0f / (Top - Bottom);
	GLfloat rDepth	= 1.0f / (zNear - zFar);

	
	memset((void*)tMatrix, 0, 16*sizeof(float));
	
	tMatrix[ 0] = 2.0f * (zNear * rWidth);
	tMatrix[ 5] = 2.0f * (zNear * rHeight);
	tMatrix[ 8] = 2.0f * ((Right + Left) * rWidth);
	tMatrix[ 9] = (Top + Bottom) * rHeight;
	tMatrix[10] = (zFar + zNear) * rDepth;
	tMatrix[14] = 2.0f * (zFar * zNear * rDepth);
	tMatrix[11] = -1.0f;	
}



void MPerspective(GLfloat * Mat, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar){
	GLfloat fWidth, fHeight;

	fWidth	= tan(fovy * PI / 360.0) * zNear;
	fHeight	= fWidth * aspect;
	
	MFrustum( Mat, -fWidth, fWidth, -fHeight, fHeight, zNear, zFar);
}






