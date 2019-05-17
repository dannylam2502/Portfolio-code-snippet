#ifndef __UTILS_H__
#define __UTILS_H__

#include "Config.h"
#include "tinyxml2/tinyxml2.h"
#include "cocos2d.h"
#include <vector>
#include <sstream>
#include <iostream>

//NOTE: returned objects are not auto release.
vector<ActionInfomation*>	initialActionInformaiton(const char* name);

//NOTE: returned objects are not auto release.
PlistOption*				readAnimatePList(tinyxml2::XMLElement* element);

//NOTE: returned objects are not auto release.
ActionInfomation*			createAnimation(PlistOption* option);

//NOTE: switch type form string to int
int							switchTagIDBullet(const char*);

//NOTE: switch level form int to string
char*						switchLevelBulletFormIntToString(int level);

//NOTE: switch ID level Bullet from string
int							switchTagIDLevelBullet(const char*);

//NOTE: switch IDBULLET to string source
const char*					switchIdBulletToLinkSource(int ID = 0);
#pragma region ERIC
//NOTE: create vector action information from file
map<int, ActionInfomation*>	createActionInformation(String name);
//NOTE: read sprite information from element into object (not autorelease)
SpriteInformation*			createSpriteInformation(tinyxml2::XMLElement* element);
//NOTE: create a object from spriteInformation
ActionInfomation*			createActionInformation(SpriteInformation* information);
//NOTE: read bullet info from element into object (not autorelease)
BulletInfo					createBulletInfo(tinyxml2::XMLElement* element);
vector<BulletInfo>			createBulletInfoFromFile(const char* filePath);
#pragma endregion

bool	isStringEqual(const char* str1, const char* str2);

template < class T >
const char* tostr(const T& content)
{
	ostringstream os;
	os << content;
	return os.str().c_str();
}

#include <utility>
#include <algorithm>
#include <initializer_list>

template <typename KeyType, typename FunPtrType, typename Comp>
FunPtrType Switch(Comp comp, const KeyType &value, std::initializer_list<std::pair<const KeyType, FunPtrType>> sws) {

	typedef std::pair<const KeyType &, FunPtrType> KVT;

	auto cmp = [&comp](const KVT &a, const KVT &b){ return comp(a.first, b.first); };

	auto val = KVT(value, FunPtrType());

	auto r = std::lower_bound(sws.begin(), sws.end(), val, cmp);

	if ((r != sws.end()) && (!cmp(val, *r))) {
		return r->second;
	} // else: not found
}

#endif // !__UTILS_H__
