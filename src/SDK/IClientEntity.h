#pragma once

#include "vector.h"
extern uintptr_t* GetCSWpnData_address;

enum MoveType_t
{
    MOVETYPE_NONE = 0,
    MOVETYPE_ISOMETRIC,
    MOVETYPE_WALK,
    MOVETYPE_STEP,
    MOVETYPE_FLY,
    MOVETYPE_FLYGRAVITY,
    MOVETYPE_VPHYSICS,
    MOVETYPE_PUSH,
    MOVETYPE_NOCLIP,
    MOVETYPE_LADDER,
    MOVETYPE_OBSERVER,
    MOVETYPE_CUSTOM,
    MOVETYPE_LAST = MOVETYPE_CUSTOM, MOVETYPE_MAX_BITS = 4 };

enum DataUpdateType_t
{
    DATA_UPDATE_CREATED = 0,
    DATA_UPDATE_DATATABLE_CHANGED,
};

class ICollideable
{
public:
    virtual void pad0( );
    virtual const Vector& OBBMins( ) const;
    virtual const Vector& OBBMaxs( ) const;
};

class IHandleEntity
{
public:
    virtual ~IHandleEntity() {};
};

class IClientUnknown : public IHandleEntity {};
class IClientRenderable
{
public:
    virtual ~IClientRenderable() {};

    model_t* GetModel()
    {
        typedef model_t* (* oGetModel)(void*);
        return getvfunc<oGetModel>(this, 8)(this);
    }

    bool SetupBones(matrix3x4_t* pBoneMatrix, int nMaxBones, int nBoneMask, float flCurTime = 0)
    {
        typedef bool (* oSetupBones)(void*, matrix3x4_t*, int, int, float);
        return getvfunc<oSetupBones>(this, 13)(this, pBoneMatrix, nMaxBones, nBoneMask, flCurTime);
    }
};

class IClientNetworkable
{
public:
    virtual ~IClientNetworkable() {};

    void Release()
    {
        typedef void (* oRelease)(void*);
        return getvfunc<oRelease>(this, 1)(this);
    }

    ClientClass* GetClientClass()
    {
        typedef ClientClass* (* oGetClientClass)(void*);
        return getvfunc<oGetClientClass>(this, 2)(this);
    }

    void PreDataUpdate(DataUpdateType_t updateType)
    {
        typedef void (* oPreDataUpdate)(void*, DataUpdateType_t);
        return getvfunc<oPreDataUpdate>(this, 6)(this, updateType);
    }

    int GetIndex()
    {
        typedef int (* oGetIndex)(void*);
        return getvfunc<oGetIndex>(this, 10)(this);
    }

    void SetDestroyedOnRecreateEntities()
    {
        typedef void (* oSetDestroyedOnRecreateEntities)(void*);
        return getvfunc<oSetDestroyedOnRecreateEntities>(this, 13)(this);
    }
};

class IClientThinkable
{
public:
    virtual ~IClientThinkable() {};
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
    virtual ~IClientEntity() {};
};

class C_BaseEntity : public IClientEntity
{
public:

    IClientNetworkable* GetNetworkable()
    {
        return (IClientNetworkable*)((uintptr_t)this + 0x10);
    }

    void SetModelIndex(int index)
    {
        typedef void (* oSetModelIndex)(void*, int);
        return getvfunc<oSetModelIndex>(this, 111)(this, index);
    }

    int* GetModelIndex()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseViewModel.m_nModelIndex);
    }

    TeamID GetTeam()
    {
        return *(TeamID*)((uintptr_t)this + offsets.DT_BaseEntity.m_iTeamNum);
    }

    Vector GetVecOrigin()
    {
        return *(Vector*)((uintptr_t)this + offsets.DT_BaseEntity.m_vecOrigin);
    }

    MoveType_t GetMoveType()
    {
        return *(MoveType_t*)((uintptr_t)this + offsets.DT_BaseEntity.m_MoveType);
    }

    ICollideable* GetCollideable()
    {
        return (ICollideable*)((uintptr_t)this + offsets.DT_BaseEntity.m_Collision);
    }

    bool* GetSpotted()
    {
        return (bool*)((uintptr_t)this + offsets.DT_BaseEntity.m_bSpotted);
    }

    char* GetClassname()
    {
        typedef char* (* oGetClassname)(void*);
        return getvfunc<oGetClassname>(this, 195)(this);
    }

    bool IsDormant()
    {
        typedef bool (* oIsDormant)(void*);
        return getvfunc<oIsDormant>(this, 79)(this);
    }

};

/* generic game classes */
class C_BasePlayer : public C_BaseEntity
{
public:
    QAngle* GetViewPunchAngle()
    {
        return (QAngle*)((uintptr_t)this + offsets.DT_BasePlayer.m_viewPunchAngle);
    }

    QAngle* GetAimPunchAngle()
    {
        return (QAngle*)((uintptr_t)this + offsets.DT_BasePlayer.m_aimPunchAngle);
    }

    Vector GetVecViewOffset()
    {
        return *(Vector*)((uintptr_t)this + offsets.DT_BasePlayer.m_vecViewOffset);
    }

    unsigned int GetTickBase()
    {
        return *(unsigned int*)((uintptr_t)this + offsets.DT_BasePlayer.m_nTickBase);
    }

