#include "Utils.h"
#include "tinyxml2\tinyxml2.h"
#include "Entity\EObject.h"

vector<ActionInfomation*>	initialActionInformaiton(const char* name) 
{
	vector<ActionInfomation*> result;

	// Read xml file.
	FILE* f = fopen(name, "rb");
	tinyxml2::XMLDocument l_xmlDocument;
	if (f != nullptr)
	{
		if (l_xmlDocument.LoadFile(f) != tinyxml2::XMLError::XML_NO_ERROR)
			return result;
	}

	// Read the plist files for the animation options.
	vector<PlistOption*>	l_vectorPlistOption;
	tinyxml2::XMLNode*		l_xmlNode = l_xmlDocument.FirstChild();
	tinyxml2::XMLElement*	l_xmlElement = l_xmlNode->FirstChildElement(XML_ANIMATION_ELEM_ANI);

	while (l_xmlElement != nullptr)
	{
		l_vectorPlistOption.push_back(readAnimatePList(l_xmlElement));
		l_xmlElement = l_xmlElement->NextSiblingElement(XML_ANIMATION_ELEM_ANI);
	}
	fclose(f);

	// Create animation(s) from read frames then push into result.
	for (size_t i = 0; i < l_vectorPlistOption.size(); i++)
	{
		ActionInfomation* animation = createAnimation(l_vectorPlistOption[i]);
		result.push_back(animation);
	}
	return result;
}

PlistOption* readAnimatePList(tinyxml2::XMLElement* element)
{
	PlistOption* l_currentPlistOption = new PlistOption();

	if (l_currentPlistOption != nullptr)
	{
		l_currentPlistOption->textureFileName = element->Attribute("TextureFileName");
		l_currentPlistOption->plist = element->Attribute("Plist");
		l_currentPlistOption->spriteFrameName = element->Attribute("SpriteFrameName");
		element->QueryIntAttribute("nFrames", &l_currentPlistOption->nFrames);
		element->QueryFloatAttribute("frameTime", &l_currentPlistOption->frameTimes);
		element->QueryFloatAttribute("anchorX", &l_currentPlistOption->anchorPoint.x);
		element->QueryFloatAttribute("anchorY", &l_currentPlistOption->anchorPoint.y);
	}

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
		l_currentPlistOption->plist.getCString(), 
		l_currentPlistOption->textureFileName.getCString());

	return l_currentPlistOption;
}

