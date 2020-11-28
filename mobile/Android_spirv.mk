
LOCAL_PATH := $(call my-dir)/../libraries/glslang/spirv

include $(CLEAR_VARS)


LOCAL_MODULE    := spirv_gl3

LOCAL_CFLAGS :=

LOCAL_LDLIBS += -llog

LOCAL_C_INCLUDES :=  . $(GZDOOM_TOP_PATH)/mobile/src/extrafiles $(GZDOOM_TOP_PATH)/libraries/glslang


LOCAL_SRC_FILES =  	\
    GlslangToSpv.cpp \
    InReadableOrder.cpp \
    Logger.cpp \
    SpvBuilder.cpp \
    SpvPostProcess.cpp \
    doc.cpp \
    SpvTools.cpp \
    disassemble.cpp \


include $(BUILD_STATIC_LIBRARY)








