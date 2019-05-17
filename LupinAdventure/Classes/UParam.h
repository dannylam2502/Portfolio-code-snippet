// Author: Frederick Tran
// Created: 30/4/2014
// A Struct of Union type, to assist EObject to set up component's properties.

#ifndef UParam_h__
#define UParam_h__

#include "cocos2d.h"
#include "Config.h"
USING_NS_CC;

class CAIMove;
class CAIAttack;
class CStats;
class EObject;

enum
{
	UPARAM_TYPE_VEC3 = 1,
	UPARAM_TYPE_VEC2,
	UPARAM_TYPE_VEC2_CO_MOVE_JUMP_VELO,
	UPARAM_TYPE_VEC2_CO_MOVE_VELO,
	UPARAM_TYPE_VEC2_CO_MOVE_CLIMB_VELO,
	UPARAM_TYPE_NODE,
	UPARAM_TYPE_KEYSET,
	UPARAM_TYPE_CSTATS,
	UPARAM_TYPE_CAIATTACK,
	UPARAM_TYPE_CAIMOVE,
	UPARAM_TYPE_INT,
	UPARAM_TYPE_COMMAND_PAIR,
	UPARAM_TYPE_ACTION_PAIR,
	UPARAM_TYPE_EOBJECT,
	UPARAM_TYPE_RECT
};

struct UParam
{
	// Remember to edit Copy-constructor and Destructor if have any change in the union.
	// Member of union must be pointer-type.
	union
	{
		void*				uReference;

		// Data storage pointers.
		b2Vec3*				uVec3Ptr;
		b2Vec2*				uVec2Ptr;
		Node*				uNodePtr;
		EObject*			uEObjectPtr;
		CControllerKeySet*	uKeySetPtr;
		CAIAttack*			uAIAttack;
		CAIMove*			uAIMove;
		CStats*				uStats;
		Rect*				uRect;
		const std::pair<int, int>*	uCommandPair;
		const std::pair<int, Action*>* uActionPair;
	};

	bool	m_SelfDestroy;
	int		m_TypeFlag;

#pragma region Vec3
	// Pass [selfDestroy] as [true] if you want to delete the [vec] when it's done.
	// The [vec] pointer will be directly referenced by this UParam's Object.
	UParam(b2Vec3* vec, bool selfDestroy)
	{
		uVec3Ptr = vec;
		m_SelfDestroy = selfDestroy;
		m_TypeFlag = UPARAM_TYPE_VEC3;
	}

	UParam(b2Vec3 vec)
	{
		uVec3Ptr = new b2Vec3(vec);
		m_SelfDestroy = true;
		m_TypeFlag = UPARAM_TYPE_VEC3;
	}
#pragma endregion

#pragma region Vec2
	// Pass [selfDestroy] as [true] if you want to delete the [vec] when it's done.
	// The [vec] pointer will be directly referenced by this UParam's Object.
	UParam(b2Vec2* vec, bool selfDestroy)
	{
		uVec2Ptr = vec;
		m_SelfDestroy = selfDestroy;
		m_TypeFlag = UPARAM_TYPE_VEC3;
	}

	UParam(b2Vec2 vec)
	{
		uVec2Ptr = new b2Vec2(vec);
		m_SelfDestroy = true;
		m_TypeFlag = UPARAM_TYPE_VEC3;
	}
#pragma endregion

#pragma region Node
	// Node is self-destroy type.
	UParam(Node* node)
	{
		uNodePtr = node;
		m_SelfDestroy = false;
		m_TypeFlag = UPARAM_TYPE_NODE;
	}
#pragma endregion

#pragma region CControllerKeySet
	// Pass [selfDestroy] as [true] if you want to delete the [keySet] when it's done.
	// The [keySet] pointer will be directly referenced by this UParam's Object.
	UParam(CControllerKeySet* keySet, bool selfDestroy)
	{
		this->uKeySetPtr = keySet;
		m_SelfDestroy = selfDestroy;
		m_TypeFlag = UPARAM_TYPE_KEYSET;
	}

	UParam(CControllerKeySet keySet)
	{
		this->uKeySetPtr = new CControllerKeySet(keySet);
		m_SelfDestroy = true;
		m_TypeFlag = UPARAM_TYPE_KEYSET;
	}
#pragma endregion

#pragma region CAIAttack
	UParam(CAIAttack* const param)
	{
		uAIAttack = param;
		m_TypeFlag = UPARAM_TYPE_CAIATTACK;
		m_SelfDestroy = false;
	}
#pragma endregion

#pragma region CAIMove
	UParam(CAIMove* const param)
	{
		uAIMove = param;
		m_TypeFlag = UPARAM_TYPE_CAIMOVE;
		m_SelfDestroy = false;
	}
#pragma endregion

#pragma region CStats
	// param will not be destroyed after this method
	UParam(CStats* param)
	{
		uStats = param;
		m_TypeFlag = UPARAM_TYPE_CSTATS;
		m_SelfDestroy = false;
	}
#pragma endregion

#pragma region CommandPair
	UParam(const int& command, const int& commandGivenInCase)
	{
		uCommandPair = new const std::pair<int, int>(command, commandGivenInCase);
		m_TypeFlag = UPARAM_TYPE_COMMAND_PAIR;
		m_SelfDestroy = true;
	}
#pragma endregion

#pragma region ActionPair
	UParam(const int& command, Action* const action)
	{
		uActionPair = new const std::pair<int, Action*>(command, action);
		m_TypeFlag = UPARAM_TYPE_ACTION_PAIR;
		m_SelfDestroy = true;
	}
#pragma endregion

#pragma region Rect
	UParam(Rect param)
	{
		uRect = new Rect(param);
		m_TypeFlag = UPARAM_TYPE_RECT;
		m_SelfDestroy = true;
	}
#pragma endregion

#pragma region EObject
	UParam(EObject* param, bool isAutodestroy = false)
	{
		uEObjectPtr = param;
		m_TypeFlag = UPARAM_TYPE_EOBJECT;
		m_SelfDestroy = isAutodestroy;
	}
#pragma endregion

#pragma region UParam Copy-constructor and Destructor
	UParam(const UParam& copy)
	{
		uReference	= copy.uReference;
		m_TypeFlag	= copy.m_TypeFlag;

		// The copy-one is not allowed to destroy the data.
		m_SelfDestroy = false;
	}

	~UParam()
	{
		if (m_SelfDestroy)
		{
			switch (m_TypeFlag)
			{
			case UPARAM_TYPE_VEC3:
			case UPARAM_TYPE_VEC2_CO_MOVE_VELO:
				CC_SAFE_DELETE(uVec3Ptr);
				break;

			case UPARAM_TYPE_VEC2_CO_MOVE_CLIMB_VELO:
			case UPARAM_TYPE_VEC2_CO_MOVE_JUMP_VELO:
				CC_SAFE_DELETE(uVec2Ptr);
				break;

			case UPARAM_TYPE_COMMAND_PAIR:
				CC_SAFE_DELETE(uCommandPair);
				break;

			case UPARAM_TYPE_ACTION_PAIR:
				CC_SAFE_DELETE(uActionPair);
				break;

			case UPARAM_TYPE_RECT:
				CC_SAFE_DELETE(uRect);
				break;

			// Node is self-destroy type.
			//case UPARAM_TYPE_NODE:
				//CC_SAFE_DELETE(uNodePtr);
				//break;

			case UPARAM_TYPE_KEYSET:
				CC_SAFE_DELETE(uKeySetPtr);
				break;

			default:
				break;
			}
		}
	}
#pragma endregion
};

#endif // UParam_h__