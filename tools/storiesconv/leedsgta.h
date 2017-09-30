// Stuff shared by LCS and VCS (even though they might differ)

enum ModelInfoType
{
	MODELINFO_SIMPLE       = 1,
	MODELINFO_MLO          = 2,	// unused
	MODELINFO_TIME         = 3,
	MODELINFO_WEAPON       = 4,
	MODELINFO_ELEMENTGROUP = 5,
	MODELINFO_VEHICLE      = 6,
	MODELINFO_PED          = 7,
	MODELINFO_XTRACOMP     = 8,	// unused
};

struct CVector
{
	float x, y, z;
};

struct sChunkHeader
{
	uint32   ident;
	bool32   type;
	uint32   fileSize;
	uint32   dataSize;
	uint32   relocTab;
	uint32   numRelocs;
	uint32   globalTab;
	uint16   numClasses;
	uint16   numFuncs;
};

struct TexListDef
{
	RslTexList *texlist;
	int32 refCount;
	char name[20];
};

// might be nicer to have this as proper templates
struct CPool_generic
{
	void *items;
	void *flags;
	int  size;
	int  allocPtr;
	char name[16];
};

struct CPool_txd
{
	TexListDef *items;
	void *flags;
	int  size;
	int  allocPtr;
	char name[16];
};

struct CBaseModelInfo;

struct ResourceImage {
	void *paths;
	CPool_generic *buildingPool;
	CPool_generic *treadablePool;
	CPool_generic *dummyPool;
	CPool_generic *entryInfoNodePool;
	CPool_generic *ptrNodePool;
	int32 numModelInfos;
	CBaseModelInfo **modelInfoPtrs;
	void *carArrays;
	void *totalNumOfCarsOfRating;
	void *theZones;	// gta3.zon
	void *sectors;
	void *bigBuildingList;
	void *_2deffectStore;
	void *_2deffects;
	int16 *modelIndices;
	CPool_txd *texlistPool;
	RslTexList *storedTexList;
	CPool_generic *colPool;
	int32 colOnlyBB;
	void *tempColModels;
	void *objectInfo;	// object.dat
#ifdef LCS
	void *vehicleModelInfo_Info;	// carcols.dat
#else
	void *unknown0;
#endif
	void *streaming_Inst;	// gta3.dir

	void *animManagerInst;	// ifps
	void *fightMoves;		// fistfite.dat

#ifdef LCS
	void *pedAnimInfo;
#endif
	void *pedType;		// ped.dat
	void *pedStats;		// pedstats.dat

#ifdef VCS
	void *vehicleModelInfo_Info;	// carcols.dat
	void *unknown1;
#endif

	int32 numAttributeZones;	// cull.ipl
	void *attributeZones;		// cull.ipl
	int32 numOccludersOnMap;
	void *occluders;
	void *waterLevelInst;		// waterpro.dat
#ifdef LCS
	void *handlingManager;		// handling.dat
#else
	void *weatherTypeList;		// not hardcoded in VCS?
#endif
	void *adhesiveLimitTable;		// surface.dat
	void *timecycle;			// timecyc.dat
	void *pedGroups;			// pedgrp.dat
	void *particleSystemManager;	// particle.dat
	void *weaponTables;		// weapon.dat
#ifdef VCS
	int unknown2;
#endif
	RslElementGroup **markers;	// [32] in LCS
	int cutsceneDir;

#ifdef LCS
	void *ferryInst;	// ferry.dat
	void *trainInst;	// tracks(2).dat
	void *planeInst;	// flight.dat
//	int UNUSED;	// only on mobile, which doesn't write a game.dtz/dat to begin with
#else
	// one probably flight.dat
	void *unknown3;
	void *unknown4;	// 0
	void *unknown5;
	void *unknown6;	// 0
#endif
	void *menuCompressedTextures;
#ifdef VCS
	void *unknown7;
	void *unknown8;
	void *unknown9;
	int32 unknown10;
	void *unknown11;
#endif
	int fontTexListSize;
	void *fontCompressedTexList;
#ifdef VCS
	void *radarTextures;
	// some more
#endif
};
#ifdef VCS
static_assert(offsetof(ResourceImage, weatherTypeList) == 0x8C, "ResourceImage: error");
static_assert(offsetof(ResourceImage, timecycle) == 0x94, "ResourceImage: error");
static_assert(offsetof(ResourceImage, markers)+0x20 == 0xC8, "ResourceImage: error");
static_assert(offsetof(ResourceImage, menuCompressedTextures)+0x20 == 0xE0, "ResourceImage: error");
static_assert(offsetof(ResourceImage, fontTexListSize)+0x20 == 0xF8, "ResourceImage: error");
#endif
#ifdef LCS
static_assert(offsetof(ResourceImage, markers)+0x20 == 0xC0, "ResourceImage: error");
static_assert(offsetof(ResourceImage, ferryInst)+0x20 == 0xC8, "ResourceImage: error");
#endif
