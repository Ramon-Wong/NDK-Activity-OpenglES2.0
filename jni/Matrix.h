#ifndef MATRIX_H
#define MATRIX_H

#include "Function.h"


#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286




void						LoadIdentity(GLfloat *);

void						Normalize(GLfloat *);
void						CrossProduct(GLfloat *, GLfloat *, GLfloat *);

void						MMultiply(GLfloat *, GLfloat *, GLfloat *);

void 						MScale(GLfloat *, GLfloat, GLfloat, GLfloat);
void						MTranslate(GLfloat *, GLfloat, GLfloat, GLfloat);
void						MRotate(GLfloat *, GLfloat, GLfloat, GLfloat, GLfloat);

void						MFrustum(GLfloat *, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void						MPerspective(GLfloat *, GLfloat, GLfloat, GLfloat, GLfloat);
void						MOrtho( GLfloat *, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

void						LookAtM( GLfloat *, GLfloat *, GLfloat *, GLfloat *);


#endif
