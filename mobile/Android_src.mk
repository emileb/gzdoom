LOCAL_PATH := $(call my-dir)/../src


include $(CLEAR_VARS)

LOCAL_SHORT_COMMANDS := true

LOCAL_MODULE    := gzdoom_gl3

LOCAL_CFLAGS   :=  -frtti -D__MOBILE__ -DOPNMIDI_DISABLE_GX_EMULATOR -DGZDOOM  -DGZDOOM_GL3 -D__STDINT_LIMITS -DENGINE_NAME=\"gzdoom_dev\"


LOCAL_CPPFLAGS := -DHAVE_FLUIDSYNTH -DHAVE_MPG123 -DHAVE_SNDFILE -std=c++14 -DHAVE_JWZGLES  -Wno-inconsistent-missing-override -Werror=format-security  -fexceptions -fpermissive -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -D__forceinline=inline -DNO_GTK -DNO_SSE -fsigned-char

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
 $(GZDOOM_TOP_PATH)/src/  \
 $(GZDOOM_TOP_PATH)/mobile/src/extrafiles  \
 $(GZDOOM_TOP_PATH)/game-music-emu/ \
 $(GZDOOM_TOP_PATH)/gdtoa \
 $(GZDOOM_TOP_PATH)/lzma/C \
 $(GZDOOM_TOP_PATH)/bzip2 \
 $(GZDOOM_TOP_PATH)/asmjit \
 $(GZDOOM_TOP_PATH)/src/g_statusbar \
 	$(GZDOOM_TOP_PATH)/src/g_shared \
 	$(GZDOOM_TOP_PATH)/src/gamedata \
 	$(GZDOOM_TOP_PATH)/src/gamedata/textures \
 	$(GZDOOM_TOP_PATH)/src/gamedata/fonts \
 	$(GZDOOM_TOP_PATH)/src/rendering \
 	$(GZDOOM_TOP_PATH)/src/rendering/2d \
 	$(GZDOOM_TOP_PATH)/src/sound \
 	$(GZDOOM_TOP_PATH)/src/sound/oplsynth \
 	$(GZDOOM_TOP_PATH)/src/sound/timidity \
 	$(GZDOOM_TOP_PATH)/src/sound/wildmidi \
 	$(GZDOOM_TOP_PATH)/src/xlat \
 	$(GZDOOM_TOP_PATH)/src/utility \
 	$(GZDOOM_TOP_PATH)/src/utility/nodebuilder \
 	$(GZDOOM_TOP_PATH)/src/scripting \
 	$(GZDOOM_TOP_PATH)/src/scripting/vm \
 	$(GZDOOM_TOP_PATH)/src/rendering/vulkan/thirdparty \
 	$(GZDOOM_TOP_PATH)/src/../gdtoa \
 	$(GZDOOM_TOP_PATH)/src/../dumb/include \
 	$(GZDOOM_TOP_PATH)/src/../glslang/glslang/Public \
 	$(GZDOOM_TOP_PATH)/src/../glslang/spirv \
 	$(GZDOOM_TOP_PATH)/src/posix \
 	$(GZDOOM_TOP_PATH)/src/posix/sdl \
 $(SDL_INCLUDE_PATHS) \
 $(TOP_DIR)/AudioLibs_OpenTouch/openal/include/AL \
 $(TOP_DIR)/AudioLibs_OpenTouch/libsndfile-android/jni/ \
 $(TOP_DIR)/AudioLibs_OpenTouch/libmpg123 \
 $(TOP_DIR)/jpeg8d \
 $(TOP_DIR)/Clibs_OpenTouch \
 $(TOP_DIR)/jwzgles \
 $(TOP_DIR)/MobileTouchControls  \
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
	posix/i_cd.cpp \
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