ActionInfomation* createAnimation(PlistOption* option)
{
	char file[100] = { 0 };
	cocos2d::Vector<cocos2d::SpriteFrame *> frames;

	for (size_t j = 0; j < (size_t)option->nFrames; j++)
	{
		sprintf(file, option->spriteFrameName.getCString(), (int)j);
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(file);
		frames.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(frames, option->frameTimes);

	Animate* animate = Animate::create(animation);
	// Mark as this object will not be auto-release.
	// Call release() at once to release it.
	animate->retain();

	return new ActionInfomation(animate, option->anchorPoint);
}

int switchTagIDBullet(const char* InputData)
{
	if (InputData == string("Type1"))
		return 1;
	if (InputData == string("Type2"))
		return 2;
};

char* switchLevelBulletFormIntToString(int level)
{
	if (level == 1)
		return "Level1";
	if (level == 2)
		return "Level2";
	if (level == 3)
		return "Level3";

};

int switchTagIDLevelBullet(const char* InputData)
{
	if (InputData == string("Level1"))
		return 1;
	if (InputData == string("Level2"))
		return 2;
	if (InputData == string("Level3"))
		return 3;
};

const char* switchIdBulletToLinkSource(int idBullet)
{
	switch (idBullet)
	{
	case 1:
	case 2:
	case 3:
		return XML_ANIMATION_SKILL_DARK_BLUE;
	default:
		break;
	}
	return nullptr;

};

// why alway me

#pragma region ERIC
// 25-05-2015
//NOTE: create vector action information from file
//vector<ActionInfomation*>	createActionInformation(String name)
//{
//	vector<ActionInfomation*> result;
//
//	// Read xml file.
//	String l_xmlLink  = name;
//	String l_pngLink = name;
//	String l_plistLink = name;
//
//	l_xmlLink.appendWithFormat(".xml");
//	l_pngLink.appendWithFormat(".png");
//	l_plistLink.appendWithFormat(".plist");
//
//	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(l_plistLink.getCString(), l_pngLink.getCString());
//
//	FILE* f = fopen(l_xmlLink.getCString(), "rb");
//	tinyxml2::XMLDocument l_xmlDocument;
//	if (f != nullptr)
//	{
//		if (l_xmlDocument.LoadFile(f) != tinyxml2::XMLError::XML_NO_ERROR)
//			return result;
//	}
//
//	// Read the plist files for the animation options.
//	vector<SpriteInformation*>	l_vectorSpriteInformation;
//	tinyxml2::XMLNode*		l_xmlNode = l_xmlDocument.FirstChild();
//	tinyxml2::XMLElement*	l_xmlElement = l_xmlNode->FirstChildElement(XML_ANIMATION_ELEM_ANI);
//
//	while (l_xmlElement != nullptr)
//	{
//		l_vectorSpriteInformation.push_back(createSpriteInformation(l_xmlElement));
//		l_xmlElement = l_xmlElement->NextSiblingElement(XML_ANIMATION_ELEM_ANI);
//	}
//	fclose(f);
//
//	// Create animation(s) from read frames then push into result.
//	for (size_t i = 0; i < l_vectorSpriteInformation.size(); i++)
//	{
//		ActionInfomation* animation = createActionInformation(l_vectorSpriteInformation[i]);
//		result.push_back(animation);
//	}
//	return result;
//}

map<int,ActionInfomation*>	createActionInformation(String name)
{
	map<int, ActionInfomation*> result;

	// Read xml file.
	String l_xmlLink  = name;
	String l_pngLink = name;
	String l_plistLink = name;

	l_xmlLink.appendWithFormat(".xml");
	l_pngLink.appendWithFormat(".png");
	l_plistLink.appendWithFormat(".plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(l_plistLink.getCString(), l_pngLink.getCString());

	FILE* f = fopen(l_xmlLink.getCString(), "rb");
	tinyxml2::XMLDocument l_xmlDocument;
	if (f != nullptr)
	{
		if (l_xmlDocument.LoadFile(f) != tinyxml2::XMLError::XML_NO_ERROR)
			return result;
	}

	// Read the plist files for the animation options.
	vector<SpriteInformation*>	l_vectorSpriteInformation;
	tinyxml2::XMLNode*		l_xmlNode = l_xmlDocument.FirstChild();
	tinyxml2::XMLElement*	l_xmlElement = l_xmlNode->FirstChildElement(XML_ANIMATION_ELEM_ANI);

	while (l_xmlElement != nullptr)
	{
		l_vectorSpriteInformation.push_back(createSpriteInformation(l_xmlElement));
		l_xmlElement = l_xmlElement->NextSiblingElement(XML_ANIMATION_ELEM_ANI);
	}
	fclose(f);

	// Create animation(s) from read frames then push into result.
	for (size_t i = 0; i < l_vectorSpriteInformation.size(); i++)
	{
		ActionInfomation* animation = createActionInformation(l_vectorSpriteInformation[i]);
		result.insert(std::pair<int, ActionInfomation*>(animation->animateKey, animation));
	}
	return result;
}


//NOTE: read sprite information from element into object (not autorelease)
SpriteInformation*			createSpriteInformation(tinyxml2::XMLElement* element)
{
	SpriteInformation* l_currentSpriteInformation = new SpriteInformation();

	if (l_currentSpriteInformation != nullptr)
	{
		l_currentSpriteInformation->Name = element->Attribute("Name");
		l_currentSpriteInformation->Path = element->Attribute("Path");
		element->QueryIntAttribute("Key", &l_currentSpriteInformation->Key);
		element->QueryIntAttribute("nFrames", &l_currentSpriteInformation->nFrames);
		element->QueryIntAttribute("From", &l_currentSpriteInformation->From);
		element->QueryIntAttribute("To", &l_currentSpriteInformation->To);
		element->QueryFloatAttribute("frameTime", &l_currentSpriteInformation->frameTime);
		element->QueryFloatAttribute("anchorPointX", &l_currentSpriteInformation->anchorPoint.x);
		element->QueryFloatAttribute("anchorPointY", &l_currentSpriteInformation->anchorPoint.y);
	}

	return l_currentSpriteInformation;
}
//NOTE: create a object from spriteInformation
ActionInfomation*			createActionInformation(SpriteInformation* information)
{
	char file[100] = { 0 };
	Vector<SpriteFrame *> frames;

	for (size_t j = information->From; j <= (size_t)information->To; j++)
	{
		sprintf(file, information->Path.getCString(), (int)j);
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(file);
		frames.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(frames, information->frameTime);

	Animate* animate = Animate::create(animation);
	// Cheat for change animation
	// animate->setDuration(information->frameTime * (information->nFrames + 1));
	// Mark as this object will not be auto-release.
	// Call release() at once to release it.
	animate->retain();

	return new ActionInfomation(information->Key, animate, information->anchorPoint);
}
//NOTE: create a bullet info
BulletInfo		createBulletInfo(tinyxml2::XMLElement* element)
{
	BulletInfo* l_currentBulletInfo = new BulletInfo();

	if (l_currentBulletInfo != nullptr)
	{
		element->QueryIntAttribute("type", &l_currentBulletInfo->type);
		element->QueryFloatAttribute("runTimeEffectStart", &l_currentBulletInfo->runTime.x);
		element->QueryFloatAttribute("timeLive", &l_currentBulletInfo->runTime.y);
		element->QueryFloatAttribute("X", &l_currentBulletInfo->shootSpot.x);
		element->QueryFloatAttribute("Y", &l_currentBulletInfo->shootSpot.y);
		l_currentBulletInfo->resource = element->Attribute("resource");
	}

	return *l_currentBulletInfo;
}

vector<BulletInfo>	createBulletInfoFromFile(const char* filePath)
{
	//read data from xml
	vector<BulletInfo> l_vectorBulletInfo;
	FILE* file = fopen(filePath, "rb");
	tinyxml2::XMLDocument l_xmlDocument;
	if (file == nullptr)
		return l_vectorBulletInfo;
	if (l_xmlDocument.LoadFile(file) != tinyxml2::XMLError::XML_NO_ERROR)
		return l_vectorBulletInfo;


	tinyxml2::XMLNode*		l_xmlNode = l_xmlDocument.FirstChild();
	tinyxml2::XMLElement*	l_xmlElement = l_xmlNode->FirstChildElement(XML_BULLETINFO_ELEM_INFO);

	while (l_xmlElement != nullptr)
	{
		l_vectorBulletInfo.push_back(createBulletInfo(l_xmlElement));
		l_xmlElement = l_xmlElement->NextSiblingElement();
	}

	fclose(file);
	return l_vectorBulletInfo;
}

bool isStringEqual(const char* str1, const char* str2)
{
	bool result = strcmp(str1, str2) == 0;
	return result;
}

#pragma endregion
