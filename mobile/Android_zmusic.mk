
LOCAL_PATH := $(call my-dir)/../libraries/zmusic


include $(CLEAR_VARS)


LOCAL_MODULE    := zmusic_gl3

LOCAL_CPPFLAGS := -frtti -DHAVE_SNDFILE -DHAVE_FLUIDSYNTH  -fexceptions -std=c++11 -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp  -fsigned-char

LOCAL_C_INCLUDES :=  \
$(LOCAL_PATH)/../adlmidi \
$(LOCAL_PATH)/../opnmidi \
$(LOCAL_PATH)/../oplmidi \
$(LOCAL_PATH)/../oplsynth \
$(LOCAL_PATH)/../oplsynth/oplsynth \
$(LOCAL_PATH)/../timidity \
$(LOCAL_PATH)/../timidityplus \
$(LOCAL_PATH)/../wildmidi \
$(LOCAL_PATH)/../game-music-emu \
$(TOP_DIR)/AudioLibs_OpenTouch/fluidsynth-lite/include \
$(TOP_DIR)/AudioLibs_OpenTouch/libsndfile-android/jni/ \
$(TOP_DIR)/AudioLibs_OpenTouch/libmpg123 \


LOCAL_SRC_FILES =  	\
i_module.cpp \
	mididevices/music_base_mididevice.cpp \
	mididevices/music_adlmidi_mididevice.cpp \
	mididevices/music_opl_mididevice.cpp \
	mididevices/music_opnmidi_mididevice.cpp \
	mididevices/music_timiditypp_mididevice.cpp \
	mididevices/music_fluidsynth_mididevice.cpp \
	mididevices/music_softsynth_mididevice.cpp \
	mididevices/music_timidity_mididevice.cpp \
	mididevices/music_wildmidi_mididevice.cpp \
	mididevices/music_wavewriter_mididevice.cpp \
	midisources/midisource.cpp \
	midisources/midisource_mus.cpp \
	midisources/midisource_smf.cpp \
	midisources/midisource_hmi.cpp \
	midisources/midisource_xmi.cpp \
	streamsources/music_dumb.cpp \
	streamsources/music_gme.cpp \
	streamsources/music_libsndfile.cpp \
	streamsources/music_opl.cpp \
	streamsources/music_xa.cpp \
	musicformats/music_stream.cpp \
	musicformats/music_midi.cpp \
	musicformats/music_cd.cpp \
	decoder/sounddecoder.cpp \
	decoder/sndfile_decoder.cpp \
	decoder/mpg123_decoder.cpp \
	zmusic/configuration.cpp \
	zmusic/zmusic.cpp \

LOCAL_STATIC_LIBRARIES := oplsynth_gl3  opnmidi_gl3 timidity_gl3 timidityplus_gl3 wildmidi_gl3 adlmidi_gl3 gme_gl3 dumb_gl3  sndfile mpg123 fluidsynth-static

include $(BUILD_STATIC_LIBRARY)








