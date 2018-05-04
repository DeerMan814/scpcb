Function FillRoom_closets_2(r.Rooms)
    Local d.Doors, d2.Doors, sc.SecurityCams, de.Decals, r2.Rooms, sc2.SecurityCams
	Local it.Items, i%
	Local xtemp%, ytemp%, ztemp%
	
	Local t1;, Bump
    it = CreateItem("Document SCP-173", "paper", r\x + 736.0 * RoomScale, r\y + 176.0 * RoomScale, r\z + 736.0 * RoomScale)
    EntityParent(it\collider, r\obj)
    
    it = CreateItem("Gas Mask", "gasmask", r\x + 736.0 * RoomScale, r\y + 176.0 * RoomScale, r\z + 544.0 * RoomScale)
    ScaleEntity(it\collider, 0.02, 0.02, 0.02) : EntityParent(it\collider, r\obj)
    
    it = CreateItem("9V Battery", "bat", r\x + 736.0 * RoomScale, r\y + 176.0 * RoomScale, r\z - 448.0 * RoomScale)
    EntityParent(it\collider, r\obj)
    If Rand(2) = 1 Then
        it = CreateItem("9V Battery", "bat", r\x + 730.0 * RoomScale, r\y + 176.0 * RoomScale, r\z - 496.0 * RoomScale)
        EntityParent(it\collider, r\obj)
    EndIf
    If Rand(2) = 1 Then
        it = CreateItem("9V Battery", "bat", r\x + 740.0 * RoomScale, r\y + 176.0 * RoomScale, r\z - 560.0 * RoomScale)
        EntityParent(it\collider, r\obj)
    EndIf
    
    it = CreateItem("Level 1 Key Card", "key1", r\x + 736.0 * RoomScale, r\y + 240.0 * RoomScale, r\z + 752.0 * RoomScale)
    EntityParent(it\collider, r\obj)
    
    Local clipboard.Items = CreateItem("Clipboard","clipboard",r\x + 736.0 * RoomScale, r\y + 224.0 * RoomScale, r\z -480.0 * RoomScale)
    EntityParent(it\collider, r\obj)
    
    it = CreateItem("Incident Report SCP-1048-A", "paper",r\x + 736.0 * RoomScale, r\y + 224.0 * RoomScale, r\z -480.0 * RoomScale)
    ;clipboard\SecondInv[0] = it
    HideEntity(it\collider)
    
    r\Objects[0]=CreatePivot(r\obj)
    PositionEntity r\Objects[0], r\x-1120*RoomScale, -256*RoomScale, r\z+896*RoomScale, True
    r\Objects[1]=CreatePivot(r\obj)
    PositionEntity r\Objects[1], r\x-1232*RoomScale, -256*RoomScale, r\z-160*RoomScale, True
    
    d.Doors = CreateDoor(0, r\x - 240.0 * RoomScale, 0.0, r\z, 90, r, False)
    d\open = False : d\AutoClose = False 
    MoveEntity(d\buttons[0], 0.0, 0.0, 22.0 * RoomScale)
    MoveEntity(d\buttons[1], 0.0, 0.0, 22.0 * RoomScale)
    
    sc.SecurityCams = CreateSecurityCam(r\x, r\y + 704*RoomScale, r\z + 863*RoomScale, r)
    sc\angle = 180
    sc\turn = 45
    TurnEntity(sc\CameraObj, 20, 0, 0)
    sc\ID = 0
    ;sc\FollowPlayer = True
End Function


