#ifndef MATRIX_H
#define MATRIX_H

#include "Function.h"


#define PI 3.14159265359





void						LoadIdentity(GLfloat *);
void						Translate(GLfloat, GLfloat, GLfloat, GLfloat *);

GLfloat *					MMultiply(GLfloat *, GLfloat *, GLfloat *);
void						MTranslate(GLfloat *, GLfloat, GLfloat, GLfloat);

#endif

