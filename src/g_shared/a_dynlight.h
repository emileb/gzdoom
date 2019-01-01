#pragma once
#include "c_cvars.h"
#include "actor.h"
#include "cycler.h"
#include "g_levellocals.h"

EXTERN_CVAR(Bool, r_dynlights)
EXTERN_CVAR(Bool, gl_lights)
EXTERN_CVAR(Bool, gl_attachedlights)

struct side_t;
struct seg_t;

class FSerializer;

enum ELightType
{
	PointLight,
	PulseLight,
	FlickerLight,
	RandomFlickerLight,
	SectorLight,
	DummyLight,
	ColorPulseLight,
	ColorFlickerLight,
	RandomColorFlickerLight
};

enum
{
	LIGHT_RED = 0,
	LIGHT_GREEN = 1,
	LIGHT_BLUE = 2,
	LIGHT_INTENSITY = 3,
	LIGHT_SECONDARY_INTENSITY = 4,
};

enum LightFlag
{
	LF_SUBTRACTIVE = 1,
	LF_ADDITIVE = 2,
	LF_DONTLIGHTSELF = 4,
	LF_ATTENUATE = 8,
	LF_NOSHADOWMAP = 16,
	LF_DONTLIGHTACTORS = 32,
	LF_SPOT = 64
};

//==========================================================================
//
// Light definitions
//
//==========================================================================
class FLightDefaults
{
public:
	FLightDefaults(FName name, ELightType type);

	void ApplyProperties(FDynamicLight * light) const;
	FName GetName() const { return m_Name; }
	void SetParameter(double p) { m_Param = p; }
	void SetArg(int arg, int val) { m_Args[arg] = val; }
	int GetArg(int arg) { return m_Args[arg]; }
	uint8_t GetAttenuate() const { return m_attenuate; }
	void SetOffset(float* ft) { m_Pos.X = ft[0]; m_Pos.Z = ft[1]; m_Pos.Y = ft[2]; }
	void SetSubtractive(bool subtract) { m_subtractive = subtract; }
	void SetAdditive(bool add) { m_additive = add; }
	void SetDontLightSelf(bool add) { m_dontlightself = add; }
	void SetAttenuate(bool on) { m_attenuate = on; }
	void SetHalo(bool halo) { m_halo = halo; }
	void SetDontLightActors(bool on) { m_dontlightactors = on; }
	void SetSpot(bool spot) { m_spot = spot; }
	void SetSpotInnerAngle(double angle) { m_spotInnerAngle = angle; }
	void SetSpotOuterAngle(double angle) { m_spotOuterAngle = angle; }

	void OrderIntensities()
	{
		if (m_Args[LIGHT_INTENSITY] > m_Args[LIGHT_SECONDARY_INTENSITY])
		{
			std::swap(m_Args[LIGHT_INTENSITY], m_Args[LIGHT_SECONDARY_INTENSITY]);
			m_swapped = true;
		}
	}

protected:
	FName m_Name = NAME_None;
	int m_Args[5] = { 0,0,0,0,0 };
	double m_Param = 0;
	DVector3 m_Pos = { 0,0,0 };
	ELightType m_type;
	int8_t m_attenuate = -1;
	bool m_subtractive = false;
	bool m_additive = false;
	bool m_halo = false;
	bool m_dontlightself = false;
	bool m_dontlightactors = false;
	bool m_swapped = false;
	bool m_spot = false;
	bool m_explicitPitch = false;
	DAngle m_spotInnerAngle = 10.0;
	DAngle m_spotOuterAngle = 25.0;
	DAngle m_pitch = 0.0;
};

//==========================================================================
//
// Light associations (intermediate parser data)
//
//==========================================================================

class FLightAssociation
{
public:
	//FLightAssociation();
	FLightAssociation(FName actorName, const char *frameName, FName lightName)
		: m_ActorName(actorName), m_AssocLight(lightName)
	{
		strncpy(m_FrameName, frameName, 8);
	}