SWRENDER_SOURCES = \
	rendering/swrenderer/r_swcolormaps.cpp \
	rendering/swrenderer/r_swrenderer.cpp \
	rendering/swrenderer/r_memory.cpp \
	rendering/swrenderer/r_renderthread.cpp \
	rendering/swrenderer/drawers/r_draw.cpp \
	rendering/swrenderer/drawers/r_draw_pal.cpp \
	rendering/swrenderer/drawers/r_draw_rgba.cpp \
	rendering/swrenderer/drawers/r_thread.cpp \
	rendering/swrenderer/scene/r_3dfloors.cpp \
	rendering/swrenderer/scene/r_light.cpp \
	rendering/swrenderer/scene/r_opaque_pass.cpp \
	rendering/swrenderer/scene/r_portal.cpp \
	rendering/swrenderer/scene/r_scene.cpp \
	rendering/swrenderer/scene/r_translucent_pass.cpp \
	rendering/swrenderer/viewport/r_drawerargs.cpp \
	rendering/swrenderer/viewport/r_skydrawer.cpp \
	rendering/swrenderer/viewport/r_spandrawer.cpp \
	rendering/swrenderer/viewport/r_spritedrawer.cpp \
	rendering/swrenderer/viewport/r_viewport.cpp \
	rendering/swrenderer/viewport/r_walldrawer.cpp \
	rendering/swrenderer/line/r_line.cpp \
	rendering/swrenderer/line/r_farclip_line.cpp \
	rendering/swrenderer/line/r_walldraw.cpp \
	rendering/swrenderer/line/r_wallsetup.cpp \
	rendering/swrenderer/line/r_fogboundary.cpp \
	rendering/swrenderer/line/r_renderdrawsegment.cpp \
	rendering/swrenderer/segments/r_clipsegment.cpp \
	rendering/swrenderer/segments/r_drawsegment.cpp \
	rendering/swrenderer/segments/r_portalsegment.cpp \
	rendering/swrenderer/things/r_visiblesprite.cpp \
	rendering/swrenderer/things/r_visiblespritelist.cpp \
	rendering/swrenderer/things/r_voxel.cpp \
	rendering/swrenderer/things/r_particle.cpp \
	rendering/swrenderer/things/r_playersprite.cpp \
	rendering/swrenderer/things/r_sprite.cpp \
	rendering/swrenderer/things/r_wallsprite.cpp \
	rendering/swrenderer/things/r_decal.cpp \
	rendering/swrenderer/things/r_model.cpp \
	rendering/swrenderer/plane/r_visibleplane.cpp \
	rendering/swrenderer/plane/r_visibleplanelist.cpp \
	rendering/swrenderer/plane/r_skyplane.cpp \
	rendering/swrenderer/plane/r_planerenderer.cpp \
	rendering/swrenderer/plane/r_flatplane.cpp \
	rendering/swrenderer/plane/r_slopeplane.cpp \


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


VM_JIT_SOURCES = \
	scripting/vm/jit.cpp \
	scripting/vm/jit_runtime.cpp \
	scripting/vm/jit_call.cpp \
	scripting/vm/jit_flow.cpp \
	scripting/vm/jit_load.cpp \
	scripting/vm/jit_math.cpp \
	scripting/vm/jit_move.cpp \
	scripting/vm/jit_store.cpp \



 FASTMATH_SOURCES = \
	rendering/swrenderer/r_all.cpp \
	rendering/swrenderer/r_swscene.cpp \
	rendering/polyrenderer/poly_all.cpp \
	sound/oplsynth/opl_mus_player.cpp \
	sound/mpg123_decoder.cpp \
	sound/music_midi_base.cpp \
	sound/oalsound.cpp \
	sound/sndfile_decoder.cpp \
	sound/timiditypp/fft4g.cpp \
	sound/timiditypp/reverb.cpp \
	gamedata/textures/hires/hqnx/init.cpp \
	gamedata/textures/hires/hqnx/hq2x.cpp \
	gamedata/textures/hires/hqnx/hq3x.cpp \
	gamedata/textures/hires/hqnx/hq4x.cpp \
	gamedata/textures/hires/xbr/xbrz.cpp \
	gamedata/textures/hires/xbr/xbrz_old.cpp \
	rendering/gl_load/gl_load.c \
	rendering/hwrenderer/postprocessing/hw_postprocess_cvars.cpp \
	rendering/hwrenderer/postprocessing/hw_postprocessshader.cpp \
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
	r_data/matrix.cpp \
	sound/adlmidi/adldata.cpp \
	sound/adlmidi/adlmidi.cpp \
	sound/adlmidi/adlmidi_load.cpp \
	sound/adlmidi/adlmidi_midiplay.cpp \
	sound/adlmidi/adlmidi_opl3.cpp \
	sound/adlmidi/adlmidi_private.cpp \
	sound/adlmidi/chips/dosbox/dbopl.cpp \
	sound/adlmidi/chips/dosbox_opl3.cpp \
	sound/adlmidi/chips/nuked/nukedopl3_174.c \
	sound/adlmidi/chips/nuked/nukedopl3.c \
	sound/adlmidi/chips/nuked_opl3.cpp \
	sound/adlmidi/chips/nuked_opl3_v174.cpp \
	sound/adlmidi/wopl/wopl_file.c \
	sound/opnmidi/chips/gens_opn2.cpp \
	sound/opnmidi/chips/gens/Ym2612_Emu.cpp \
	sound/opnmidi/chips/mame/mame_ym2612fm.c \
	sound/opnmidi/chips/mame_opn2.cpp \
	sound/opnmidi/chips/nuked_opn2.cpp \
	sound/opnmidi/chips/nuked/ym3438.c \
	sound/opnmidi/opnmidi.cpp \
	sound/opnmidi/opnmidi_load.cpp \
	sound/opnmidi/opnmidi_midiplay.cpp \
	sound/opnmidi/opnmidi_opn2.cpp \
	sound/opnmidi/opnmidi_private.cpp \
	sound/opnmidi/wopn/wopn_file.c \

