
LOCAL_PATH := $(call my-dir)/../asmjit

include $(CLEAR_VARS)

LOCAL_MODULE    := asmjit_3.8.2

LOCAL_CFLAGS := -frtti -DASMJIT_BUILD_EMBED -DASMJIT_STATIC -DASMJIT_BUILD_X86 #-DASMJIT_ARCH_X86=1

LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/asmjit/

LOCAL_SRC_FILES =  	\
   asmjit/base/arch.cpp \
    asmjit/base/assembler.cpp \
    asmjit/base/codebuilder.cpp \
    asmjit/base/codecompiler.cpp \
    asmjit/base/codeemitter.cpp \
    asmjit/base/codeholder.cpp \
    asmjit/base/constpool.cpp \
    asmjit/base/cpuinfo.cpp \
    asmjit/base/func.cpp \
    asmjit/base/globals.cpp \
    asmjit/base/inst.cpp \
    asmjit/base/logging.cpp \
    asmjit/base/operand.cpp \
    asmjit/base/osutils.cpp \
    asmjit/base/regalloc.cpp \
    asmjit/base/runtime.cpp \
    asmjit/base/string.cpp \
    asmjit/base/utils.cpp \
    asmjit/base/vmem.cpp \
    asmjit/base/zone.cpp \
    asmjit/x86/x86assembler.cpp \
    asmjit/x86/x86builder.cpp \
    asmjit/x86/x86compiler.cpp \
    asmjit/x86/x86inst.cpp \
    asmjit/x86/x86instimpl.cpp \
    asmjit/x86/x86internal.cpp \
    asmjit/x86/x86logging.cpp \
    asmjit/x86/x86operand.cpp \
    asmjit/x86/x86operand_regs.cpp \
    asmjit/x86/x86regalloc.cpp \

LOCAL_CFLAGS += -fvisibility=hidden -fdata-sections -ffunction-sections

include $(BUILD_STATIC_LIBRARY)








