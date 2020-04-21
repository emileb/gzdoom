LOCAL_PATH := $(call my-dir)/../src


include $(CLEAR_VARS)

#LOCAL_SHORT_COMMANDS := true

LOCAL_MODULE    := g4

LOCAL_CFLAGS   := -DNO_CLOCK_GETTIME -DUSE_GL_HW_BUFFERS -fvisibility=hidden -frtti  -D__MOBILE__  -DOPNMIDI_DISABLE_GX_EMULATOR -DGZDOOM  -DGZDOOM_GL3 -D__STDINT_LIMITS -DENGINE_NAME=\"gzdoom_dev\"
#-DNO_PIX_BUFF
#-DUSE_GL_HW_BUFFERS
#-DHAVE_VULKAN
#-DUSE_GL_HW_BUFFERS

LOCAL_CPPFLAGS := -include g_pch.h -DHAVE_FLUIDSYNTH -DHAVE_MPG123 -DHAVE_SNDFILE -std=c++14  -Wno-inconsistent-missing-override -Werror=format-security  -fexceptions -fpermissive -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -D__forceinline=inline -DNO_GTK -DNO_SSE -fsigned-char

LOCAL_CFLAGS  += -DNO_SEND_STATS

LOCAL_CFLAGS  += -DOPNMIDI_USE_LEGACY_EMULATOR
LOCAL_CFLAGS  += -DADLMIDI_DISABLE_MUS_SUPPORT -DADLMIDI_DISABLE_XMI_SUPPORT -DADLMIDI_DISABLE_MIDI_SEQUENCER
LOCAL_CFLAGS  += -DOPNMIDI_DISABLE_MUS_SUPPORT -DOPNMIDI_DISABLE_XMI_SUPPORT -DOPNMIDI_DISABLE_MIDI_SEQUENCER

ifeq ($(BUILD_SERIAL),1)
LOCAL_CPPFLAGS += -DANTI_HACK 
endif

	
LOCAL_C_INCLUDES := \
 $(TOP_DIR)/ \
 $(TOP_DIR)/AudioLibs_OpenTouch/fluidsynth-lite/include \
	 $(GZDOOM_TOP_PATH)/src/g_statusbar \
	$(GZDOOM_TOP_PATH)/src/console \
	$(GZDOOM_TOP_PATH)/src/playsim \
	$(GZDOOM_TOP_PATH)/src/playsim/bots \
	$(GZDOOM_TOP_PATH)/src/playsim/mapthinkers \
	$(GZDOOM_TOP_PATH)/src/gamedata \
	$(GZDOOM_TOP_PATH)/src/gamedata/textures \
	$(GZDOOM_TOP_PATH)/src/gamedata/fonts \
	$(GZDOOM_TOP_PATH)/src/rendering \
	$(GZDOOM_TOP_PATH)/src/rendering/2d \
	$(GZDOOM_TOP_PATH)/src/r_data \
	$(GZDOOM_TOP_PATH)/src/sound \
	$(GZDOOM_TOP_PATH)/src/sound/music \
	$(GZDOOM_TOP_PATH)/src/sound/backend \
	$(GZDOOM_TOP_PATH)/src/xlat \
	$(GZDOOM_TOP_PATH)/src/utility \
	$(GZDOOM_TOP_PATH)/src/utility/nodebuilder \
	$(GZDOOM_TOP_PATH)/src/scripting \
	$(GZDOOM_TOP_PATH)/src/scripting/vm \
	$(GZDOOM_TOP_PATH)/src/rendering \
	$(GZDOOM_TOP_PATH)/src/rendering/vulkan/thirdparty \
	$(GZDOOM_TOP_PATH)/src/../libraries/gdtoa \
    $(GZDOOM_TOP_PATH)/src/../libraries/bzip2 \
	$(GZDOOM_TOP_PATH)/src/../libraries/game-music-emu/ \
	$(GZDOOM_TOP_PATH)/src/../libraries/dumb/include \
	$(GZDOOM_TOP_PATH)/src/../libraries/glslang/glslang/Public \
	$(GZDOOM_TOP_PATH)/src/../libraries/glslang/spirv \
	$(GZDOOM_TOP_PATH)/src/../libraries/lzma/C \
	$(GZDOOM_TOP_PATH)/src/../libraries/zmusic \
	$(GZDOOM_TOP_PATH)/src/posix \
	$(GZDOOM_TOP_PATH)/src/posix/sdl \
 $(SDL_INCLUDE_PATHS) \
 $(TOP_DIR)/AudioLibs_OpenTouch/openal/include/AL \
 $(TOP_DIR)/jpeg8d \
 $(TOP_DIR)/Clibs_OpenTouch \
 $(TOP_DIR)/jwzgles \
 $(TOP_DIR)/MobileTouchControls  \
 $(GZDOOM_TOP_PATH)/mobile/src/extrafiles  \
 $(GZDOOM_TOP_PATH)/mobile/src


