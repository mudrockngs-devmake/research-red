// Research Red: custom native script commands.
#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "follower_npc.h"
#include "script.h"
#include "constants/event_objects.h"

// rr_ivy_to_scene localId: Ivy stops following and becomes a cutscene object.
// If the follower is visible, the scene object takes her tile and facing so
// the handoff is seamless; VAR_RESULT says whether that happened.
void RR_ScriptIvyFollowerToObject(struct ScriptContext *ctx)
{
    u32 localId = ScriptReadByte(ctx);
    struct ObjectEvent *follower;
    s16 x, y;
    enum Direction facing;
    u32 objId;

    gSpecialVar_Result = FALSE;
    if (!PlayerHasFollowerNPC())
        return;

    follower = &gObjectEvents[GetFollowerNPCObjectId()];
    if (follower->invisible)
    {
        DestroyFollowerNPC();
        return;
    }

    x = follower->currentCoords.x;
    y = follower->currentCoords.y;
    facing = follower->facingDirection;
    DestroyFollowerNPC();

    objId = GetObjectEventIdByLocalId(localId);
    if (objId >= OBJECT_EVENTS_COUNT)
        objId = TrySpawnObjectEvent(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    if (objId >= OBJECT_EVENTS_COUNT)
        return;

    MoveObjectEventToMapCoords(&gObjectEvents[objId], x, y);
    ObjectEventTurn(&gObjectEvents[objId], facing);
    gSpecialVar_Result = TRUE;
}
