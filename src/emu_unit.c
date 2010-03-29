/* $Id$ */

#include <stdio.h>
#include "types.h"
#include "libemu.h"
#include "global.h"
#include "pool/unit.h"
#include "pool/team.h"
#include "house.h"
#include "unit.h"
#include "pool/pool.h"
#include "team.h"

/**
 * Emulator wrapper around Unit_GetHouseID().
 *
 * @name emu_Unit_GetHouseID
 * @implements 1A34:3623:0010:B11C ()
 */
void emu_Unit_GetHouseID()
{
	csip32 ucsip;
	Unit *u;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip = emu_get_csip32(emu_ss, emu_sp, 0x0);

	emu_ax = HOUSE_INVALID;

	if (ucsip.csip == 0x0) return;
	u = Unit_Get_ByMemory(ucsip);

	emu_ax = Unit_GetHouseID(u);
}

/**
 * Emulator wrapper around Unit_Create().
 *
 * @name emu_Unit_Create
 * @implements 1A34:08FB:002C:D192 ()
 */
void emu_Unit_Create()
{
	uint16 index, unknown;
	uint8 typeID, houseID;
	tile32 position;
	Unit *u;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	index    =        emu_get_memory16(emu_ss, emu_sp, 0x0);
	typeID   = (uint8)emu_get_memory16(emu_ss, emu_sp, 0x2);
	houseID  = (uint8)emu_get_memory16(emu_ss, emu_sp, 0x4);
	position =        emu_get_tile32  (emu_ss, emu_sp, 0x6);
	unknown  =        emu_get_memory16(emu_ss, emu_sp, 0xA);

	u = Unit_Create(index, typeID, houseID, position, (uint8)unknown);

	emu_ax = 0x0;
	emu_dx = 0x0;

	if (u == NULL) return;
	emu_dx = g_global->unitStartPos.s.cs;
	emu_ax = g_global->unitStartPos.s.ip + u->index * sizeof(Unit);
}

/**
 * Decompiled function emu_Unit_IsTypeOnMap()
 *
 * @name emu_Unit_IsTypeOnMap
 * @implements 0FE4:0333:000F:2B73 ()
 */
void emu_Unit_IsTypeOnMap()
{
	uint8 houseID, typeID;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	houseID = (uint8)emu_get_memory16(emu_ss, emu_sp, 0x0);
	typeID  = (uint8)emu_get_memory16(emu_ss, emu_sp, 0x2);

	emu_ax = Unit_IsTypeOnMap(houseID, typeID) ? 1 : 0;
}

/**
 * Emulator wrapper around Unit_SetAction().
 *
 * @name emu_Unit_SetAction
 * @implements 176C:000E:000E:633D ()
 */
void emu_Unit_SetAction()
{
	csip32 ucsip;
	Unit *u;
	ActionType action;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip  = emu_get_csip32  (emu_ss, emu_sp, 0x0);
	action = emu_get_memory16(emu_ss, emu_sp, 0x4);

	if (ucsip.csip == 0x0) return;
	u = Unit_Get_ByMemory(ucsip);

	Unit_SetAction(u, action);
}

/**
 * Emulator wrapper around Unit_AddToTeam().
 *
 * @name emu_Unit_AddToTeam
 * @implements 16BC:0001:0018:AE47 ()
 */
void emu_Unit_AddToTeam()
{
	csip32 csip_team, csip_unit;
	Team *t;
	Unit *u;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	/* Set the default return value */
	emu_ax = 0;

	/* emu implementation passes team as first arg */
	csip_team = emu_get_csip32(emu_ss, emu_sp, 0x0);
	csip_unit = emu_get_csip32(emu_ss, emu_sp, 0x4);

	if (csip_team.csip == 0x0 || csip_unit.csip == 0x0) return;

	t = Team_Get_ByMemory(csip_team);
	u = Unit_Get_ByMemory(csip_unit);

	emu_ax = Unit_AddToTeam(u, t);
}

/**
 * Emulator wrapper around Unit_RemoveFromTeam().
 *
 * @name emu_Unit_RemoveFromTeam
 * @implements 16BC:0044:0027:CB92 ()
 */