#############################################################################
# CLIENT/SERVER
#############################################################################


ANDROID_SRC_FILES = \
    ../../../Clibs_OpenTouch/idtech1/gzdoom_game_interface.cpp \
    ../../../Clibs_OpenTouch/idtech1/touch_interface.cpp \
    ../../../Clibs_OpenTouch/idtech1/android_jni.cpp \
    ../mobile/src/i_specialpaths_android.cpp \

    #../mobile/src/extrafiles/etc1.cpp

PLAT_POSIX_SOURCES = \
	posix/i_steam.cpp

PLAT_SDL_SOURCES = \
		posix/sdl/crashcatcher.c \
    	posix/sdl/hardware.cpp \
    	posix/sdl/i_gui.cpp \
    	posix/sdl/i_input.cpp \
    	posix/sdl/i_joystick.cpp \
    	posix/sdl/i_main.cpp \
    	posix/sdl/i_system.cpp \
    	posix/sdl/sdlglvideo.cpp \
    	posix/sdl/st_start.cpp

VULKAN_SOURCES = \
	rendering/vulkan/system/vk_device.cpp \
	rendering/vulkan/system/vk_swapchain.cpp \
	rendering/vulkan/system/vk_builders.cpp \
	rendering/vulkan/system/vk_framebuffer.cpp \
	rendering/vulkan/system/vk_buffers.cpp \
	rendering/vulkan/renderer/vk_renderstate.cpp \
	rendering/vulkan/renderer/vk_renderpass.cpp \
	rendering/vulkan/renderer/vk_streambuffer.cpp \
	rendering/vulkan/renderer/vk_postprocess.cpp \
	rendering/vulkan/renderer/vk_renderbuffers.cpp \
	rendering/vulkan/shaders/vk_shader.cpp \
	rendering/vulkan/textures/vk_samplers.cpp \
	rendering/vulkan/textures/vk_hwtexture.cpp \
	rendering/vulkan/textures/vk_imagetransition.cpp \
	rendering/vulkan/thirdparty/volk/volk.c \
	rendering/vulkan/thirdparty/vk_mem_alloc/vk_mem_alloc.cpp \






POLYRENDER_SOURCES = \
	rendering/polyrenderer/poly_renderer.cpp \
	rendering/polyrenderer/poly_renderthread.cpp \
	rendering/polyrenderer/scene/poly_scene.cpp \
	rendering/polyrenderer/scene/poly_portal.cpp \
	rendering/polyrenderer/scene/poly_cull.cpp \
	rendering/polyrenderer/scene/poly_decal.cpp \
	rendering/polyrenderer/scene/poly_particle.cpp \
	rendering/polyrenderer/scene/poly_plane.cpp \
	rendering/polyrenderer/scene/poly_playersprite.cpp \
	rendering/polyrenderer/scene/poly_wall.cpp \
	rendering/polyrenderer/scene/poly_wallsprite.cpp \
	rendering/polyrenderer/scene/poly_sprite.cpp \
	rendering/polyrenderer/scene/poly_model.cpp \
	rendering/polyrenderer/scene/poly_sky.cpp \
	rendering/polyrenderer/scene/poly_light.cpp \
	rendering/polyrenderer/drawers/poly_buffer.cpp \
	rendering/polyrenderer/drawers/poly_triangle.cpp \
	rendering/polyrenderer/drawers/poly_draw_args.cpp \
	rendering/polyrenderer/drawers/screen_triangle.cpp \
	rendering/polyrenderer/math/gpu_types.cpp \

