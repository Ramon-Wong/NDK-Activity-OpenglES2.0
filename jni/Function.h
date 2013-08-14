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


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))









struct engine {
	struct android_app* app;

	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	
	GLuint	programObject;
	
	int32_t width;
	int32_t height;

	int32_t touchX;
	int32_t touchY;
};



int			init_display(struct engine *);
void		terminate_display(struct engine *);
void		handle_cmd(struct android_app *, int32_t);
int32_t 	handle_input(struct android_app *, AInputEvent *);
void		draw_frame(struct engine* ); 
void		checkGlError(const char* );




#endif