void emu_Unit_RemoveFromTeam()
{
	csip32 csip_unit;
	Unit *u;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	/* Set the default return value */
	emu_ax = 0;

	csip_unit = emu_get_csip32(emu_ss, emu_sp, 0x0);

	if (csip_unit.csip == 0x0) return;

	u = Unit_Get_ByMemory(csip_unit);

	emu_ax = Unit_RemoveFromTeam(u);
}

/**
 * Emulator wrapper around Unit_GetTeam().
 *
 * @name emu_Unit_GetTeam
 * @implements 0FE4:0632:0031:75AA ()
 */
void emu_Unit_GetTeam()
{
	csip32 ucsip;
	Unit *u;
	Team *t;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip = emu_get_csip32(emu_ss, emu_sp, 0x0);

	emu_dx = 0x0;
	emu_ax = 0x0;

	if (ucsip.csip == 0x0) return;
	u = Unit_Get_ByMemory(ucsip);

	t = Unit_GetTeam(u);

	if (t == NULL) return;
	emu_dx = g_global->teamStartPos.s.cs;
	emu_ax = g_global->teamStartPos.s.ip + t->index * sizeof(Team);
}

/**
 * Emulator wrapper around Unit_Sort().
 *
 * @name emu_Unit_Sort
 * @implements 0FE4:0002:0021:D0A9 ()
 */
void emu_Unit_Sort()
{
	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	Unit_Sort();
}

/**
 * Emulator wrapper around Unit_Get_ByPackedTile()
 *
 * @name emu_Unit_Get_ByPackedTile
 * @implements B4CD:10EE:0039:EC73 ()
 */
void emu_Unit_Get_ByPackedTile()
{
	uint16 packed;
	Unit *u;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	packed = emu_get_memory16(emu_ss, emu_sp, 0x0);

	u = Unit_Get_ByPackedTile(packed);

	emu_ax = 0x0;
	emu_dx = 0x0;

	if (u == NULL) return;
	emu_dx = g_global->unitStartPos.s.cs;
	emu_ax = g_global->unitStartPos.s.ip + u->index * sizeof(Unit);
}

/**
 * Emulator wrapper around Unit_SetDestination().
 *
 * @name emu_Unit_SetDestination
 * @implements 1A34:1B68:0019:AAA0 ()
 */
void emu_Unit_SetDestination()
{
	csip32 ucsip;
	Unit *u;
	uint16 destination;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip       = emu_get_csip32  (emu_ss, emu_sp, 0x0);
	destination = emu_get_memory16(emu_ss, emu_sp, 0x4);

	if (ucsip.csip == 0x0) return;
	u = Unit_Get_ByMemory(ucsip);

	Unit_SetDestination(u, destination);
}

/**
 * Emulator wrapper around Unit_GetTargetPriority().
 *
 * @name emu_Unit_GetTargetPriority
 * @implements 1A34:117E:001D:E17B ()
 */
void emu_Unit_GetTargetPriority()
{
	csip32 unitcsip;
	csip32 targetcsip;
	Unit *unit;
	Unit *target;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	unitcsip   = emu_get_csip32(emu_ss, emu_sp, 0x0);
	targetcsip = emu_get_csip32(emu_ss, emu_sp, 0x4);

	emu_ax = 0;

	if (unitcsip.csip == 0x0) return;
	if (targetcsip.csip == 0x0) return;

	unit   = Unit_Get_ByMemory(unitcsip);
	target = Unit_Get_ByMemory(targetcsip);

	emu_ax = Unit_GetTargetPriority(unit, target);
}

/**
 * Emulator wrapper around Unit_FindClosestRefinery()
 *
 * @name emu_Unit_FindClosestRefinery
 * @implements 1A34:3312:0017:29E8 ()
 */
void emu_Unit_FindClosestRefinery()
{
	csip32 ucsip;
	Unit *u;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip = emu_get_csip32(emu_ss, emu_sp, 0x0);

	emu_ax = 0;

	if (ucsip.csip == 0x0) return;

	u = Unit_Get_ByMemory(ucsip);

	emu_ax = Unit_FindClosestRefinery(u);
}