POLYBACKEND_SOURCES = \
	rendering/polyrenderer/backend/poly_framebuffer.cpp \
	rendering/polyrenderer/backend/poly_buffers.cpp \
	rendering/polyrenderer/backend/poly_hwtexture.cpp \
	rendering/polyrenderer/backend/poly_renderstate.cpp \



 FASTMATH_SOURCES = \
	rendering/swrenderer/r_all.cpp \
	rendering/swrenderer/r_swscene.cpp \
	rendering/polyrenderer/poly_all.cpp \
	sound/music/music_midi_base.cpp \
	sound/backend/oalsound.cpp \
	gamedata/textures/hires/hqnx/init.cpp \
	gamedata/textures/hires/hqnx/hq2x.cpp \
	gamedata/textures/hires/hqnx/hq3x.cpp \
	gamedata/textures/hires/hqnx/hq4x.cpp \
	gamedata/textures/hires/xbr/xbrz.cpp \
	gamedata/textures/hires/xbr/xbrz_old.cpp \
	rendering/gl_load/gl_load.c \
	rendering/hwrenderer/dynlights/hw_dynlightdata.cpp \
	rendering/hwrenderer/scene/hw_bsp.cpp \
	rendering/hwrenderer/scene/hw_fakeflat.cpp \
	rendering/hwrenderer/scene/hw_decal.cpp \
	rendering/hwrenderer/scene/hw_drawinfo.cpp \
	rendering/hwrenderer/scene/hw_drawlist.cpp \
	rendering/hwrenderer/scene/hw_clipper.cpp \
	rendering/hwrenderer/scene/hw_flats.cpp \
	rendering/hwrenderer/scene/hw_portal.cpp \
	rendering/hwrenderer/scene/hw_renderhacks.cpp \
	rendering/hwrenderer/scene/hw_sky.cpp \
	rendering/hwrenderer/scene/hw_skyportal.cpp \
	rendering/hwrenderer/scene/hw_sprites.cpp \
	rendering/hwrenderer/scene/hw_spritelight.cpp \
	rendering/hwrenderer/scene/hw_walls.cpp \
	rendering/hwrenderer/scene/hw_walls_vertex.cpp \
	rendering/hwrenderer/scene/hw_weapon.cpp \
	r_data/models/models.cpp \
	utility/matrix.cpp \



