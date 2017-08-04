#include "III.h"

CEntity::CEntity(void)
{
	m_type = ENTITY_TYPE_NOTHING;
	m_status = ENTITY_STATUS_4;
	m_doCollision = 0;
	m_isVisible = 1;
	m_scanCode = -1;
	m_modelIndex = -1;
	m_rwObject = nil;
}

CEntity::~CEntity(void)
{
}

void
CEntity::SetupBigBuilding(void)
{
	CSimpleModelInfo *mi = (CSimpleModelInfo*)CModelInfo::GetModelInfo(m_modelIndex);
	m_isBigBuilding = 1;
	m_flagC20 = 1;
	m_doCollision = 0;
	m_level = CTheZones::GetLevelFromPosition(GetPosition());
	if(m_level == LEVEL_NONE &&
	   mi->GetTxdSlot() != CTxdStore::FindTxdSlot("generic")){
		mi->SetTexDictionary("generic");
		printf("%d:%s txd has been set to generic\n",
		       m_modelIndex, mi->GetName());
	}
	if(mi->GetLodDistance(0) > 2000.0f)
		m_level = LEVEL_NONE;
}

bool
CEntity::IsVisible(void)
{
	return m_rwObject && m_isVisible;
	// TODO: GetIsOnScreen
}

/* Adds an entity into all sectors of its type it belongs to */
void
CEntity::Add(void)
{
	int x, xstart, xmid, xend;
	int y, ystart, ymid, yend;
	CSector *s;
	CPtrList *list;

	CRect bounds = GetBoundRect();
	xstart = CWorld::GetSectorIndex(bounds.left);
	xend   = CWorld::GetSectorIndex(bounds.right);
	xmid   = CWorld::GetSectorIndex((bounds.left + bounds.right)/2.0f);
	ystart = CWorld::GetSectorIndex(bounds.bottom);
	yend   = CWorld::GetSectorIndex(bounds.top);
	ymid   = CWorld::GetSectorIndex((bounds.bottom + bounds.top)/2.0f);
	assert(xstart >= 0);
	assert(xend < 100);
	assert(ystart >= 0);
	assert(yend < 100);

	for(y = ystart; y <= yend; y++)
		for(x = xstart; x <= xend; x++){
			s = CWorld::GetSector(x, y);
			if(x == xmid && y == ymid) switch(m_type){
			case ENTITY_TYPE_BUILDING:
				list = &s->m_buildings;
				break;
			case ENTITY_TYPE_VEHICLE:
				list = &s->m_vehicles;
				break;
			case ENTITY_TYPE_PED:
				list = &s->m_peds;
				break;
			case ENTITY_TYPE_OBJECT:
				list = &s->m_objects;
				break;
			case ENTITY_TYPE_DUMMY:
				list = &s->m_dummies;
				break;
			}else switch(m_type){
			case ENTITY_TYPE_BUILDING:
				list = &s->m_buildingsOverlap;
				break;
			case ENTITY_TYPE_VEHICLE:
				list = &s->m_vehiclesOverlap;
				break;
			case ENTITY_TYPE_PED:
				list = &s->m_pedsOverlap;
				break;
			case ENTITY_TYPE_OBJECT:
				list = &s->m_objectsOverlap;
				break;
			case ENTITY_TYPE_DUMMY:
				list = &s->m_dummiesOverlap;
				break;
			}
			list->InsertItem(this);
		}
}

void
CEntity::DeleteRwObject(void)
{
	m_matrix.Detach();
	if(m_rwObject){
		if(m_rwObject->type == rw::Atomic::ID){
			rw::Atomic *a = (rw::Atomic*)m_rwObject;
			rw::Frame *f = a->getFrame();
			a->destroy();
			f->destroy();
		}else if(m_rwObject->type == rw::Clump::ID)
			((rw::Clump*)m_rwObject)->destroy();
		m_rwObject = nil;
		CModelInfo::GetModelInfo(m_modelIndex)->RemoveRef();
	}
}

void
CEntity::CreateRwObject(void)
{
	CBaseModelInfo *mi = CModelInfo::GetModelInfo(m_modelIndex);
	m_rwObject = mi->CreateInstance();
	if(m_rwObject == nil)
		return;
	if(m_rwObject->type == rw::Atomic::ID){
		rw::Atomic *a = (rw::Atomic*)m_rwObject;
		m_matrix.AttachRW(&a->getFrame()->matrix, false);
	}else if(m_rwObject->type == rw::Clump::ID){
		rw::Clump *c = (rw::Clump*)m_rwObject;
		m_matrix.AttachRW(&c->getFrame()->matrix, false);
	}
	mi->AddRef();
}

CRect
CEntity::GetBoundRect(void)
{
	CRect rect;
	rw::V3d v;
	CColModel *col = CModelInfo::GetModelInfo(m_modelIndex)->GetColModel();

	v = col->boundingBox.min;
	rw::V3d::transformPoints(&v, &v, 1, &m_matrix.m_matrix);
	rect.ContainPoint(CVector(v));

	v = col->boundingBox.max;
	rw::V3d::transformPoints(&v, &v, 1, &m_matrix.m_matrix);
	rect.ContainPoint(CVector(v));

	v = col->boundingBox.min;
	v.x = col->boundingBox.max.x;
	rw::V3d::transformPoints(&v, &v, 1, &m_matrix.m_matrix);
	rect.ContainPoint(CVector(v));

	v = col->boundingBox.max;
	v.x = col->boundingBox.min.x;
	rw::V3d::transformPoints(&v, &v, 1, &m_matrix.m_matrix);
	rect.ContainPoint(CVector(v));

	return rect;
}

void
CEntity::Render(void)
{
	if(m_rwObject){
		m_isBeingRendered = 1;
		if(m_rwObject->type == rw::Atomic::ID)
			((rw::Atomic*)m_rwObject)->render();
		else
			((rw::Clump*)m_rwObject)->render();
		m_isBeingRendered = 0;
	}
}

void
CEntity::SetupLighting(void)
{
	DeActivateDirectional();
	SetAmbientColours();
}

void
CEntity::RemoveLighting(void)
{
}
