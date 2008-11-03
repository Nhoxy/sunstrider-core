/*
 * Copyright (C) 2005-2008 MaNGOS <http://www.mangosproject.org/>
 *
 * Copyright (C) 2008 Trinity <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef _SPELLMGR_H
#define _SPELLMGR_H

// For static or at-server-startup loaded spell data
// For more high level function for sSpellStore data

#include "SharedDefines.h"
#include "Database/DBCStructure.h"
#include "Database/SQLStorage.h"

#include "Utilities/HashMap.h"
#include <map>

class Player;
class Spell;

extern SQLStorage sSpellThreatStore;

enum SpellFailedReason
{
    SPELL_FAILED_AFFECTING_COMBAT               = 0x00,
    SPELL_FAILED_ALREADY_AT_FULL_HEALTH         = 0x01,
    SPELL_FAILED_ALREADY_AT_FULL_MANA           = 0x02,
    SPELL_FAILED_ALREADY_AT_FULL_POWER          = 0x03,
    SPELL_FAILED_ALREADY_BEING_TAMED            = 0x04,
    SPELL_FAILED_ALREADY_HAVE_CHARM             = 0x05,
    SPELL_FAILED_ALREADY_HAVE_SUMMON            = 0x06,
    SPELL_FAILED_ALREADY_OPEN                   = 0x07,
    SPELL_FAILED_AURA_BOUNCED                   = 0x08,
    SPELL_FAILED_AUTOTRACK_INTERRUPTED          = 0x09,
    SPELL_FAILED_BAD_IMPLICIT_TARGETS           = 0x0A,
    SPELL_FAILED_BAD_TARGETS                    = 0x0B,
    SPELL_FAILED_CANT_BE_CHARMED                = 0x0C,
    SPELL_FAILED_CANT_BE_DISENCHANTED           = 0x0D,
    SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL     = 0x0E,
    SPELL_FAILED_CANT_BE_PROSPECTED             = 0x0F,
    SPELL_FAILED_CANT_CAST_ON_TAPPED            = 0x10,
    SPELL_FAILED_CANT_DUEL_WHILE_INVISIBLE      = 0x11,
    SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED      = 0x12,
    SPELL_FAILED_CANT_STEALTH                   = 0x13,
    SPELL_FAILED_CASTER_AURASTATE               = 0x14,
    SPELL_FAILED_CASTER_DEAD                    = 0x15,
    SPELL_FAILED_CHARMED                        = 0x16,
    SPELL_FAILED_CHEST_IN_USE                   = 0x17,
    SPELL_FAILED_CONFUSED                       = 0x18,
    SPELL_FAILED_DONT_REPORT                    = 0x19,
    SPELL_FAILED_EQUIPPED_ITEM                  = 0x1A,
    SPELL_FAILED_EQUIPPED_ITEM_CLASS            = 0x1B,
    SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND   = 0x1C,
    SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND    = 0x1D,
    SPELL_FAILED_ERROR                          = 0x1E,
    SPELL_FAILED_FIZZLE                         = 0x1F,
    SPELL_FAILED_FLEEING                        = 0x20,
    SPELL_FAILED_FOOD_LOWLEVEL                  = 0x21,
    SPELL_FAILED_HIGHLEVEL                      = 0x22,
    SPELL_FAILED_HUNGER_SATIATED                = 0x23,
    SPELL_FAILED_IMMUNE                         = 0x24,
    SPELL_FAILED_INTERRUPTED                    = 0x25,
    SPELL_FAILED_INTERRUPTED_COMBAT             = 0x26,
    SPELL_FAILED_ITEM_ALREADY_ENCHANTED         = 0x27,
    SPELL_FAILED_ITEM_GONE                      = 0x28,
    SPELL_FAILED_ITEM_NOT_FOUND                 = 0x29,
    SPELL_FAILED_ITEM_NOT_READY                 = 0x2A,
    SPELL_FAILED_LEVEL_REQUIREMENT              = 0x2B,
    SPELL_FAILED_LINE_OF_SIGHT                  = 0x2C,
    SPELL_FAILED_LOWLEVEL                       = 0x2D,
    SPELL_FAILED_LOW_CASTLEVEL                  = 0x2E,
    SPELL_FAILED_MAINHAND_EMPTY                 = 0x2F,
    SPELL_FAILED_MOVING                         = 0x30,
    SPELL_FAILED_NEED_AMMO                      = 0x31,
    SPELL_FAILED_NEED_AMMO_POUCH                = 0x32,
    SPELL_FAILED_NEED_EXOTIC_AMMO               = 0x33,
    SPELL_FAILED_NOPATH                         = 0x34,
    SPELL_FAILED_NOT_BEHIND                     = 0x35,
    SPELL_FAILED_NOT_FISHABLE                   = 0x36,
    SPELL_FAILED_NOT_FLYING                     = 0x37,
    SPELL_FAILED_NOT_HERE                       = 0x38,
    SPELL_FAILED_NOT_INFRONT                    = 0x39,
    SPELL_FAILED_NOT_IN_CONTROL                 = 0x3A,
    SPELL_FAILED_NOT_KNOWN                      = 0x3B,
    SPELL_FAILED_NOT_MOUNTED                    = 0x3C,
    SPELL_FAILED_NOT_ON_TAXI                    = 0x3D,
    SPELL_FAILED_NOT_ON_TRANSPORT               = 0x3E,
    SPELL_FAILED_NOT_READY                      = 0x3F,
    SPELL_FAILED_NOT_SHAPESHIFT                 = 0x40,
    SPELL_FAILED_NOT_STANDING                   = 0x41,
    SPELL_FAILED_NOT_TRADEABLE                  = 0x42,
    SPELL_FAILED_NOT_TRADING                    = 0x43,
    SPELL_FAILED_NOT_UNSHEATHED                 = 0x44,
    SPELL_FAILED_NOT_WHILE_GHOST                = 0x45,
    SPELL_FAILED_NO_AMMO                        = 0x46,
    SPELL_FAILED_NO_CHARGES_REMAIN              = 0x47,
    SPELL_FAILED_NO_CHAMPION                    = 0x48,
    SPELL_FAILED_NO_COMBO_POINTS                = 0x49,
    SPELL_FAILED_NO_DUELING                     = 0x4A,
    SPELL_FAILED_NO_ENDURANCE                   = 0x4B,
    SPELL_FAILED_NO_FISH                        = 0x4C,
    SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED    = 0x4D,
    SPELL_FAILED_NO_MOUNTS_ALLOWED              = 0x4E,
    SPELL_FAILED_NO_PET                         = 0x4F,
    SPELL_FAILED_NO_POWER                       = 0x50,
    SPELL_FAILED_NOTHING_TO_DISPEL              = 0x51,
    SPELL_FAILED_NOTHING_TO_STEAL               = 0x52,
    SPELL_FAILED_ONLY_ABOVEWATER                = 0x53,
    SPELL_FAILED_ONLY_DAYTIME                   = 0x54,
    SPELL_FAILED_ONLY_INDOORS                   = 0x55,
    SPELL_FAILED_ONLY_MOUNTED                   = 0x56,
    SPELL_FAILED_ONLY_NIGHTTIME                 = 0x57,
    SPELL_FAILED_ONLY_OUTDOORS                  = 0x58,
    SPELL_FAILED_ONLY_SHAPESHIFT                = 0x59,
    SPELL_FAILED_ONLY_STEALTHED                 = 0x5A,
    SPELL_FAILED_ONLY_UNDERWATER                = 0x5B,
    SPELL_FAILED_OUT_OF_RANGE                   = 0x5C,
    SPELL_FAILED_PACIFIED                       = 0x5D,
    SPELL_FAILED_POSSESSED                      = 0x5E,
    SPELL_FAILED_REAGENTS                       = 0x5F,
    SPELL_FAILED_REQUIRES_AREA                  = 0x60,
    SPELL_FAILED_REQUIRES_SPELL_FOCUS           = 0x61,
    SPELL_FAILED_ROOTED                         = 0x62,
    SPELL_FAILED_SILENCED                       = 0x63,
    SPELL_FAILED_SPELL_IN_PROGRESS              = 0x64,
    SPELL_FAILED_SPELL_LEARNED                  = 0x65,
    SPELL_FAILED_SPELL_UNAVAILABLE              = 0x66,
    SPELL_FAILED_STUNNED                        = 0x67,
    SPELL_FAILED_TARGETS_DEAD                   = 0x68,
    SPELL_FAILED_TARGET_AFFECTING_COMBAT        = 0x69,
    SPELL_FAILED_TARGET_AURASTATE               = 0x6A,
    SPELL_FAILED_TARGET_DUELING                 = 0x6B,
    SPELL_FAILED_TARGET_ENEMY                   = 0x6C,
    SPELL_FAILED_TARGET_ENRAGED                 = 0x6D,
    SPELL_FAILED_TARGET_FRIENDLY                = 0x6E,
    SPELL_FAILED_TARGET_IN_COMBAT               = 0x6F,
    SPELL_FAILED_TARGET_IS_PLAYER               = 0x70,
    SPELL_FAILED_TARGET_IS_PLAYER_CONTROLLED    = 0x71,
    SPELL_FAILED_TARGET_NOT_DEAD                = 0x72,
    SPELL_FAILED_TARGET_NOT_IN_PARTY            = 0x73,
    SPELL_FAILED_TARGET_NOT_LOOTED              = 0x74,
    SPELL_FAILED_TARGET_NOT_PLAYER              = 0x75,
    SPELL_FAILED_TARGET_NO_POCKETS              = 0x76,
    SPELL_FAILED_TARGET_NO_WEAPONS              = 0x77,
    SPELL_FAILED_TARGET_UNSKINNABLE             = 0x78,
    SPELL_FAILED_THIRST_SATIATED                = 0x79,
    SPELL_FAILED_TOO_CLOSE                      = 0x7A,
    SPELL_FAILED_TOO_MANY_OF_ITEM               = 0x7B,
    SPELL_FAILED_TOTEM_CATEGORY                 = 0x7C,
    SPELL_FAILED_TOTEMS                         = 0x7D,
    SPELL_FAILED_TRAINING_POINTS                = 0x7E,
    SPELL_FAILED_TRY_AGAIN                      = 0x7F,
    SPELL_FAILED_UNIT_NOT_BEHIND                = 0x80,
    SPELL_FAILED_UNIT_NOT_INFRONT               = 0x81,
    SPELL_FAILED_WRONG_PET_FOOD                 = 0x82,
    SPELL_FAILED_NOT_WHILE_FATIGUED             = 0x83,
    SPELL_FAILED_TARGET_NOT_IN_INSTANCE         = 0x84,
    SPELL_FAILED_NOT_WHILE_TRADING              = 0x85,
    SPELL_FAILED_TARGET_NOT_IN_RAID             = 0x86,
    SPELL_FAILED_DISENCHANT_WHILE_LOOTING       = 0x87,
    SPELL_FAILED_PROSPECT_WHILE_LOOTING         = 0x88,
    SPELL_FAILED_PROSPECT_NEED_MORE             = 0x89,
    SPELL_FAILED_TARGET_FREEFORALL              = 0x8A,
    SPELL_FAILED_NO_EDIBLE_CORPSES              = 0x8B,
    SPELL_FAILED_ONLY_BATTLEGROUNDS             = 0x8C,
    SPELL_FAILED_TARGET_NOT_GHOST               = 0x8D,
    SPELL_FAILED_TOO_MANY_SKILLS                = 0x8E,
    SPELL_FAILED_TRANSFORM_UNUSABLE             = 0x8F,
    SPELL_FAILED_WRONG_WEATHER                  = 0x90,
    SPELL_FAILED_DAMAGE_IMMUNE                  = 0x91,
    SPELL_FAILED_PREVENTED_BY_MECHANIC          = 0x92,
    SPELL_FAILED_PLAY_TIME                      = 0x93,
    SPELL_FAILED_REPUTATION                     = 0x94,
    SPELL_FAILED_MIN_SKILL                      = 0x95,
    SPELL_FAILED_NOT_IN_ARENA                   = 0x96,
    SPELL_FAILED_NOT_ON_SHAPESHIFT              = 0x97,
    SPELL_FAILED_NOT_ON_STEALTHED               = 0x98,
    SPELL_FAILED_NOT_ON_DAMAGE_IMMUNE           = 0x99,
    SPELL_FAILED_NOT_ON_MOUNTED                 = 0x9A,
    SPELL_FAILED_TOO_SHALLOW                    = 0x9B,
    SPELL_FAILED_TARGET_NOT_IN_SANCTUARY        = 0x9C,
    SPELL_FAILED_TARGET_IS_TRIVIAL              = 0x9D,
    SPELL_FAILED_BM_OR_INVISGOD                 = 0x9E,
    SPELL_FAILED_EXPERT_RIDING_REQUIREMENT      = 0x9F,
    SPELL_FAILED_ARTISAN_RIDING_REQUIREMENT     = 0xA0,
    SPELL_FAILED_NOT_IDLE                       = 0xA1,
    SPELL_FAILED_NOT_INACTIVE                   = 0xA2,
    SPELL_FAILED_PARTIAL_PLAYTIME               = 0xA3,
    SPELL_FAILED_NO_PLAYTIME                    = 0xA4,
    SPELL_FAILED_NOT_IN_BATTLEGROUND            = 0xA5,
    SPELL_FAILED_ONLY_IN_ARENA                  = 0xA6,
    SPELL_FAILED_TARGET_LOCKED_TO_RAID_INSTANCE = 0xA7,
    SPELL_FAILED_UNKNOWN                        = 0xA8,
};

enum SpellFamilyNames
{
    SPELLFAMILY_GENERIC     = 0,
    SPELLFAMILY_UNK1        = 1,                            // events, holidays
    // 2 - unused
    SPELLFAMILY_MAGE        = 3,
    SPELLFAMILY_WARRIOR     = 4,
    SPELLFAMILY_WARLOCK     = 5,
    SPELLFAMILY_PRIEST      = 6,
    SPELLFAMILY_DRUID       = 7,
    SPELLFAMILY_ROGUE       = 8,
    SPELLFAMILY_HUNTER      = 9,
    SPELLFAMILY_PALADIN     = 10,
    SPELLFAMILY_SHAMAN      = 11,
    SPELLFAMILY_UNK2        = 12,
    SPELLFAMILY_POTION      = 13
};

enum SpellDisableTypes
{
    SPELL_DISABLE_PLAYER = 1,
    SPELL_DISABLE_CREATURE = 2
};

//Some SpellFamilyFlags
#define SPELLFAMILYFLAG_ROGUE_VANISH            0x000000800LL
#define SPELLFAMILYFLAG_ROGUE_STEALTH           0x000400000LL
#define SPELLFAMILYFLAG_ROGUE_BACKSTAB          0x000800004LL
#define SPELLFAMILYFLAG_ROGUE_SAP               0x000000080LL
#define SPELLFAMILYFLAG_ROGUE_FEINT             0x008000000LL
#define SPELLFAMILYFLAG_ROGUE_KIDNEYSHOT        0x000200000LL
#define SPELLFAMILYFLAG_ROGUE__FINISHING_MOVE   0x9003E0000LL

// Spell clasification
enum SpellSpecific
{
    SPELL_NORMAL            = 0,
    SPELL_SEAL              = 1,
    SPELL_BLESSING          = 2,
    SPELL_AURA              = 3,
    SPELL_STING             = 4,
    SPELL_CURSE             = 5,
    SPELL_ASPECT            = 6,
    SPELL_TRACKER           = 7,
    SPELL_WARLOCK_ARMOR     = 8,
    SPELL_MAGE_ARMOR        = 9,
    SPELL_ELEMENTAL_SHIELD  = 10,
    SPELL_MAGE_POLYMORPH    = 11,
    SPELL_POSITIVE_SHOUT    = 12,
    SPELL_JUDGEMENT         = 13,
    SPELL_BATTLE_ELIXIR     = 14,
    SPELL_GUARDIAN_ELIXIR   = 15,
    SPELL_FLASK_ELIXIR      = 16
};

SpellSpecific GetSpellSpecific(uint32 spellId);

// Different spell properties
inline float GetSpellRadius(SpellRadiusEntry const *radius) { return (radius ? radius->Radius : 0); }
uint32 GetSpellCastTime(SpellEntry const* spellInfo, Spell const* spell = NULL);
inline float GetSpellMinRange(SpellRangeEntry const *range) { return (range ? range->minRange : 0); }
inline float GetSpellMaxRange(SpellRangeEntry const *range) { return (range ? range->maxRange : 0); }
inline uint32 GetSpellRecoveryTime(SpellEntry const *spellInfo) { return spellInfo->RecoveryTime > spellInfo->CategoryRecoveryTime ? spellInfo->RecoveryTime : spellInfo->CategoryRecoveryTime; }
int32 GetSpellDuration(SpellEntry const *spellInfo);
int32 GetSpellMaxDuration(SpellEntry const *spellInfo);

inline bool IsSpellHaveEffect(SpellEntry const *spellInfo, SpellEffects effect)
{
    for(int i= 0; i < 3; ++i)
        if(spellInfo->Effect[i]==effect)
            return true;
    return false;
}

bool IsNoStackAuraDueToAura(uint32 spellId_1, uint32 effIndex_1, uint32 spellId_2, uint32 effIndex_2);

inline bool IsSealSpell(SpellEntry const *spellInfo)
{
    //Collection of all the seal family flags. No other paladin spell has any of those.
    return spellInfo->SpellFamilyName == SPELLFAMILY_PALADIN &&
        ( spellInfo->SpellFamilyFlags & 0x4000A000200LL );
}

inline bool IsElementalShield(SpellEntry const *spellInfo)
{
    // family flags 10 (Lightning), 42 (Earth), 37 (Water), proc shield from T2 8 pieces bonus
    return (spellInfo->SpellFamilyFlags & 0x42000000400LL) || spellInfo->Id == 23552;
}

int32 CompareAuraRanks(uint32 spellId_1, uint32 effIndex_1, uint32 spellId_2, uint32 effIndex_2);
bool IsSingleFromSpellSpecificPerCaster(uint32 spellSpec1,uint32 spellSpec2);
bool IsPassiveSpell(uint32 spellId);

inline bool IsDeathPersistentSpell(SpellEntry const *spellInfo)
{
    switch(spellInfo->Id)
    {
        case 40214:                                     // Dragonmaw Illusion
        case 35480: case 35481: case 35482:             // Human Illusion
        case 35483: case 39824:                         // Human Illusion
            return true;
    }

    return spellInfo->AttributesEx3 & SPELL_ATTR_EX3_DEATH_PERSISTENT;
}

inline bool IsNonCombatSpell(SpellEntry const *spellInfo)
{
    return (spellInfo->Attributes & SPELL_ATTR_CANT_USED_IN_COMBAT) != 0;
}

bool IsPositiveSpell(uint32 spellId);
bool IsPositiveEffect(uint32 spellId, uint32 effIndex);
bool IsPositiveTarget(uint32 targetA, uint32 targetB);

bool IsSingleTargetSpell(SpellEntry const *spellInfo);
bool IsSingleTargetSpells(SpellEntry const *spellInfo1, SpellEntry const *spellInfo2);

bool IsSpellAllowedInLocation(SpellEntry const *spellInfo,uint32 map_id,uint32 zone_id,uint32 area_id);

inline bool IsAreaEffectTarget( Targets target )
{
    switch (target )
    {
        case TARGET_AREAEFFECT_CUSTOM:
        case TARGET_ALL_ENEMY_IN_AREA:
        case TARGET_ALL_ENEMY_IN_AREA_INSTANT:
        case TARGET_ALL_PARTY_AROUND_CASTER:
        case TARGET_ALL_AROUND_CASTER:
        case TARGET_ALL_ENEMY_IN_AREA_CHANNELED:
        case TARGET_ALL_FRIENDLY_UNITS_AROUND_CASTER:
        case TARGET_ALL_PARTY:
        case TARGET_ALL_PARTY_AROUND_CASTER_2:
        case TARGET_AREAEFFECT_PARTY:
        case TARGET_AREAEFFECT_CUSTOM_2:
        case TARGET_AREAEFFECT_PARTY_AND_CLASS:
        case TARGET_IN_FRONT_OF_CASTER:
        case TARGET_ALL_FRIENDLY_UNITS_IN_AREA:
            return true;
        default:
            break;
    }
    return false;
}

inline bool IsAreaOfEffectSpell(SpellEntry const *spellInfo)
{
    if(IsAreaEffectTarget(Targets(spellInfo->EffectImplicitTargetA[0])) || IsAreaEffectTarget(Targets(spellInfo->EffectImplicitTargetB[0])))
        return true;
    if(IsAreaEffectTarget(Targets(spellInfo->EffectImplicitTargetA[1])) || IsAreaEffectTarget(Targets(spellInfo->EffectImplicitTargetB[1])))
        return true;
    if(IsAreaEffectTarget(Targets(spellInfo->EffectImplicitTargetA[2])) || IsAreaEffectTarget(Targets(spellInfo->EffectImplicitTargetB[2])))
        return true;
    return false;
}

inline bool IsAreaAuraEffect(uint32 effect)
{
    if( effect == SPELL_EFFECT_APPLY_AREA_AURA_PARTY    ||
        effect == SPELL_EFFECT_APPLY_AREA_AURA_FRIEND   ||
        effect == SPELL_EFFECT_APPLY_AREA_AURA_ENEMY    ||
        effect == SPELL_EFFECT_APPLY_AREA_AURA_PET      ||
        effect == SPELL_EFFECT_APPLY_AREA_AURA_OWNER)
        return true;
    return false;
}

inline bool IsDispelSpell(SpellEntry const *spellInfo)
{
    if (spellInfo->Effect[0] == SPELL_EFFECT_DISPEL ||
        spellInfo->Effect[1] == SPELL_EFFECT_DISPEL ||
        spellInfo->Effect[2] == SPELL_EFFECT_DISPEL )
        return true;
    return false;
}
inline bool isSpellBreakStealth(SpellEntry const* spellInfo)
{
    return !(spellInfo->AttributesEx & SPELL_ATTR_EX_NOT_BREAK_STEALTH);
}

uint8 GetErrorAtShapeshiftedCast (SpellEntry const *spellInfo, uint32 form);

inline bool IsChanneledSpell(SpellEntry const* spellInfo)
{
    return (spellInfo->AttributesEx & (SPELL_ATTR_EX_CHANNELED_1 | SPELL_ATTR_EX_CHANNELED_2));
}

inline bool NeedsComboPoints(SpellEntry const* spellInfo)
{
    return (spellInfo->AttributesEx & (SPELL_ATTR_EX_REQ_COMBO_POINTS1 | SPELL_ATTR_EX_REQ_COMBO_POINTS2));
}

inline SpellSchoolMask GetSpellSchoolMask(SpellEntry const* spellInfo)
{
    return SpellSchoolMask(spellInfo->SchoolMask);
}

inline uint32 GetSpellMechanicMask(SpellEntry const* spellInfo, int32 effect)
{
    uint32 mask = 0;
    if (spellInfo->Mechanic)
        mask |= 1<<spellInfo->Mechanic;
    if (spellInfo->EffectMechanic[effect])
        mask |= 1<<spellInfo->EffectMechanic[effect];
    return mask;
}

inline Mechanics GetEffectMechanic(SpellEntry const* spellInfo, int32 effect)
{
    if (spellInfo->EffectMechanic[effect])
        return Mechanics(spellInfo->EffectMechanic[effect]);
    if (spellInfo->Mechanic)
        return Mechanics(spellInfo->Mechanic);
    return MECHANIC_NONE;
}

inline uint32 GetDispellMask(DispelType dispel)
{
    // If dispell all
    if (dispel == DISPEL_ALL)
        return DISPEL_ALL_MASK;
    else
        return (1 << dispel);
}

// Diminishing Returns interaction with spells
DiminishingGroup GetDiminishingReturnsGroupForSpell(SpellEntry const* spellproto, bool triggered);
bool IsDiminishingReturnsGroupDurationLimited(DiminishingGroup group);
DiminishingReturnsType GetDiminishingReturnsGroupType(DiminishingGroup group);

// Spell affects related declarations (accessed using SpellMgr functions)
typedef std::map<uint32, uint64> SpellAffectMap;

// Spell proc event related declarations (accessed using SpellMgr functions)
enum ProcFlags
{
    PROC_FLAG_NONE                  = 0x00000000,           // None
    PROC_FLAG_HIT_MELEE             = 0x00000001,           // On melee hit
    PROC_FLAG_STRUCK_MELEE          = 0x00000002,           // On being struck melee
    PROC_FLAG_KILL_XP_GIVER         = 0x00000004,           // On kill target giving XP or honor
    PROC_FLAG_SPECIAL_DROP          = 0x00000008,           //
    PROC_FLAG_DODGE                 = 0x00000010,           // On dodge melee attack
    PROC_FLAG_PARRY                 = 0x00000020,           // On parry melee attack
    PROC_FLAG_BLOCK                 = 0x00000040,           // On block attack
    PROC_FLAG_TOUCH                 = 0x00000080,           // On being touched (for bombs, probably?)
    PROC_FLAG_TARGET_LOW_HEALTH     = 0x00000100,           // On deal damage to enemy with 20% or less health
    PROC_FLAG_LOW_HEALTH            = 0x00000200,           // On health dropped below 20%
    PROC_FLAG_STRUCK_RANGED         = 0x00000400,           // On being struck ranged
    PROC_FLAG_HIT_SPECIAL           = 0x00000800,           // (!)Removed, may be reassigned in future
    PROC_FLAG_CRIT_MELEE            = 0x00001000,           // On crit melee
    PROC_FLAG_STRUCK_CRIT_MELEE     = 0x00002000,           // On being critically struck in melee
    PROC_FLAG_CAST_SPELL            = 0x00004000,           // On cast spell
    PROC_FLAG_TAKE_DAMAGE           = 0x00008000,           // On take damage
    PROC_FLAG_CRIT_SPELL            = 0x00010000,           // On crit spell
    PROC_FLAG_HIT_SPELL             = 0x00020000,           // On hit spell
    PROC_FLAG_STRUCK_CRIT_SPELL     = 0x00040000,           // On being critically struck by a spell
    PROC_FLAG_HIT_RANGED            = 0x00080000,           // On getting ranged hit
    PROC_FLAG_STRUCK_SPELL          = 0x00100000,           // On being struck by a spell
    PROC_FLAG_TRAP                  = 0x00200000,           // On trap activation (?)
    PROC_FLAG_CRIT_RANGED           = 0x00400000,           // On getting ranged crit
    PROC_FLAG_STRUCK_CRIT_RANGED    = 0x00800000,           // On being critically struck by a ranged attack
    PROC_FLAG_RESIST_SPELL          = 0x01000000,           // On resist enemy spell
    PROC_FLAG_TARGET_RESISTS        = 0x02000000,           // On enemy resisted spell
    PROC_FLAG_TARGET_DODGE_OR_PARRY = 0x04000000,           // On enemy dodges/parries
    PROC_FLAG_HEAL                  = 0x08000000,           // On heal
    PROC_FLAG_CRIT_HEAL             = 0x10000000,           // On critical healing effect
    PROC_FLAG_HEALED                = 0x20000000,           // On healing
    PROC_FLAG_TARGET_BLOCK          = 0x40000000,           // On enemy blocks
    PROC_FLAG_MISS                  = 0x80000000            // On miss melee attack
};

struct SpellProcEventEntry
{
    uint32      schoolMask;                                 // if nonzero - bit mask for matching proc condition based on spell candidate's school: Fire=2, Mask=1<<(2-1)=2
    uint32      category;                                   // if nonzero - match proc condition based on candidate spell's category
    uint32      skillId;                                    // if nonzero - for matching proc condition based on candidate spell's skillId from SkillLineAbility.dbc (Shadow Bolt = Destruction)
    uint32      spellFamilyName;                            // if nonzero - for matching proc condition based on candidate spell's SpellFamilyNamer value
    uint64      spellFamilyMask;                            // if nonzero - for matching proc condition based on candidate spell's SpellFamilyFlags (like auras 107 and 108 do)
    uint32      procFlags;                                  // bitmask for matching proc event
    float       ppmRate;                                    // for melee (ranged?) damage spells - proc rate per minute. if zero, falls back to flat chance from Spell.dbc
    uint32      cooldown;                                   // hidden cooldown used for some spell proc events, applied to _triggered_spell_
};

typedef HM_NAMESPACE::hash_map<uint32, SpellProcEventEntry> SpellProcEventMap;

#define ELIXIR_BATTLE_MASK    0x1
#define ELIXIR_GUARDIAN_MASK  0x2
#define ELIXIR_FLASK_MASK     (ELIXIR_BATTLE_MASK|ELIXIR_GUARDIAN_MASK)
#define ELIXIR_UNSTABLE_MASK  0x4
#define ELIXIR_SHATTRATH_MASK 0x8

typedef std::map<uint32, uint8> SpellElixirMap;

// Spell script target related declarations (accessed using SpellMgr functions)
enum SpellTargetType
{
    SPELL_TARGET_TYPE_GAMEOBJECT = 0,
    SPELL_TARGET_TYPE_CREATURE   = 1,
    SPELL_TARGET_TYPE_DEAD       = 2
};

#define MAX_SPELL_TARGET_TYPE 3

struct SpellTargetEntry
{
    SpellTargetEntry(SpellTargetType type_,uint32 targetEntry_) : type(type_), targetEntry(targetEntry_) {}
    SpellTargetType type;
    uint32 targetEntry;
};

typedef std::multimap<uint32,SpellTargetEntry> SpellScriptTarget;

// coordinates for spells (accessed using SpellMgr functions)
struct SpellTargetPosition
{
    uint32 target_mapId;
    float  target_X;
    float  target_Y;
    float  target_Z;
    float  target_Orientation;
};

typedef HM_NAMESPACE::hash_map<uint32, SpellTargetPosition> SpellTargetPositionMap;

// Spell pet auras
class PetAura
{
    public:
        PetAura()
        {
            auras.clear();
        }

        PetAura(uint16 petEntry, uint16 aura, bool _removeOnChangePet, int _damage) :
        removeOnChangePet(_removeOnChangePet), damage(_damage)
        {
            auras[petEntry] = aura;
        }

        uint16 GetAura(uint16 petEntry) const
        {
            std::map<uint16, uint16>::const_iterator itr = auras.find(petEntry);
            if(itr != auras.end())
                return itr->second;
            else
            {
                std::map<uint16, uint16>::const_iterator itr = auras.find(0);
                if(itr != auras.end())
                    return itr->second;
                else
                    return 0;
            }
        }

        void AddAura(uint16 petEntry, uint16 aura)
        {
            auras[petEntry] = aura;
        }

        bool IsRemovedOnChangePet() const
        {
            return removeOnChangePet;
        }

        int32 GetDamage() const
        {
            return damage;
        }

    private:
        std::map<uint16, uint16> auras;
        bool removeOnChangePet;
        int32 damage;
};
typedef std::map<uint16, PetAura> SpellPetAuraMap;

// Spell rank chain  (accessed using SpellMgr functions)
struct SpellChainNode
{
    uint32 prev;
    uint32 first;
    uint32 req;
    uint8  rank;
};

typedef HM_NAMESPACE::hash_map<uint32, SpellChainNode> SpellChainMap;
typedef std::multimap<uint32, uint32> SpellChainMapNext;

// Spell learning properties (accessed using SpellMgr functions)
struct SpellLearnSkillNode
{
    uint32 skill;
    uint32 value;                                           // 0  - max skill value for player level
    uint32 maxvalue;                                        // 0  - max skill value for player level
};

typedef std::map<uint32, SpellLearnSkillNode> SpellLearnSkillMap;

struct SpellLearnSpellNode
{
    uint32 spell;
    bool autoLearned;
};

typedef std::multimap<uint32, SpellLearnSpellNode> SpellLearnSpellMap;

typedef std::multimap<uint32, SkillLineAbilityEntry const*> SkillLineAbilityMap;

inline bool IsPrimaryProfessionSkill(uint32 skill)
{
    SkillLineEntry const *pSkill = sSkillLineStore.LookupEntry(skill);
    if(!pSkill)
        return false;

    if(pSkill->categoryId != SKILL_CATEGORY_PROFESSION)
        return false;

    return true;
}

inline bool IsProfessionSkill(uint32 skill)
{
    return  IsPrimaryProfessionSkill(skill) || skill == SKILL_FISHING || skill == SKILL_COOKING || skill == SKILL_FIRST_AID;
}

enum SpellExtraAttributeType
{
    SPELL_EXTRA_ATTR_MAX_TARGETS,
    SPELL_EXTRA_ATTR_CONE_TYPE,
    SPELL_EXTRA_ATTR_SHARE_DAMAGE
};

struct SpellExtraAttribute
{
    uint32 attr[3];
};

typedef std::map<uint32, SpellExtraAttribute> SpellExtraAttrMap;

typedef std::map<int32, std::vector<int32> > SpellLinkedMap;

class SpellMgr
{
    // Constructors
    public:
        SpellMgr();
        ~SpellMgr();

        // Accessors (const or static functions)
    public:
        // Spell affects
        uint64 GetSpellAffectMask(uint16 spellId, uint8 effectId) const
        {
            SpellAffectMap::const_iterator itr = mSpellAffectMap.find((spellId<<8) + effectId);
            if( itr != mSpellAffectMap.end( ) )
                return itr->second;
            return 0;
        }

        bool IsAffectedBySpell(SpellEntry const *spellInfo, uint32 spellId, uint8 effectId, uint64 familyFlags) const;

        SpellElixirMap const& GetSpellElixirMap() const { return mSpellElixirs; }

        uint32 GetSpellElixirMask(uint32 spellid) const
        {
            SpellElixirMap::const_iterator itr = mSpellElixirs.find(spellid);
            if(itr==mSpellElixirs.end())
                return 0x0;

            return itr->second;
        }

        SpellSpecific GetSpellElixirSpecific(uint32 spellid) const
        {
            uint32 mask = GetSpellElixirMask(spellid);
            if((mask & ELIXIR_FLASK_MASK)==ELIXIR_FLASK_MASK)
                return SPELL_FLASK_ELIXIR;
            else if(mask & ELIXIR_BATTLE_MASK)
                return SPELL_BATTLE_ELIXIR;
            else if(mask & ELIXIR_GUARDIAN_MASK)
                return SPELL_GUARDIAN_ELIXIR;
            else
                return SPELL_NORMAL;
        }

        // Spell proc events
        SpellProcEventEntry const* GetSpellProcEvent(uint32 spellId) const
        {
            SpellProcEventMap::const_iterator itr = mSpellProcEventMap.find(spellId);
            if( itr != mSpellProcEventMap.end( ) )
                return &itr->second;
            return NULL;
        }

        static bool IsSpellProcEventCanTriggeredBy( SpellProcEventEntry const * spellProcEvent, SpellEntry const * procSpell, uint32 procFlags );

        // Spell target coordinates
        SpellTargetPosition const* GetSpellTargetPosition(uint32 spell_id) const
        {
            SpellTargetPositionMap::const_iterator itr = mSpellTargetPositions.find( spell_id );
            if( itr != mSpellTargetPositions.end( ) )
                return &itr->second;
            return NULL;
        }

        // Spell ranks chains
        SpellChainNode const* GetSpellChainNode(uint32 spell_id) const
        {
            SpellChainMap::const_iterator itr = mSpellChains.find(spell_id);
            if(itr == mSpellChains.end())
                return NULL;

            return &itr->second;
        }

        uint32 GetFirstSpellInChain(uint32 spell_id) const
        {
            if(SpellChainNode const* node = GetSpellChainNode(spell_id))
                return node->first;

            return spell_id;
        }

        uint32 GetPrevSpellInChain(uint32 spell_id) const
        {
            if(SpellChainNode const* node = GetSpellChainNode(spell_id))
                return node->prev;

            return 0;
        }

        SpellChainMapNext const& GetSpellChainNext() const { return mSpellChainsNext; }

        // Note: not use rank for compare to spell ranks: spell chains isn't linear order
        // Use IsHighRankOfSpell instead
        uint8 GetSpellRank(uint32 spell_id) const
        {
            if(SpellChainNode const* node = GetSpellChainNode(spell_id))
                return node->rank;

            return 0;
        }

        uint8 IsHighRankOfSpell(uint32 spell1,uint32 spell2) const
        {
            SpellChainMap::const_iterator itr = mSpellChains.find(spell1);

            uint32 rank2 = GetSpellRank(spell2);

            // not ordered correctly by rank value
            if(itr == mSpellChains.end() || !rank2 || itr->second.rank <= rank2)
                return false;

            // check present in same rank chain
            for(; itr != mSpellChains.end(); itr = mSpellChains.find(itr->second.prev))
                if(itr->second.prev==spell2)
                    return true;

            return false;
        }

        bool IsRankSpellDueToSpell(SpellEntry const *spellInfo_1,uint32 spellId_2) const;
        static bool canStackSpellRanks(SpellEntry const *spellInfo);
        bool IsNoStackSpellDueToSpell(uint32 spellId_1, uint32 spellId_2) const;

        SpellEntry const* SelectAuraRankForPlayerLevel(SpellEntry const* spellInfo, uint32 playerLevel) const;

        // Spell learning
        SpellLearnSkillNode const* GetSpellLearnSkill(uint32 spell_id) const
        {
            SpellLearnSkillMap::const_iterator itr = mSpellLearnSkills.find(spell_id);
            if(itr != mSpellLearnSkills.end())
                return &itr->second;
            else
                return NULL;
        }

        bool IsSpellLearnSpell(uint32 spell_id) const
        {
            return mSpellLearnSpells.count(spell_id)!=0;
        }

        SpellLearnSpellMap::const_iterator GetBeginSpellLearnSpell(uint32 spell_id) const
        {
            return mSpellLearnSpells.lower_bound(spell_id);
        }

        SpellLearnSpellMap::const_iterator GetEndSpellLearnSpell(uint32 spell_id) const
        {
            return mSpellLearnSpells.upper_bound(spell_id);
        }

        bool IsSpellLearnToSpell(uint32 spell_id1,uint32 spell_id2) const
        {
            SpellLearnSpellMap::const_iterator b = GetBeginSpellLearnSpell(spell_id1);
            SpellLearnSpellMap::const_iterator e = GetEndSpellLearnSpell(spell_id1);
            for(SpellLearnSpellMap::const_iterator i = b; i != e; ++i)
                if(i->second.spell==spell_id2)
                    return true;
            return false;
        }

        static bool IsProfessionSpell(uint32 spellId);
        static bool IsPrimaryProfessionSpell(uint32 spellId);
        bool IsPrimaryProfessionFirstRankSpell(uint32 spellId) const;

        // Spell script targets
        SpellScriptTarget::const_iterator GetBeginSpellScriptTarget(uint32 spell_id) const
        {
            return mSpellScriptTarget.lower_bound(spell_id);
        }

        SpellScriptTarget::const_iterator GetEndSpellScriptTarget(uint32 spell_id) const
        {
            return mSpellScriptTarget.upper_bound(spell_id);
        }

        // Spell correctess for client using
        static bool IsSpellValid(SpellEntry const * spellInfo, Player* pl = NULL, bool msg = true);

        SkillLineAbilityMap::const_iterator GetBeginSkillLineAbilityMap(uint32 spell_id) const
        {
            return mSkillLineAbilityMap.lower_bound(spell_id);
        }

        SkillLineAbilityMap::const_iterator GetEndSkillLineAbilityMap(uint32 spell_id) const
        {
            return mSkillLineAbilityMap.upper_bound(spell_id);
        }

        PetAura const* GetPetAura(uint16 spell_id)
        {
            SpellPetAuraMap::const_iterator itr = mSpellPetAuraMap.find(spell_id);
            if(itr != mSpellPetAuraMap.end())
                return &itr->second;
            else
                return NULL;
        }

        uint32 GetSpellExtraAttr(uint32 spell_id, uint32 type) const
        {
            SpellExtraAttrMap::const_iterator itr = mSpellExtraAttrMap.find(spell_id);
            if(itr != mSpellExtraAttrMap.end())
                return itr->second.attr[type];
            else
                return 0;
        }

        const std::vector<int32> *GetSpellLinked(int32 spell_id) const
        {
            SpellLinkedMap::const_iterator itr = mSpellLinkedMap.find(spell_id);
            return itr != mSpellLinkedMap.end() ? &(itr->second) : NULL;
        }

        // Modifiers
    public:
        static SpellMgr& Instance();

        // Loading data at server startup
        void LoadSpellChains();
        void LoadSpellLearnSkills();
        void LoadSpellLearnSpells();
        void LoadSpellScriptTarget();
        void LoadSpellAffects();
        void LoadSpellElixirs();
        void LoadSpellProcEvents();
        void LoadSpellTargetPositions();
        void LoadSpellThreats();
        void LoadSkillLineAbilityMap();
        void LoadSpellPetAuras();
        void LoadSpellExtraAttr();
        void LoadSpellLinked();

    private:
        SpellScriptTarget  mSpellScriptTarget;
        SpellChainMap      mSpellChains;
        SpellChainMapNext  mSpellChainsNext;
        SpellLearnSkillMap mSpellLearnSkills;
        SpellLearnSpellMap mSpellLearnSpells;
        SpellTargetPositionMap mSpellTargetPositions;
        SpellAffectMap     mSpellAffectMap;
        SpellElixirMap     mSpellElixirs;
        SpellProcEventMap  mSpellProcEventMap;
        SkillLineAbilityMap mSkillLineAbilityMap;
        SpellPetAuraMap     mSpellPetAuraMap;
        SpellExtraAttrMap   mSpellExtraAttrMap;
        SpellLinkedMap      mSpellLinkedMap;
};

#define spellmgr SpellMgr::Instance()
#endif
