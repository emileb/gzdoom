
LOCAL_PATH := $(call my-dir)/../libraries/glslang/glslang

include $(CLEAR_VARS)


LOCAL_MODULE    := glslang_gl3

LOCAL_CFLAGS :=

LOCAL_LDLIBS += -llog

LOCAL_C_INCLUDES :=  . $(GZDOOM_TOP_PATH)/mobile/src/extrafiles ./../


LOCAL_SRC_FILES =  	\
   MachineIndependent/glslang_tab.cpp \
    MachineIndependent/attribute.cpp \
    MachineIndependent/Constant.cpp \
    MachineIndependent/iomapper.cpp \
    MachineIndependent/InfoSink.cpp \
    MachineIndependent/Initialize.cpp \
    MachineIndependent/IntermTraverse.cpp \
    MachineIndependent/Intermediate.cpp \
    MachineIndependent/ParseContextBase.cpp \
    MachineIndependent/ParseHelper.cpp \
    MachineIndependent/PoolAlloc.cpp \
    MachineIndependent/RemoveTree.cpp \
    MachineIndependent/Scan.cpp \
    MachineIndependent/ShaderLang.cpp \
    MachineIndependent/SymbolTable.cpp \
    MachineIndependent/Versions.cpp \
    MachineIndependent/intermOut.cpp \
    MachineIndependent/limits.cpp \
    MachineIndependent/linkValidate.cpp \
    MachineIndependent/parseConst.cpp \
    MachineIndependent/reflection.cpp \
    MachineIndependent/preprocessor/Pp.cpp \
    MachineIndependent/preprocessor/PpAtom.cpp \
    MachineIndependent/preprocessor/PpContext.cpp \
    MachineIndependent/preprocessor/PpScanner.cpp \
    MachineIndependent/preprocessor/PpTokens.cpp \
    MachineIndependent/propagateNoContraction.cpp \
    GenericCodeGen/CodeGen.cpp \
    GenericCodeGen/Link.cpp \
    \
    OSDependent/Unix/ossource.cpp \
    ../OGLCompilersDLL/InitializeDll.cpp \


LOCAL_STATIC_LIBRARIES := spirv_gl3

include $(BUILD_STATIC_LIBRARY)