/**
 * Emulator wrapper around Unit_SetPosition()
 *
 * @name emu_Unit_SetPosition
 * @implements 1A34:2958:0013:3A47 ()
 */
void emu_Unit_SetPosition()
{
	csip32 ucsip;
	Unit *u;
	tile32 position;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip    = emu_get_csip32(emu_ss, emu_sp, 0x0);
	position = emu_get_tile32(emu_ss, emu_sp, 0x4);

	emu_ax = 0;

	if (ucsip.csip == 0x0) return;
	u = Unit_Get_ByMemory(ucsip);

	emu_ax = Unit_SetPosition(u, position) ? 1 : 0;
}

/**
 * Emulator wrapper around Unit_Unknown10EC()
 *
 * @name emu_Unit_Unknown10EC
 * @implements 1A34:10EC:000E:A326 ()
 */
void emu_Unit_Unknown10EC()
{
	csip32 ucsip;
	Unit *u;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip = emu_get_csip32(emu_ss, emu_sp, 0x0);

	if (ucsip.csip == 0x0) return;
	u = Unit_Get_ByMemory(ucsip);

	Unit_Unknown10EC(u);
}

/**
 * Emulator wrapper around Unit_FindBestTarget()
 *
 * @name emu_Unit_FindBestTarget
 * @implements 1A34:1328:0026:C398 ()
 */
void emu_Unit_FindBestTarget()
{
	csip32 ucsip;
	uint16 mode;
	Unit *unit;
	Unit *target;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip = emu_get_csip32(emu_ss, emu_sp, 0x0);
	mode  = emu_get_memory16(emu_ss, emu_sp, 0x4);

	emu_ax = 0x0;
	emu_dx = 0x0;

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	target = Unit_FindBestTarget(unit, mode);

	if (target == NULL) return;
	emu_dx = g_global->unitStartPos.s.cs;
	emu_ax = g_global->unitStartPos.s.ip + target->index * sizeof(Unit);
}

/**
 * Emulator wrapper around Unit_Unknown15F4()
 *
 * @name emu_Unit_Unknown15F4
 * @implements 1A34:15F4:0026:EDA4 ()
 */
void emu_Unit_Unknown15F4()
{
	csip32 ucsip;
	Unit *unit;
	Unit *res;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip = emu_get_csip32(emu_ss, emu_sp, 0x0);

	emu_ax = 0x0;
	emu_dx = 0x0;

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	res = Unit_Unknown15F4(unit);
	emu_dx = g_global->unitStartPos.s.cs;
	emu_ax = g_global->unitStartPos.s.ip + res->index * sizeof(Unit);
}

/**
 * Emulator wrapper around Unit_Unknown167C()
 *
 * @name emu_Unit_Unknown167C
 * @implements 1A34:167C:0015:AA25 ()
 */
void emu_Unit_Unknown167C()
{
	csip32 ucsip;
	Unit *u;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip = emu_get_csip32(emu_ss, emu_sp, 0x0);

	emu_ax = 0;

	if (ucsip.csip == 0x0) return;
	u = Unit_Get_ByMemory(ucsip);

	emu_ax = Unit_Unknown167C(u) ? 1 : 0;
}

/**
 * Emulator wrapper around Unit_SetTarget()
 *
 * @name emu_Unit_SetTarget
 * @implements 1A34:1A66:0011:34D4 ()
 */
void emu_Unit_SetTarget()
{
	csip32 ucsip;
	uint16 encoded;
	Unit *unit;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip   = emu_get_csip32(emu_ss, emu_sp, 0x0);
	encoded = emu_get_memory16(emu_ss, emu_sp, 0x4);

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	Unit_SetTarget(unit, encoded);
}

/**
 * Emulator wrapper around Unit_Deviation_Decrease()
 *
 * @name emu_Unit_Deviation_Decrease
 * @implements 1A34:193F:0013:FA4D ()
 */
void emu_Unit_Deviation_Decrease()
{
	csip32 ucsip;
	uint16 amount;
	Unit *unit;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip  = emu_get_csip32(emu_ss, emu_sp, 0x0);
	amount = emu_get_memory16(emu_ss, emu_sp, 0x4);

	emu_ax = 0;

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	emu_ax = Unit_Deviation_Decrease(unit, amount) ? 1 : 0;
}