PCH_SOURCES = \
	am_map.cpp \
	playsim/bots/b_bot.cpp \
	playsim/bots/b_func.cpp \
	playsim/bots/b_game.cpp \
	playsim/bots/b_move.cpp \
	playsim/bots/b_think.cpp \
	bbannouncer.cpp \
	console/c_bind.cpp \
	console/c_cmds.cpp \
	console/c_console.cpp \
	console/c_consolebuffer.cpp \
	console/c_cvars.cpp \
	console/c_dispatch.cpp \
	console/c_expr.cpp \
	console/c_functions.cpp \
	ct_chat.cpp \
	d_iwad.cpp \
	d_main.cpp \
	d_anonstats.cpp \
	d_net.cpp \
	d_netinfo.cpp \
	d_protocol.cpp \
	dobject.cpp \
	dobjgc.cpp \
	dobjtype.cpp \
	doomstat.cpp \
	g_cvars.cpp \
	g_dumpinfo.cpp \
	g_game.cpp \
	g_hub.cpp \
	g_level.cpp \
	gameconfigfile.cpp \
	gitinfo.cpp \
	hu_scores.cpp \
	i_net.cpp \
	m_cheat.cpp \
	m_joy.cpp \
	m_misc.cpp \
	playsim/p_acs.cpp \
	playsim/p_actionfunctions.cpp \
	p_conversation.cpp \
	playsim/p_destructible.cpp \
	playsim/p_effect.cpp \
	playsim/p_enemy.cpp \
	playsim/p_interaction.cpp \
	playsim/p_lnspec.cpp \
	playsim/p_map.cpp \
	playsim/p_maputl.cpp \
	playsim/p_mobj.cpp \
	p_openmap.cpp \
	playsim/p_pspr.cpp \
	p_saveg.cpp \
	p_setup.cpp \
	playsim/p_spec.cpp \
	p_states.cpp \
	playsim/p_things.cpp \
	p_tick.cpp \
	playsim/p_user.cpp \
	rendering/r_utility.cpp \
	rendering/r_sky.cpp \
	rendering/r_videoscale.cpp \
	sound/s_advsound.cpp \
	sound/s_environment.cpp \
	sound/s_reverbedit.cpp \
	sound/s_sndseq.cpp \
	sound/s_doomsound.cpp \
	sound/s_sound.cpp \
	sound/s_music.cpp \
	serializer.cpp \
	scriptutil.cpp \
	st_stuff.cpp \
	rendering/v_framebuffer.cpp \
	r_data/v_palette.cpp \
	rendering/v_video.cpp \
	wi_stuff.cpp \
	gamedata/a_keys.cpp \
	gamedata/a_weapons.cpp \
	gamedata/decallib.cpp \
	gamedata/g_mapinfo.cpp \
	gamedata/g_skill.cpp \
	gamedata/gi.cpp \
	gamedata/stringtable.cpp \
	gamedata/umapinfo.cpp \
	gamedata/w_wad.cpp \
	gamedata/d_dehacked.cpp \
	gamedata/g_doomedmap.cpp \
	gamedata/info.cpp \
	gamedata/keysections.cpp \
	gamedata/p_terrain.cpp \
	gamedata/statistics.cpp \
	gamedata/teaminfo.cpp \
	playsim/mapthinkers/a_decalfx.cpp \
	playsim/mapthinkers/a_doors.cpp \
	playsim/mapthinkers/a_lightning.cpp \
	playsim/mapthinkers/a_quake.cpp \
	playsim/mapthinkers/a_ceiling.cpp \
	playsim/mapthinkers/a_floor.cpp \
	playsim/mapthinkers/a_lights.cpp \
	playsim/mapthinkers/a_lighttransfer.cpp \
	playsim/mapthinkers/a_pillar.cpp \
	playsim/mapthinkers/a_plats.cpp \
	playsim/mapthinkers/a_pusher.cpp \
	playsim/mapthinkers/a_scroll.cpp \
	playsim/mapthinkers/dsectoreffect.cpp \
	playsim/a_pickups.cpp \
	playsim/a_action.cpp \
	playsim/a_decals.cpp \
	playsim/a_dynlight.cpp \
	playsim/a_flashfader.cpp \
	playsim/a_morph.cpp \
	playsim/a_specialspot.cpp \
	playsim/p_secnodes.cpp \
	playsim/p_sectors.cpp \
	playsim/p_sight.cpp \
	playsim/p_switch.cpp \
	playsim/p_tags.cpp \
	playsim/p_teleport.cpp \
	playsim/actorptrselect.cpp \
	playsim/dthinker.cpp \
	playsim/p_3dfloors.cpp \
	playsim/p_3dmidtex.cpp \
	playsim/p_linkedsectors.cpp \
	playsim/p_trace.cpp \
	playsim/po_man.cpp \
	playsim/portal.cpp \
	g_statusbar/hudmessages.cpp \
	g_statusbar/shared_hud.cpp \
	g_statusbar/sbarinfo.cpp \
	g_statusbar/sbar_mugshot.cpp \
	g_statusbar/shared_sbar.cpp \
	rendering/2d/f_wipe.cpp \
	rendering/2d/v_2ddrawer.cpp \
	rendering/2d/v_drawtext.cpp \
	rendering/2d/v_blend.cpp \
	rendering/2d/v_draw.cpp \
	rendering/gl/renderer/gl_renderer.cpp \
	rendering/gl/renderer/gl_renderstate.cpp \
	rendering/gl/renderer/gl_renderbuffers.cpp \
	rendering/gl/renderer/gl_postprocess.cpp \
	rendering/gl/renderer/gl_postprocessstate.cpp \
	rendering/gl/renderer/gl_stereo3d.cpp \
	rendering/gl/renderer/gl_scene.cpp \
	rendering/gl/shaders/gl_shader.cpp \
	rendering/gl/shaders/gl_shaderprogram.cpp \
	rendering/gl_load/gl_interface.cpp \
	rendering/gl/system/gl_framebuffer.cpp \
	rendering/gl/system/gl_debug.cpp \
	rendering/gl/system/gl_buffers.cpp \
	rendering/gl/textures/gl_hwtexture.cpp \
	rendering/gl/textures/gl_samplers.cpp \
	rendering/hwrenderer/data/hw_vertexbuilder.cpp \
	rendering/hwrenderer/data/flatvertices.cpp \
	rendering/hwrenderer/data/hw_viewpointbuffer.cpp \
	rendering/hwrenderer/dynlights/hw_aabbtree.cpp \
	rendering/hwrenderer/dynlights/hw_shadowmap.cpp \
	rendering/hwrenderer/dynlights/hw_lightbuffer.cpp \
	rendering/hwrenderer/models/hw_models.cpp \
	rendering/hwrenderer/scene/hw_skydome.cpp \
	rendering/hwrenderer/scene/hw_drawlistadd.cpp \
	rendering/hwrenderer/scene/hw_renderstate.cpp \
	rendering/hwrenderer/postprocessing/hw_postprocess.cpp \
	rendering/hwrenderer/postprocessing/hw_postprocess_cvars.cpp \
	rendering/hwrenderer/postprocessing/hw_postprocessshader.cpp \
	rendering/hwrenderer/textures/hw_material.cpp \
	rendering/hwrenderer/textures/hw_precache.cpp \
	rendering/hwrenderer/utility/hw_clock.cpp \
	rendering/hwrenderer/utility/hw_cvars.cpp \
	rendering/hwrenderer/utility/hw_draw2d.cpp \
	rendering/hwrenderer/utility/hw_lighting.cpp \
	rendering/hwrenderer/utility/hw_shaderpatcher.cpp \
	rendering/hwrenderer/utility/hw_vrmodes.cpp \
	maploader/edata.cpp \
	maploader/specials.cpp \
	maploader/maploader.cpp \
	maploader/slopes.cpp \
	maploader/glnodes.cpp \
	maploader/udmf.cpp \
	maploader/usdf.cpp \
	maploader/strifedialogue.cpp \
	maploader/polyobjects.cpp \
	maploader/renderinfo.cpp \
	maploader/compatibility.cpp \
	maploader/postprocessor.cpp \
	menu/joystickmenu.cpp \
	menu/loadsavemenu.cpp \
	menu/menu.cpp \
	menu/menudef.cpp \
	menu/messagebox.cpp \
	menu/optionmenu.cpp \
	menu/playermenu.cpp \
	menu/resolutionmenu.cpp \
	gamedata/resourcefiles/ancientzip.cpp \
	gamedata/resourcefiles/file_7z.cpp \
	gamedata/resourcefiles/file_grp.cpp \
	gamedata/resourcefiles/file_lump.cpp \
	gamedata/resourcefiles/file_rff.cpp \
	gamedata/resourcefiles/file_wad.cpp \
	gamedata/resourcefiles/file_zip.cpp \
	gamedata/resourcefiles/file_pak.cpp \
	gamedata/resourcefiles/file_directory.cpp \
	gamedata/resourcefiles/resourcefile.cpp \
	gamedata/textures/animations.cpp \
	gamedata/textures/anim_switches.cpp \
	gamedata/textures/bitmap.cpp \
	gamedata/textures/texture.cpp \
	gamedata/textures/image.cpp \
	gamedata/textures/imagetexture.cpp \
	gamedata/textures/texturemanager.cpp \
	gamedata/textures/multipatchtexturebuilder.cpp \
	gamedata/textures/skyboxtexture.cpp \
	gamedata/textures/formats/automaptexture.cpp \
	gamedata/textures/formats/brightmaptexture.cpp \
	gamedata/textures/formats/buildtexture.cpp \
	gamedata/textures/formats/canvastexture.cpp \
	gamedata/textures/formats/ddstexture.cpp \
	gamedata/textures/formats/flattexture.cpp \
	gamedata/textures/formats/fontchars.cpp \
	gamedata/textures/formats/imgztexture.cpp \
	gamedata/textures/formats/jpegtexture.cpp \
	gamedata/textures/formats/md5check.cpp \
	gamedata/textures/formats/multipatchtexture.cpp \
	gamedata/textures/formats/patchtexture.cpp \
	gamedata/textures/formats/pcxtexture.cpp \
	gamedata/textures/formats/pngtexture.cpp \
	gamedata/textures/formats/rawpagetexture.cpp \
	gamedata/textures/formats/emptytexture.cpp \
	gamedata/textures/formats/shadertexture.cpp \
	gamedata/textures/formats/tgatexture.cpp \
	gamedata/textures/formats/stbtexture.cpp \
	gamedata/textures/hires/hqresize.cpp \
	gamedata/textures/hires/hirestex.cpp \
	gamedata/fonts/singlelumpfont.cpp \
	gamedata/fonts/singlepicfont.cpp \
	gamedata/fonts/specialfont.cpp \
	gamedata/fonts/font.cpp \
	gamedata/fonts/hexfont.cpp \
	gamedata/fonts/v_font.cpp \
	gamedata/fonts/v_text.cpp \
	gamedata/p_xlat.cpp \
	gamedata/xlat/parse_xlat.cpp \
	gamedata/xlat/parsecontext.cpp \
	playsim/fragglescript/t_func.cpp \
	playsim/fragglescript/t_load.cpp \
	playsim/fragglescript/t_oper.cpp \
	playsim/fragglescript/t_parse.cpp \
	playsim/fragglescript/t_prepro.cpp \
	playsim/fragglescript/t_script.cpp \
	playsim/fragglescript/t_spec.cpp \
	playsim/fragglescript/t_variable.cpp \
	playsim/fragglescript/t_cmd.cpp \
	intermission/intermission.cpp \
	intermission/intermission_parse.cpp \
	r_data/colormaps.cpp \
	r_data/cycler.cpp \
	r_data/gldefs.cpp \
	r_data/a_dynlightdata.cpp \
	r_data/r_translate.cpp \
	r_data/sprites.cpp \
	r_data/portalgroups.cpp \
	r_data/voxels.cpp \
	r_data/renderstyle.cpp \
	r_data/r_canvastexture.cpp \
	r_data/r_interpolate.cpp \
	r_data/r_vanillatrans.cpp \
	r_data/r_sections.cpp \
	r_data/models/models_md3.cpp \
	r_data/models/models_md2.cpp \
	r_data/models/models_voxel.cpp \
	r_data/models/models_ue1.cpp \
	r_data/models/models_obj.cpp \
	scripting/symbols.cpp \
	scripting/vmiterators.cpp \
	scripting/vmthunks.cpp \
	scripting/vmthunks_actors.cpp \
	scripting/types.cpp \
	scripting/thingdef.cpp \
	scripting/thingdef_data.cpp \
	scripting/thingdef_properties.cpp \
	scripting/backend/codegen.cpp \
	scripting/backend/scopebarrier.cpp \
	scripting/backend/dynarrays.cpp \
	scripting/backend/vmbuilder.cpp \
	scripting/backend/vmdisasm.cpp \
	scripting/decorate/olddecorations.cpp \
	scripting/decorate/thingdef_exp.cpp \
	scripting/decorate/thingdef_parse.cpp \
	scripting/decorate/thingdef_states.cpp \
	scripting/vm/vmexec.cpp \
	scripting/vm/vmframe.cpp \
	scripting/zscript/ast.cpp \
	scripting/zscript/zcc_compile.cpp \
	scripting/zscript/zcc_parser.cpp \
	utility/sfmt/SFMT.cpp \
	sound/music/i_music.cpp \
	sound/music/i_soundfont.cpp \
	sound/backend/i_sound.cpp \
	sound/music/music_config.cpp \
	rendering/swrenderer/textures/r_swtexture.cpp \
	rendering/swrenderer/textures/warptexture.cpp \
	rendering/swrenderer/textures/swcanvastexture.cpp \
	events.cpp \
	utility/palette.cpp \
	utility/files.cpp \
	utility/files_decompress.cpp \
	utility/m_png.cpp \
	utility/m_random.cpp \
	utility/memarena.cpp \
	utility/md5.cpp \
	utility/nodebuilder/nodebuild.cpp \
	utility/nodebuilder/nodebuild_classify_nosse2.cpp \
	utility/nodebuilder/nodebuild_events.cpp \
	utility/nodebuilder/nodebuild_extract.cpp \
	utility/nodebuilder/nodebuild_gl.cpp \
	utility/nodebuilder/nodebuild_utility.cpp \
	utility/sc_man.cpp \
	utility/stats.cpp \
	utility/cmdlib.cpp \
	utility/configfile.cpp \
	utility/i_time.cpp \
	utility/m_alloc.cpp \
	utility/m_argv.cpp \
	utility/m_bbox.cpp \
	utility/name.cpp \
	utility/s_playlist.cpp \
	utility/v_collection.cpp \
	utility/utf8.cpp \
	utility/zstrformat.cpp \


