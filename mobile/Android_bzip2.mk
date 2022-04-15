
LOCAL_PATH := $(call my-dir)/../bzip2

include $(CLEAR_VARS)

LOCAL_MODULE    := bzip2_1.9

LOCAL_C_INCLUDES :=   $(LOCAL_PATH)/include/

LOCAL_SRC_FILES =  	\
	  blocksort.c \
    bzlib.c \
    compress.c \
    crctable.c \
    decompress.c \
    huffman.c \
    randtable.c \

include $(BUILD_STATIC_LIBRARY)








