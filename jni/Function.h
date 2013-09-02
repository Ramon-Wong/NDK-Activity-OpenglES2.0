#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <jni.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,  "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "native-activity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))





struct engine {
	struct android_app* 	app;
	
	struct{
		EGLDisplay 			display;
		EGLSurface 			surface;
		EGLContext 			context;
	}EGL;
	
	
	struct{
		GLuint				pObject;
		GLint				gPositionAttribute;
		GLint				pMatrixUniform;
		GLint				mvMatrixUniform;
	}GLData;
	
	struct{
		int32_t 			width;
		int32_t 			height;
	}Scr;

	int32_t 				touchX;				// there are going to be an array of this	
	int32_t 				touchY;
};



GLuint						displayInit(struct engine *);
void						displayEnd(struct engine *);

void						printGLString(const char *, GLenum);
GLuint 						LoadShader( GLenum, const char *);
GLuint						CreateGLProgram(const char *, const char *);
GLuint						SetupGraphics(struct engine *);

void						handle_Cmd(struct android_app *, int32_t);
int32_t 					handle_Input(struct android_app *, AInputEvent *);
void						Render(struct engine* ); 
void						checkGlError(const char* );




#endif
