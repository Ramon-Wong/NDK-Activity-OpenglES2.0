#include "Matrix.h"





void LoadIdentity(GLfloat * Mat){
	GLfloat M[16] = { 1.0, 0.0, 0.0, 0.0, 
					  0.0, 1.0, 0.0, 0.0,
					  0.0, 0.0, 1.0, 0.0,
					  0.0, 0.0, 0.0, 1.0 };
	memcpy( Mat, M, sizeof(GLfloat[16]));
}


void MMultiply(GLfloat * Mat, GLfloat * Mat1, GLfloat * Mat2){
	
	GLuint i;
	GLfloat Result[16];
	
	for(i = 0; i < 4; i++){
		Result[i * 4]	  = (Mat2[i*4] * Mat1[0]) + (Mat2[i*4+1] * Mat1[4]) + (Mat2[i*4+2] * Mat1[8])  + (Mat2[i*4+3] * Mat1[12]);
		Result[i * 4 + 1] = (Mat2[i*4] * Mat1[1]) + (Mat2[i*4+1] * Mat1[5]) + (Mat2[i*4+2] * Mat1[9])  + (Mat2[i*4+3] * Mat1[13]);
		Result[i * 4 + 2] = (Mat2[i*4] * Mat1[2]) + (Mat2[i*4+1] * Mat1[6]) + (Mat2[i*4+2] * Mat1[10]) + (Mat2[i*4+3] * Mat1[14]);
		Result[i * 4 + 3] = (Mat2[i*4] * Mat1[3]) + (Mat2[i*4+1] * Mat1[7]) + (Mat2[i*4+2] * Mat1[11]) + (Mat2[i*4+3] * Mat1[15]);
	}
	memcpy(Mat, Result, sizeof(GLfloat[16]));
}


void MScale(GLfloat * Result, GLfloat x, GLfloat y, GLfloat z){
	Result[0] *= x; Result[1] *= x; Result[2]  *= x; Result[3]  *= x;
	Result[4] *= y; Result[5] *= y; Result[6]  *= y; Result[7]  *= y;
	Result[8] *= z; Result[9] *= z; Result[10] *= z; Result[11] *= z;
}


void MTranslate(GLfloat * Result, GLfloat x, GLfloat y, GLfloat z){
    Result[12] += (Result[0] * x + Result[4] * y + Result[8]  * z);
    Result[13] += (Result[1] * x + Result[5] * y + Result[9]  * z);
    Result[14] += (Result[2] * x + Result[6] * y + Result[10] * z);
    Result[15] += (Result[3] * x + Result[7] * y + Result[11] * z);
}


void MRotate(GLfloat * Mat, GLfloat angle, GLfloat x, GLfloat y, GLfloat z){
	GLfloat sin_angle, cos_angle;
    GLfloat mag = sqrtf(x * x + y * y + z * z);

    sin_angle = sinf ( angle * PI / 180.0 );
    cos_angle = cosf ( angle * PI / 180.0 );
    
    if(mag > 0.0){
		GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
		GLfloat one_minus_cos;
     
		x /= mag;
		y /= mag;
		z /= mag;
    
        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sin_angle;
        ys = y * sin_angle;
        zs = z * sin_angle;
        one_minus_cos = 1.0 - cos_angle;    

        Mat[0] = (one_minus_cos * xx) + cos_angle;
        Mat[1] = (one_minus_cos * xy) - zs;
        Mat[2] = (one_minus_cos * zx) + ys;
        Mat[3] = 0.0;

        Mat[4] = (one_minus_cos * xy) + zs;
        Mat[5] = (one_minus_cos * yy) + cos_angle;
        Mat[6] = (one_minus_cos * yz) - xs;
        Mat[7] = 0.0;

        Mat[8] = (one_minus_cos * zx) - ys;
        Mat[9] = (one_minus_cos * yz) + xs;
        Mat[10] = (one_minus_cos * zz) + cos_angle;
        Mat[11] = 0.0;

        Mat[12] = 0.0;
        Mat[13] = 0.0;
        Mat[14] = 0.0;
        Mat[15] = 1.0;
	}
}