/**
 * Emulator wrapper around Unit_RemoveFog()
 *
 * @name emu_Unit_RemoveFog
 * @implements 1A34:2B18:0011:E4D5 ()
 */
void emu_Unit_RemoveFog()
{
	csip32 ucsip;
	Unit *unit;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip  = emu_get_csip32(emu_ss, emu_sp, 0x0);

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	Unit_RemoveFog(unit);
}

/**
 * Emulator wrapper around Unit_Deviate()
 *
 * @name emu_Unit_Deviate
 * @implements 1A34:1859:0017:DF09 ()
 */
void emu_Unit_Deviate()
{
	csip32 ucsip;
	uint16 probability;
	Unit *unit;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip       = emu_get_csip32(emu_ss, emu_sp, 0x0);
	probability = emu_get_memory16(emu_ss, emu_sp, 0x4);

	emu_ax = 0;

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	emu_ax = Unit_Deviate(unit, probability) ? 1 : 0;
}

/**
 * Emulator wrapper around Unit_Unknown0005()
 *
 * @name emu_Unit_Unknown0005
 * @implements 1A34:0005:001F:38E2 ()
 */
void emu_Unit_Unknown0005()
{
	csip32 ucsip;
	uint16 distance;
	Unit *unit;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip    = emu_get_csip32(emu_ss, emu_sp, 0x0);
	distance = emu_get_memory16(emu_ss, emu_sp, 0x4);

	emu_ax = 0;

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	emu_ax = Unit_Unknown0005(unit, distance) ? 1 : 0;
}

/**
 * Emulator wrapper around Unit_Damage()
 *
 * @name emu_Unit_Damage
 * @implements 1A34:0B71:0033:9787 ()
 */
void emu_Unit_Damage()
{
	csip32 ucsip;
	uint16 damage;
	uint16 range;
	Unit *unit;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip  = emu_get_csip32(emu_ss, emu_sp, 0x0);
	damage = emu_get_memory16(emu_ss, emu_sp, 0x4);
	range  = emu_get_memory16(emu_ss, emu_sp, 0x6);

	emu_ax = 0;

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	emu_ax = Unit_Damage(unit, damage, range) ? 1 : 0;
}

/**
 * Emulator wrapper around Unit_UntargetMe()
 *
 * @name emu_Unit_UntargetMe
 * @implements 1A34:364E:0012:61B3 ()
 */
void emu_Unit_UntargetMe()
{
	csip32 ucsip;
	Unit *unit;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip = emu_get_csip32(emu_ss, emu_sp, 0x0);

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	Unit_UntargetMe(unit);
}

/**
 * Emulator wrapper around Unit_Unknown1E99()
 *
 * @name emu_Unit_Unknown1E99
 * @implements 1A34:1E99:0012:1117 ()
 */
void emu_Unit_Unknown1E99()
{
	csip32 ucsip;
	uint16 arg0A;
	uint16 arg0C;
	uint16 i;
	Unit *unit;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip = emu_get_csip32(emu_ss, emu_sp, 0x0);
	arg0A = emu_get_memory16(emu_ss, emu_sp, 0x4);
	arg0C = emu_get_memory16(emu_ss, emu_sp, 0x6);
	i     = emu_get_memory16(emu_ss, emu_sp, 0x8);

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	Unit_Unknown1E99(unit, (uint8)arg0A, (arg0C != 0) ? true : false, i);
}

/**
 * Emulator wrapper around Unit_Select()
 *
 * @name emu_Unit_Select
 * @implements 1A34:0F48:0018:0DB8 ()
 */
void emu_Unit_Select()
{
	csip32 ucsip;
	Unit *unit;

	/* Pop the return CS:IP. */
	emu_pop(&emu_ip);
	emu_pop(&emu_cs);

	ucsip = emu_get_csip32(emu_ss, emu_sp, 0x0);

	if (ucsip.csip == 0x0) return;
	unit = Unit_Get_ByMemory(ucsip);

	Unit_Select(unit);
}
