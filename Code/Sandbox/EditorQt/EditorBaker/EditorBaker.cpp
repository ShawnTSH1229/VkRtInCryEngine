#include "StdAfx.h"
#include "EditorBaker.h"
#include "GameEngine.h"

#include "Objects/EntityObject.h"
#include "Objects/Group.h"
#include "Objects/BrushObject.h"

#include <set>
#include <IObjectManager.h>
#include <CryRenderer/IRenderer.h>
#include <CryRenderer/IGIBaker.h>
#include <Cry3DEngine/IStatObj.h>

CEditorBaker::CEditorBaker()
{
}

CEditorBaker::~CEditorBaker()
{
}

void CEditorBaker::Bake(IObjectManager* pObjectManager)
{
	static bool bAdded = false;
	
	IGIBaker* pGIBaker = gEnv->pRenderer->GetIGIBaker();

	if (!bAdded)
	{
		CBaseObjectsArray objects;
		pObjectManager->GetObjects(objects);

		std::set<void*> addedObjects;//will track the already added objects to never include an object twice (prefabs contain objects refered to elsewhere too)
		const int cObjectsSize = objects.size();

		// add meshes
		for (int i = 0; i < cObjectsSize; ++i)
		{
			if (objects[i]->GetType() == OBJTYPE_BRUSH)
			{
				if (addedObjects.find(objects[i]) == addedObjects.end())
				{
					if (!objects[i]->IsKindOf(RUNTIME_CLASS(CBrushObject)))
					{
						continue;
					}

					CBrushObject* pObj = (CBrushObject*)(objects[i]);
					IStatObj* pStatObj = pObj->GetIStatObj();
					IIndexedMesh* pIMesh = pStatObj->GetIndexedMesh();
					if (pIMesh)
					{
						Matrix33 rotation;
						rotation.SetRow(0, pObj->GetRotation().GetRow0());
						rotation.SetRow(1, pObj->GetRotation().GetRow1());
						rotation.SetRow(2, pObj->GetRotation().GetRow2());
						
						SMeshParam meshParam;
						meshParam.m_worldTM = Matrix44A(pObj->GetWorldTM());
						meshParam.m_rotationTM = Matrix44A(rotation);

						pGIBaker->AddMesh(pStatObj, meshParam);
						addedObjects.insert(objects[i]);
					}
				}
			}
		}

		bAdded = true;
	}
	

	pGIBaker->Bake();
}