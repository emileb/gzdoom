#pragma once

#include "actor.h"
#include "r_defs.h"
#include "g_levellocals.h"
#include "d_player.h"

// These depend on both actor.h and r_defs.h so they cannot be in either file without creating a circular dependency.

inline DVector3 AActor::PosRelative(int portalgroup) const
{
	return Pos() + Displacements.getOffset(Sector->PortalGroup, portalgroup);
}

inline DVector3 AActor::PosRelative(const AActor *other) const
{
	return Pos() + Displacements.getOffset(Sector->PortalGroup, other->Sector->PortalGroup);
}

inline DVector3 AActor::PosRelative(sector_t *sec) const
{
	return Pos() + Displacements.getOffset(Sector->PortalGroup, sec->PortalGroup);
}

inline DVector3 AActor::PosRelative(const line_t *line) const
{
	return Pos() + Displacements.getOffset(Sector->PortalGroup, line->frontsector->PortalGroup);
}

inline DVector3 PosRelative(const DVector3 &pos, line_t *line, sector_t *refsec = NULL)
{
	return pos + Displacements.getOffset(refsec->PortalGroup, line->frontsector->PortalGroup);
}


inline void AActor::ClearInterpolation()
{
	Prev = Pos();
	PrevAngles = Angles;
	if (Sector) PrevPortalGroup = Sector->PortalGroup;
	else PrevPortalGroup = 0;
}

inline double secplane_t::ZatPoint(const AActor *ac) const
{
	return (D + normal.X*ac->X() + normal.Y*ac->Y()) * negiC;
}

inline double sector_t::HighestCeilingAt(AActor *a, sector_t **resultsec)
{
	return ::HighestCeilingAt(this, a->X(), a->Y(), resultsec);
}

inline double sector_t::LowestFloorAt(AActor *a, sector_t **resultsec)
{
	return ::LowestFloorAt(this, a->X(), a->Y(), resultsec);
}

inline double AActor::GetBobOffset(double ticfrac) const
{
	if (!(flags2 & MF2_FLOATBOB))
	{
		return 0;
	}
	return BobSin(FloatBobPhase + level.maptime + ticfrac) * FloatBobStrength;
}

inline double AActor::GetCameraHeight() const
{
	return CameraHeight == INT_MIN ? Height / 2 : CameraHeight;
}


inline FDropItem *AActor::GetDropItems() const
{
	return GetInfo()->DropItems;
}

inline double AActor::GetGravity() const
{
	if (flags & MF_NOGRAVITY) return 0;
	return level.gravity * Sector->gravity * Gravity * 0.00125;
}

inline double AActor::AttackOffset(double offset)
{
	if (player != NULL)
	{
		return (FloatVar(NAME_AttackZOffset) + offset) * player->crouchfactor;
	}
	else
	{
		return 8 + offset;
	}

}

inline bool AActor::isFrozen() const
{
	if (!(flags5 & MF5_NOTIMEFREEZE))
	{
		auto state = level.isFrozen();
		if (state)
		{
			if (player == nullptr || player->Bot != nullptr) return true;

			// This is the only place in the entire game where the two freeze flags need different treatment.
			// The time freezer flag also freezes other players, the global setting does not.

			if ((state & 1) && player->timefreezer == 0)
			{
				return true;
			}
		}
	}
	return false;
}

// Consolidated from all (incomplete) variants that check if a line should block.
inline bool P_IsBlockedByLine(AActor* actor, line_t* line)
{
	// Keep this stuff readable - so no chained and nested 'if's!

	// Unconditional blockers.
	if (line->flags & (ML_BLOCKING | ML_BLOCKEVERYTHING)) return true;

	// MBF considers that friendly monsters are not blocked by monster-blocking lines.
	// This is added here as a compatibility option. Note that monsters that are dehacked
	// into being friendly with the MBF flag automatically gain MF3_NOBLOCKMONST, so this
	// just optionally generalizes the behavior to other friendly monsters.

	if (!((actor->flags3 & MF3_NOBLOCKMONST)
		|| ((i_compatflags & COMPATF_NOBLOCKFRIENDS) && (actor->flags & MF_FRIENDLY))))
	{
		// the regular 'blockmonsters' flag.
		if (line->flags & ML_BLOCKMONSTERS) return true;
		// flag for walking monsters
		if ((line->flags2 & ML2_BLOCKLANDMONSTERS) && !(actor->flags & MF_FLOAT)) return true;
	}

	// Blocking players
	if (((actor->player != nullptr) || (actor->flags8 & MF8_BLOCKASPLAYER)) && (line->flags & ML_BLOCK_PLAYERS)) return true;

	// Blocking floaters.
	if ((actor->flags & MF_FLOAT) && (line->flags & ML_BLOCK_FLOATERS)) return true;

	return false;
}