    Vector GetVelocity()
    {
        return *(Vector*)((uintptr_t)this + offsets.DT_BasePlayer.m_vecVelocity);
    }

    int GetHealth()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BasePlayer.m_iHealth);
    }

    unsigned char GetLifeState()
    {
        return *(unsigned char*)((uintptr_t)this + offsets.DT_BasePlayer.m_lifeState);
    }

    int GetFlags()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BasePlayer.m_fFlags);
    }

    ObserverMode_t* GetObserverMode()
    {
        return (ObserverMode_t*)((uintptr_t)this + offsets.DT_BasePlayer.m_iObserverMode);
    }

    void* GetObserverTarget()
    {
        return (void*)((uintptr_t)this + offsets.DT_BasePlayer.m_hObserverTarget);
    }

    void* GetViewModel()
    {
        return (void*)((uintptr_t)this + offsets.DT_BasePlayer.m_hViewModel);
    }

    const char* GetLastPlaceName()
    {
        return (const char*)((uintptr_t)this + offsets.DT_BasePlayer.m_szLastPlaceName);
    }

    QAngle* GetEyeAngles()
    {
        return (QAngle*)((uintptr_t)this + offsets.DT_INSPlayer.m_angEyeAngles[0]);
    }

    float GetFlashDuration()
    {
        return *(float*)((uintptr_t)this + offsets.DT_INSPlayer.m_flFlashDuration);
    }

    float* GetFlashMaxAlpha()
    {
        return (float*)((uintptr_t)this + offsets.DT_INSPlayer.m_flFlashMaxAlpha);
    }

    void* GetActiveWeapon()
    {
        return (void*)((uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hActiveWeapon);
    }

    int* GetWeapons()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hMyWeapons);
    }

    int* GetWearables()
    {
        return (int*)((uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hMyWearables);
    }

    bool GetAlive()
    {
        return this->GetHealth() > 0 && this->GetLifeState() == LIFE_ALIVE;
    }

    Vector GetEyePosition()
    {
        return this->GetVecOrigin() + this->GetVecViewOffset();
    }

    inline Vector GetBonePosition(int boneIndex)
    {
        matrix3x4_t BoneMatrix[MAXSTUDIOBONES];

        if (!this->SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
            return this->GetVecOrigin();

        matrix3x4_t hitbox = BoneMatrix[boneIndex];

        return Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
    }

    QAngle* GetVAngles()
    {
        return (QAngle*)((uintptr_t)this + offsets.DT_BasePlayer.deadflag + 0x4);
    }

    unsigned* GetINSFlags()
    {
        return (unsigned*)((uintptr_t)this + offsets.DT_INSPlayerShared.m_iPlayerFlags);
    }

    bool IsScoped()
    {
        return (bool) (*(GetINSFlags()) & 1);
    }

    bool IsSprinting()
    {
        return (bool) (*(GetINSFlags()) & 4);
    }
};


class C_BaseViewModel: public C_BaseEntity
{
public:
    int GetWeapon()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BaseViewModel.m_hWeapon);
    }

    int GetOwner()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BaseViewModel.m_hOwner);
    }
};


class C_BaseCombatWeapon: public C_BaseEntity
{
public:
    int GetOwner()
    {
        return *(int*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_hOwner);
    }

    unsigned int GetAmmo()
    {
        return *(unsigned int*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_iClip1);
    }

    float GetNextPrimaryAttack()
    {
        return *(float*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_flNextPrimaryAttack);
    }

    char* GetName()
    {
        typedef char* (* oGetName)(void*);
        return getvfunc<oGetName>(this, 402)(this);
    }

    bool GetInReload()
    {
        return *(bool*)((uintptr_t)this + offsets.DT_BaseCombatWeapon.m_bInReload);
    }

    float GetAccuracyPenalty()
    {
        return *(float*)((uintptr_t)this + offsets.DT_WeaponCSBase.m_fAccuracyPenalty);
    }

    bool IsGrenade()
    {
        char* weaponName = this->GetName();
        if (strcmp(weaponName, "weapon_f1")==0 ||
            strcmp(weaponName, "weapon_m67")==0 ||
            strcmp(weaponName, "weapon_m18")==0 ||
            strcmp(weaponName, "weapon_m84")==0 ||
            strcmp(weaponName, "weapon_anm14")==0 ||
            strcmp(weaponName, "weapon_molotov")==0 ||
            strcmp(weaponName, "weapon_gp25_he")==0 ||
            strcmp(weaponName, "weapon_m203")==0)
            return true;

        return false;
    }
};

class GrenadeThrownBase: C_BaseEntity
{
    public:
    bool IsPinPulled()
    {
        return *(bool*)((uintptr_t)this + offsets.DT_INSGrenadeBase.m_bPinPulled);
    }

    float GetThrowTime()
    {
        return *(float*)((uintptr_t)this + offsets.DT_INSGrenadeBase.m_fThrowTime);
    }

    Vector GetVecInitialVelocity() {
        return *(Vector*)((uintptr_t)this + offsets.DT_GrenadeThrownBase.m_vecInitialVelocity);
    }

    Vector GetVecVelocity() {
        return *(Vector*)((uintptr_t)this + offsets.DT_GrenadeThrownBase.m_vecVelocity);
    }
};