	FName ActorName() { return m_ActorName; }
	const char *FrameName() { return m_FrameName; }
	FName Light() { return m_AssocLight; }
	void ReplaceLightName(FName newName) { m_AssocLight = newName; }
protected:
	char m_FrameName[8];
	FName m_ActorName, m_AssocLight;
};


//==========================================================================
//
// Light associations per actor class
//
//==========================================================================

class FInternalLightAssociation
{
public:
	FInternalLightAssociation(FLightAssociation * asso);
	int Sprite() const { return m_sprite; }
	int Frame() const { return m_frame; }
	const FLightDefaults *Light() const { return m_AssocLight; }
protected:
	int m_sprite;
	int m_frame;
	FLightDefaults * m_AssocLight;
};



typedef TFlags<LightFlag> LightFlags;
DEFINE_TFLAGS_OPERATORS(LightFlags)


struct FLightNode
{
	FLightNode ** prevTarget;
	FLightNode * nextTarget;
	FLightNode ** prevLight;
	FLightNode * nextLight;
	FDynamicLight * lightsource;
	union
	{
		side_t * targLine;
		subsector_t * targSubsector;
		void * targ;
	};
};

struct FDynamicLight
{
	friend class FLightDefaults;

	inline DVector3 PosRelative(int portalgroup) const
	{
		return Pos + level.Displacements.getOffset(Sector->PortalGroup, portalgroup);
	}

	bool ShouldLightActor(AActor *check)
	{
		return visibletoplayer && IsActive() && (!(lightflags & LF_DONTLIGHTSELF) || target != check) && !(lightflags&LF_DONTLIGHTACTORS);
	}

	void SetOffset(const DVector3 &pos)
	{
		m_off = pos;
	}


	bool IsActive() const { return m_active; }
	float GetRadius() const { return (IsActive() ? m_currentRadius * 2.f : 0.f); }
	int GetRed() const { return pArgs[LIGHT_RED]; }
	int GetGreen() const { return pArgs[LIGHT_GREEN]; }
	int GetBlue() const { return pArgs[LIGHT_BLUE]; }
	int GetIntensity() const { return pArgs[LIGHT_INTENSITY]; }
	int GetSecondaryIntensity() const { return pArgs[LIGHT_SECONDARY_INTENSITY]; }

	bool IsSubtractive() const { return !!(lightflags & LF_SUBTRACTIVE); }
	bool IsAdditive() const { return !!(lightflags & LF_ADDITIVE); }
	bool IsSpot() const { return !!(lightflags & LF_SPOT); }
	void Deactivate() { m_active = false; }
	void Activate();

	void SetActor(AActor *ac, bool isowned) { target = ac; owned = isowned; }
	double X() const { return Pos.X; }
	double Y() const { return Pos.Y; }
	double Z() const { return Pos.Z; }

	void Tick();
	void UpdateLocation();
	void LinkLight();
	void UnlinkLight();
	void ReleaseLight();

private:
	double DistToSeg(const DVector3 &pos, seg_t *seg);
	void CollectWithinRadius(const DVector3 &pos, subsector_t *subSec, float radius);

public:
	FCycler m_cycler;
	DVector3 Pos;
	DVector3 m_off;

	// This date can either come from the owning actor or from a light definition
	// To avoid having to copy these around every tic, these are pointers to the source data.
	const DAngle *pSpotInnerAngle;
	const DAngle *pSpotOuterAngle;
	const DAngle *pPitch;	// This is to handle pitch overrides through GLDEFS, it can either point to the target's pitch or the light definition.
	const int *pArgs;

	double specialf1;
	FDynamicLight *next, *prev;
	sector_t *Sector;
	TObjPtr<AActor *> target;
	FLightNode * touching_sides;
	FLightNode * touching_subsectors;
	FLightNode * touching_sector;
	LightFlags lightflags;
	float radius;			// The maximum size the light can be with its current settings.
	float m_currentRadius;	// The current light size.
	int m_tickCount;
	int m_lastUpdate;
	int mShadowmapIndex;
	bool m_active;
	bool visibletoplayer;
	bool shadowmapped;
	uint8_t lighttype;
	bool owned;
	bool swapped;
	bool explicitpitch;

};