SYSTEM_SOURCES  = ${PLAT_POSIX_SOURCES} ${PLAT_SDL_SOURCES} ${PLAT_UNIX_SOURCES}

#$(ANDROID_SRC_FILES) \

LOCAL_SRC_FILES = \
	__autostart.cpp \
	$(ANDROID_SRC_FILES) \
	${SYSTEM_SOURCES} \
	${FASTMATH_SOURCES} \
	$(POLYBACKEND_SOURCES) \
	${PCH_SOURCES} \
	utility/x86.cpp \
	utility/strnatcmp.c \
	utility/zstring.cpp \
	utility/dictionary.cpp \
	utility/math/asin.c \
	utility/math/atan.c \
	utility/math/const.c \
	utility/math/cosh.c \
	utility/math/exp.c \
	utility/math/isnan.c \
	utility/math/log.c \
	utility/math/log10.c \
	utility/math/mtherr.c \
	utility/math/polevl.c \
	utility/math/pow.c \
	utility/math/powi.c \
	utility/math/sin.c \
	utility/math/sinh.c \
	utility/math/sqrt.c \
	utility/math/tan.c \
	utility/math/tanh.c \
	utility/math/fastsin.cpp \
	zzautozend.cpp \



LOCAL_LDLIBS := -ldl -llog -lOpenSLES
#LOCAL_LDLIBS +=-lGLESv1_CM
#LOCAL_LDLIBS += -lGLESv3

LOCAL_LDLIBS +=  -lEGL -lGLESv1_CM

# This is stop a linker warning for mp123 lib failing build
#LOCAL_LDLIBS += -Wl,--no-warn-shared-textrel
#asmjit_gl3
LOCAL_STATIC_LIBRARIES :=  SDL2_net libjpeg zlib_gl3 lzma_gl3 gdtoa_gl3 dumb_gl3 gme_gl3 bzip2_gl3 zmusic_gl3 logwritter
LOCAL_SHARED_LIBRARIES := touchcontrols openal SDL2 core_shared  saffal

LOCAL_STATIC_LIBRARIES +=

#LOCAL_SHORT_COMMANDS := true

include $(BUILD_SHARED_LIBRARY)




