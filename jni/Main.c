#include "Function.h"







GLbyte vShaderStr[] =  
	"attribute vec4 vPosition;						\n"
	"void main() {									\n"
	"   gl_Position = vPosition;					\n"
	"}                            					\n";
   
   
GLbyte fShaderStr[] =  
	"precision mediump float;\n"\
	"void main() {                                	\n"
	"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );	\n"
	"}  										  	\n";	


GLuint		vertexShader;
GLuint		fragmentShader;
GLuint		programObject;
GLuint		linked;



void handle_Cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
		case APP_CMD_SAVE_STATE:
		LOGI(" App Status: APP_CMD_SAVE_STATE");
		break;
		
		case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		LOGI(" App Status: APP_CMD_INIT_WINDOW");
		LOGI(" Project: Hairy-Spice");
		
		displayInit(engine);
		LOGI(" Loading Shaders");
		vertexShader		= LoadShader(GL_VERTEX_SHADER,   vShaderStr );
		fragmentShader		= LoadShader(GL_FRAGMENT_SHADER, fShaderStr );
		
		programObject = glCreateProgram();						checkGlError(" glCreateProgram");
   
		if(!programObject) LOGI(" Problems with ProgramObject");

		glAttachShader(programObject, vertexShader );			checkGlError(" glAttachShader: VertexShader");
		glAttachShader(programObject, fragmentShader );			checkGlError(" glAttachShader: FragmentShader");
		
		// Link the program
		glLinkProgram( programObject );							checkGlError(" glLinkProgram");

		// Check the link status
		glGetProgramiv( programObject, GL_LINK_STATUS, &linked );		
				
		// Store the program object
		engine->programObject = programObject;				
		glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );				checkGlError(" glClearColor");
		
		LOGI(" Finished with EGL/OGL es 2.0 setup :D");
		
		// Init Camera
		Render(engine);		
		break;
		
		case APP_CMD_TERM_WINDOW:
		LOGI(" App Status: APP_CMD_TERM_WINDOW, Buh bye");
		// The window is being hidden or closed, clean it up.
		displayEnd(engine);
		break;
		
		case APP_CMD_LOST_FOCUS:
		LOGI(" App Status: APP_CMD_LOST_FOCUS");
		Render(engine);
		break;
	}
}




void displayEnd(struct engine* engine) {
	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}




int32_t handle_Input(struct android_app* app, AInputEvent* event) {
	struct engine* engine = (struct engine*)app->userData;
	
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		engine->touchX = AMotionEvent_getX(event, 0);
		engine->touchY = AMotionEvent_getY(event, 0);
		LOGI("x %d\ty %d\n",engine->touchX,engine->touchY);
		return 1;
	}
	return 0;
}



void android_main(struct android_app* state) {
	
	app_dummy();
	struct engine engine;

	memset(&engine, 0, sizeof(engine));
	state->userData			= &engine;
	state->onAppCmd			= handle_Cmd;
	state->onInputEvent		= handle_Input;
	engine.app				= state;


	// Read all pending events.
	while (1) {
		int ident;
		int events;
		struct android_poll_source* source;

		while ((ident=ALooper_pollAll(0, NULL, &events,(void**)&source)) >= 0) {

		// Process this event.
			if (source != NULL) {
				source->process(state, source);
			}

		// Check if we are exiting.
			if (state->destroyRequested != 0) {
				displayEnd(&engine);
				return;
			}
		}

	// Render the current frame
	Render(&engine);
	}
}