void MFrustum(GLfloat * result, GLfloat Left, GLfloat Right, 
			  GLfloat Bottom, GLfloat Top, GLfloat zNear, GLfloat zFar){

	GLfloat rWidth	= Right - Left;
	GLfloat rHeight = Top - Bottom;
	GLfloat rDepth	= zFar - zNear;
	GLfloat tMat[16];

	memset((void*)tMat, 0, 16*sizeof(GLfloat));
	
	tMat[ 0] = 2.0f * zNear / rWidth;   
	tMat[ 5] = 2.0f * zNear / rHeight;
	tMat[ 8] = (Right + Left) / rWidth;
	tMat[ 9] = (Top + Bottom) / rHeight;
	tMat[10] =-(zFar + zNear) / rDepth;
	tMat[11] = -1.0f;	
	tMat[14] = -(zFar * zNear * 2) / rDepth;
	tMat[15] = 0.0f;
}


void MPerspective(GLfloat * Mat, GLfloat FOVY, GLfloat Aspect, GLfloat zNear, GLfloat zFar){
	
	GLfloat fHeight = tanf(FOVY / 360.0 * PI) * zNear;
	GLfloat fWidth	= fHeight * Aspect;
    
	MFrustum( Mat, -fWidth, fWidth, -fHeight, fHeight, zNear, zFar);	
}


void MOrtho( GLfloat * Mat, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far){
    GLfloat LR = 1 / (left - right);
    GLfloat BT = 1 / (bottom - top);
    GLfloat NF = 1 / (near - far);

    memset((void*)Mat, 0.0, 16*sizeof(GLfloat));

    Mat[0]  = -2 * LR;
    Mat[5]  = -2 * BT;
    Mat[10] =  2 * NF;
    Mat[12] = (left + right) * LR;
    Mat[13] = (top + bottom) * BT;
    Mat[14] = (far + near) * NF;
    Mat[15] = 1.0f;
};


void LookAtM( GLfloat * Mat, GLfloat * Pose, GLfloat * View, GLfloat * UpVx){
	
	GLfloat X[3], Y[3], Z[3];
						
	Z[0] = View[0] - Pose[0];
    Z[1] = View[1] - Pose[1];
    Z[2] = View[2] - Pose[2];
	
	Normalize(Z);
		
	CrossProduct( X, UpVx, Z);	//   Compute cross product of UpVx, Z
	Normalize(X);
	
	CrossProduct( Y, Z, X);		// reCompute vector Y
	Normalize(Y);

	Mat[0]  = X[0];
	Mat[4]  = X[1];
	Mat[8]  = X[2];
	Mat[12] = 0.0;
	
	Mat[1]  = Y[0];
	Mat[5]  = Y[1];
	Mat[9]  = Y[2];
	Mat[13] = 0.0;
	
	Mat[2]  = Z[0];
	Mat[6]  = Z[1];				
	Mat[10] = Z[2];
	Mat[14] = 0.0;
	
	Mat[3]  = 0.0;
	Mat[7]  = 0.0;
	Mat[11] = 0.0;
	Mat[15] = 1.0;
	
	MTranslate( Mat, -View[0], -View[1], -View[2]);
}


void CrossProduct(GLfloat * Result, GLfloat * Vec1, GLfloat * Vec2){
	
	Result[0] = Vec1[1] * Vec2[2] - Vec1[2] * Vec2[1];
	Result[1] = Vec1[2] * Vec2[0] - Vec1[0] * Vec2[2];
	Result[2] = Vec1[0] * Vec2[1] - Vec1[1] * Vec2[0];
}


void Normalize(GLfloat * Vec){
	
	GLfloat Magnitude =  sqrt(Vec[0] * Vec[0] + Vec[1] * Vec[1] + Vec[2] * Vec[2]);
	Vec[0] /= Magnitude;
	Vec[1] /= Magnitude;
	Vec[2] /= Magnitude;
}