Function UpdateEvent_closets_2(e.Events)
	Local dist#, i%, temp%, pvt%, strtemp$, j%, k%

	Local p.Particles, n.NPCs, r.Rooms, e2.Events, it.Items, em.Emitters, sc.SecurityCams, sc2.SecurityCams

	Local CurrTrigger$ = ""

	Local x#, y#, z#

	Local angle#

	;[Block]
	If e\EventState = 0 Then
		If (Not e\loaded) Then
			If e\room\NPC[0]=Null Then
				e\room\NPC[0] = CreateNPC(NPCtypeD, EntityX(e\room\Objects[0],True),EntityY(e\room\Objects[0],True),EntityZ(e\room\Objects[0],True))
			EndIf
			e\room\NPC[0]\texture = "GFX\npcs\janitor.jpg"
			Local tex% = LoadTexture(e\room\NPC[0]\texture)
			
			EntityTexture e\room\NPC[0]\obj, tex
			FreeTexture tex
			
			e\room\NPC[0]\sound=LoadSound("SFX\Room\Storeroom\Escape1.ogg")
			
			e\room\NPC[0]\soundChn = PlayRangedSound(e\room\NPC[0]\sound, mainPlayer\cam, e\room\NPC[0]\collider, 12)
			
			If e\room\NPC[1]=Null Then
				e\room\NPC[1] = CreateNPC(NPCtypeD, EntityX(e\room\Objects[1],True),EntityY(e\room\Objects[1],True),EntityZ(e\room\Objects[1],True))
			EndIf
			e\room\NPC[1]\texture = "GFX\npcs\scientist.jpg"
			tex = LoadTexture(e\room\NPC[1]\texture)
			EntityTexture e\room\NPC[1]\obj, tex
			
			FreeTexture tex
			
			e\room\NPC[1]\sound=LoadSound("SFX\Room\Storeroom\Escape2.ogg")
			
			PointEntity e\room\NPC[0]\collider, e\room\NPC[1]\collider
			PointEntity e\room\NPC[1]\collider, e\room\NPC[0]\collider
			
			e\loaded = True
			e\EventState=1
		EndIf
	Else
		e\EventState=e\EventState+timing\tickDuration
		If e\EventState < 70*3.5 Then
			RotateEntity(e\room\NPC[1]\collider,0,CurveAngle(e\room\angle+90,EntityYaw(e\room\NPC[1]\collider),100.0),0,True)
			
			e\room\NPC[0]\state=1
			If e\EventState > 70*3.2 And e\EventState-timing\tickDuration =< 70*3.2 Then PlayRangedSound(IntroSFX(15),mainPlayer\cam,e\room\obj,15.0)
		ElseIf e\EventState < 70*6.5
			If e\EventState-timing\tickDuration < 70*3.5 Then
				e\room\NPC[0]\state=0
				e\room\NPC[1]\soundChn = PlayRangedSound(e\room\NPC[1]\sound, mainPlayer\cam, e\room\NPC[1]\collider,12.0)
			EndIf
			
			If e\EventState > 70*4.5 Then
				PointEntity e\room\NPC[0]\obj, e\room\obj
				RotateEntity(e\room\NPC[0]\collider,0,CurveAngle(EntityYaw(e\room\NPC[0]\obj),EntityYaw(e\room\NPC[0]\collider),30.0),0,True)
			EndIf
			PointEntity e\room\NPC[1]\obj, e\room\obj
			TurnEntity e\room\NPC[1]\obj, 0, Sin(e\EventState)*25, 0
			RotateEntity(e\room\NPC[1]\collider,0,CurveAngle(EntityYaw(e\room\NPC[1]\obj),EntityYaw(e\room\NPC[1]\collider),30.0),0,True)
		Else
			If e\EventState-timing\tickDuration < 70*6.5 Then 
				;PlaySound (HorrorSFX(0))
				PlaySound (LightSFX)
			EndIf
			mainPlayer\blinkTimer = Max((70*6.5-e\EventState)/5.0 - Rnd(0.0,2.0),-10)
			If mainPlayer\blinkTimer =-10 Then
				;TODO: fix
				;If e\EventState > 70*7.5 And e\EventState-timing\tickDuration =< 70*7.5 Then PlayRangedSound(NeckSnapSFX(0),mainPlayer\cam,e\room\NPC[0]\Collider,8.0)
				;If e\EventState > 70*8.0 And e\EventState-timing\tickDuration =< 70*8.0 Then PlayRangedSound(NeckSnapSFX(1),mainPlayer\cam,e\room\NPC[1]\Collider,8.0)
				SetNPCFrame e\room\NPC[0], 60
				e\room\NPC[0]\State=8
				
				SetNPCFrame e\room\NPC[1], 19
				e\room\NPC[1]\State = 6
			EndIf
			
			If e\EventState > 70*8.5 Then
				PositionEntity Curr173\Collider, (EntityX(e\room\Objects[0],True)+EntityX(e\room\Objects[1],True))/2,EntityY(e\room\Objects[0],True),(EntityZ(e\room\Objects[0],True)+EntityZ(e\room\Objects[1],True))/2
				PointEntity Curr173\Collider, mainPlayer\collider
				ResetEntity Curr173\Collider
				RemoveEvent(e)
			EndIf
		EndIf
	EndIf
	;[End Block]
End Function

;~IDEal Editor Parameters:
;~C#Blitz3D