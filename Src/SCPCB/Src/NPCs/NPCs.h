#ifndef NPCS_H_INCLUDED
#define NPCS_H_INCLUDED
#include <vector>

namespace CBN {

// Structs.
struct NPC {
private:
    static std::vector<NPC*> list;

public:
    NPC();
    ~NPC();
    static int getListSize();
    static NPC* getObject(int index);

    int obj;
    int obj2;
    int obj3;
    int obj4;
    int collider;

    int npcType;
    int id;

    int polyData;

    //TODO: Deprecate state2 and state3. Make state an integer.
    float state;
    float state2;
    float state3;
    int prevState;

    //TODO: Deprecate?
    int makingNoise;

    float frame;
    float angle;

    int sounds[NPC_SOUND_COUNT];
    int soundChannels[NPC_CHANNEL_COUNT];

    float playerDistance;

    //TODO: wtf why aren't we using this more instead of reload?
    float soundTimer;

    float speed;
    float currSpeed;

    String texture;

    //TODO: Deprecate in favor of state.
    float idle;

    //TODO: Deprecate in favor of timer.
    float reload;

    float timer;

    int lastSeen;
    float lastDist;

    float prevX;
    float prevY;
    float prevZ;

    NPC* target;
    int targetID;

    //TODO: Deprecate in favor of target.
    float enemyX;
    float enemyY;
    float enemyZ;

    float targetX;
    float targetY;
    float targetZ;

    WayPoint* path[20];
    int pathStatus;
    float pathTimer;
    int pathLocation;
    float pathX;
    float pathZ;

    float nvX;
    float nvY;
    float nvZ;
    String nvName;

    float dropSpeed;
    int gravity;
    float gravityMult = 1.0;

    //TODO: Deprecate in favor of terminalVelocity.
    float maxGravity = 0.2;

    float terminalVelocity = 0.2;

    //TODO: Move this to DataMTF
    int mtfVariant;
    NPC* mtfLeader;
    int isDead;
    float blinkTimer = 1.0;
    int ignorePlayer;

    //TODO: Deprecate.
    String npcNameInSection;
    int manipulateBone;
    String boneToManipulate;
    String boneToManipulate2;
    int manipulationType;

    float boneX;
    float boneY;
    float boneZ;

    float bonePitch;
    float boneYaw;
    float boneRoll;

    //TODO: Burn this stuff in hell.
    int inFacility = true;
    int canUseElevator = false;
    ElevatorObj* currElevator;
    int hp;
    String model;
    float modelScaleX;
    float modelScaleY;
    float modelScaleZ;
};

// Constants.
extern const int NPCtype173;
extern const int NPCtype106;
extern const int NPCtype049;
extern const int NPCtype096;
extern const int NPCtype966;
extern const int NPCtype860;
extern const int NPCtype939;
extern const int NPCtype5131;
extern const int NPCtype066;
extern const int NPCtypeD;
extern const int NPCtypeGuard;
extern const int NPCtypeMTF;
extern const int NPCtypeApache;
extern const int NPCtypeZombie;
extern const int NPCtypeTentacle;
extern const int NPCtype1499;
extern const int NPCtypePdPlane;
extern const int NPC_SOUND_COUNT;
extern const int NPC_CHANNEL_COUNT;

// Globals.
extern NPC* Curr173;
extern NPC* Curr106;
extern NPC* Curr096;

// Functions.
NPC* CreateNPC(int NPCtype, float x, float y, float z);

void LoadOrCopyMesh(NPC* n, String filePath);

void RemoveNPC(NPC* n);

void UpdateNPCs();

void NPCStopAllChannels(NPC* n);

void TeleportCloser(NPC* n);

int OtherNPCSeesMeNPC(NPC* me, NPC* other);

int MeNPCSeesPlayer(NPC* me, int disableSoundOnCrouch = false);

void TeleportMTFGroup(NPC* n);

void Shoot(float x, float y, float z, float hitProb = 1.0, int particles = true, int instaKill = false);

void PlayMTFSound(int sound, NPC* n);

void MoveToPocketDimension();

int FindFreeNPCID();

void ForceSetNPCID(NPC* n, int newID);

void Console_SpawnNPC(String c_input, int state = -9999);

void ManipulateNPCBones();

String GetNPCManipulationValue(String NPC, String bone, String section, int valuetype = 0);

float TransformNPCManipulationData(String NPC, String bone, String section);

void NPCSpeedChange(NPC* n);

int PlayerInReachableRoom();

int CheckForNPCInFacility(NPC* n);

void FindNextElevator(NPC* n);

void GoToElevator(NPC* n);

void FinishWalking(NPC* n, float startframe, float endframe, float speed);

void RotateToDirection(NPC* n);

void AnimateNPC(NPC* n, float start, float quit, float speed, int loop = true);

void SetNPCFrame(NPC* n, float frame);

}
#endif // NPCS_H_INCLUDED