PCH_SOURCES = \
	am_map.cpp \
	b_bot.cpp \
	b_func.cpp \
	b_game.cpp \
	b_move.cpp \
	b_think.cpp \
	bbannouncer.cpp \
	c_bind.cpp \
	c_cmds.cpp \
	c_console.cpp \
	c_consolebuffer.cpp \
	c_cvars.cpp \
	c_dispatch.cpp \
	c_expr.cpp \
	c_functions.cpp \
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
	p_acs.cpp \
	p_actionfunctions.cpp \
	p_conversation.cpp \
	p_destructible.cpp \
	p_effect.cpp \
	p_enemy.cpp \
	p_interaction.cpp \
	p_lnspec.cpp \
	p_map.cpp \
	p_maputl.cpp \
	p_mobj.cpp \
	p_openmap.cpp \
	p_pspr.cpp \
	p_saveg.cpp \
	p_setup.cpp \
	p_spec.cpp \
	p_states.cpp \
	p_things.cpp \
	p_tick.cpp \
	p_user.cpp \
	r_utility.cpp \
	r_sky.cpp \
	r_videoscale.cpp \
	s_advsound.cpp \
	s_environment.cpp \
	s_sndseq.cpp \
	s_sound.cpp \
	serializer.cpp \
	scriptutil.cpp \
	st_stuff.cpp \
	v_framebuffer.cpp \
	v_palette.cpp \
	v_video.cpp \
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
	g_shared/a_pickups.cpp \
	g_shared/a_action.cpp \
	g_shared/a_decals.cpp \
	g_shared/a_decalfx.cpp \
	g_shared/a_doors.cpp \
	g_shared/a_dynlight.cpp \
	g_shared/a_flashfader.cpp \
	g_shared/a_lightning.cpp \
	g_shared/a_morph.cpp \
	g_shared/a_quake.cpp \
	g_shared/a_specialspot.cpp \
	g_shared/a_ceiling.cpp \
	g_shared/a_floor.cpp \
	g_shared/a_lights.cpp \
	g_shared/a_lighttransfer.cpp \
	g_shared/a_pillar.cpp \
	g_shared/a_plats.cpp \
	g_shared/a_pusher.cpp \
	g_shared/a_scroll.cpp \
	g_shared/dsectoreffect.cpp \
	g_shared/p_secnodes.cpp \
	g_shared/p_sectors.cpp \
	g_shared/p_sight.cpp \
	g_shared/p_switch.cpp \
	g_shared/p_tags.cpp \
	g_shared/p_teleport.cpp \
	g_shared/actorptrselect.cpp \
	g_shared/dthinker.cpp \
	g_shared/p_3dfloors.cpp \
	g_shared/p_3dmidtex.cpp \
	g_shared/p_linkedsectors.cpp \
	g_shared/p_trace.cpp \
	g_shared/po_man.cpp \
	g_shared/portal.cpp \
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
	fragglescript/t_func.cpp \
	fragglescript/t_load.cpp \
	fragglescript/t_oper.cpp \
	fragglescript/t_parse.cpp \
	fragglescript/t_prepro.cpp \
	fragglescript/t_script.cpp \
	fragglescript/t_spec.cpp \
	fragglescript/t_variable.cpp \
	fragglescript/t_cmd.cpp \
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
	sound/i_music.cpp \
	sound/i_sound.cpp \
	sound/i_soundfont.cpp \
	sound/mididevices/music_adlmidi_mididevice.cpp \
	sound/mididevices/music_opldumper_mididevice.cpp \
	sound/mididevices/music_opl_mididevice.cpp \
	sound/mididevices/music_opnmidi_mididevice.cpp \
	sound/mididevices/music_timiditypp_mididevice.cpp \
	sound/mididevices/music_fluidsynth_mididevice.cpp \
	sound/mididevices/music_softsynth_mididevice.cpp \
	sound/mididevices/music_timidity_mididevice.cpp \
	sound/mididevices/music_wildmidi_mididevice.cpp \
	sound/mididevices/music_wavewriter_mididevice.cpp \
	sound/midisources/midisource.cpp \
	sound/midisources/midisource_mus.cpp \
	sound/midisources/midisource_smf.cpp \
	sound/midisources/midisource_hmi.cpp \
	sound/midisources/midisource_xmi.cpp \
	sound/musicformats/music_cd.cpp \
	sound/musicformats/music_dumb.cpp \
	sound/musicformats/music_gme.cpp \
	sound/musicformats/music_libsndfile.cpp \
	sound/musicformats/music_midistream.cpp \
	sound/musicformats/music_opl.cpp \
	sound/musicformats/music_stream.cpp \
	sound/oplsynth/fmopl.cpp \
	sound/oplsynth/musicblock.cpp \
	sound/oplsynth/oplio.cpp \
	sound/oplsynth/dosbox/opl.cpp \
	sound/oplsynth/OPL3.cpp \
	sound/oplsynth/nukedopl3.cpp \
	sound/timidity/common.cpp \
	sound/timidity/instrum.cpp \
	sound/timidity/instrum_dls.cpp \
	sound/timidity/instrum_font.cpp \
	sound/timidity/instrum_sf2.cpp \
	sound/timidity/mix.cpp \
	sound/timidity/playmidi.cpp \
	sound/timidity/resample.cpp \
	sound/timidity/timidity.cpp \
	sound/timiditypp/common.cpp \
	sound/timiditypp/configfile.cpp \
	sound/timiditypp/effect.cpp \
	sound/timiditypp/filter.cpp \
	sound/timiditypp/freq.cpp \
	sound/timiditypp/instrum.cpp \
	sound/timiditypp/mblock.cpp \
	sound/timiditypp/mix.cpp \
	sound/timiditypp/playmidi.cpp \
	sound/timiditypp/quantity.cpp \
	sound/timiditypp/readmidic.cpp \
	sound/timiditypp/recache.cpp \
	sound/timiditypp/resample.cpp \
	sound/timiditypp/sbkconv.cpp \
	sound/timiditypp/sffile.cpp \
	sound/timiditypp/sfitem.cpp \
	sound/timiditypp/smplfile.cpp \
	sound/timiditypp/sndfont.cpp \
	sound/timiditypp/tables.cpp \
	sound/wildmidi/file_io.cpp \
	sound/wildmidi/gus_pat.cpp \
	sound/wildmidi/reverb.cpp \
	sound/wildmidi/wildmidi_lib.cpp \
	sound/wildmidi/wm_error.cpp \
	rendering/swrenderer/textures/r_swtexture.cpp \
	rendering/swrenderer/textures/warptexture.cpp \
	rendering/swrenderer/textures/swcanvastexture.cpp \
	events.cpp \
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
	utility/colormatcher.cpp \
	utility/configfile.cpp \
	utility/i_module.cpp \
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
	${PCH_SOURCES} \
	utility/x86.cpp \
	utility/strnatcmp.c \
	utility/zstring.cpp \
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

LOCAL_LDLIBS +=  -lEGL

# This is stop a linker warning for mp123 lib failing build
#LOCAL_LDLIBS += -Wl,--no-warn-shared-textrel
#asmjit_gl3
LOCAL_STATIC_LIBRARIES :=  sndfile mpg123 fluidsynth-static SDL2_net libjpeg zlib_gl3 lzma_gl3 gdtoa_gl3 dumb_gl3 gme_gl3 bzip2_gl3  logwritter
LOCAL_SHARED_LIBRARIES := touchcontrols openal SDL2 core_shared

LOCAL_STATIC_LIBRARIES +=

#LOCAL_SHORT_COMMANDS := true

include $(BUILD_SHARED_LIBRARY)




