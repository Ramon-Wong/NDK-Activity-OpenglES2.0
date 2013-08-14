LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := 	native-activity
LOCAL_SRC_FILES := 	Main.c \
					Function.c
                    
LOCAL_LDLIBS    := -lEGL -lGLESv2 -llog -landroid -ldl  
					
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
