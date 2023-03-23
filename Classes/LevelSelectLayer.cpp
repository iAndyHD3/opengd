#include "LevelSelectLayer.h"
#include "GJGameLevel.h"
#include "MenuItemSpriteExtra.h"
#include "AudioEngine.h"
#include "PlayLayer.h"
#include "GameToolbox.h"
#include "MenuLayer.h"
#include "LevelPage.h"

USING_NS_AX;


Scene* LevelSelectLayer::scene(int page)
{
	auto scene = Scene::create();
	scene->addChild(LevelSelectLayer::create(page));
	return scene;
}

LevelSelectLayer* LevelSelectLayer::create(int page) {
	LevelSelectLayer* pRet = new LevelSelectLayer();
	if (pRet->init(page)) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool LevelSelectLayer::init(int page)
{

	if(!Layer::init()) return false;

	auto director = Director::getInstance();
	auto winSize = director->getWinSize();

	_background = Sprite::create("GJ_gradientBG.png");
	_background->setAnchorPoint({0.0f, 0.0f});
	addChild(_background, -2);

	_background->setScaleX((winSize.width + 10.0f) / _background->getTextureRect().size.width);
	_background->setScaleY((winSize.height + 10.0f) / _background->getTextureRect().size.height);
	_background->setPosition({-5.0f, -5.0f});
	_background->setColor({0x28, 0x7D, 0xFF});
	_ground = GroundLayer::create(1);
	_ground->setPositionY(-25.f);
	addChild(_ground, -1);

	GameToolbox::createCorners(this, false, false, true, true);
	
	constexpr auto levelData = std::to_array<std::tuple<const char*, const char*, int>>({
		{ "Stereo Madness", "Robtop", 1 },
		{ "Back on Track", "Robtop", 2 },
		{ "Polargeist", "Robtop", 3 },
		{ "Dry Out", "Robtop", 4 },
		{ "Base After Base", "Robtop", 5 },
		{ "Cant Let Go", "Robtop", 6 },
		{ "Jumper", "Robtop", 7 },
		{ "Time Machine", "Robtop", 8 },
		{ "Cycles", "Robtop", 9 },
		{ "xStep", "Robtop", 10 },
		{ "Clutterfunk", "Robtop", 11 },
		{ "Theory Of Everything", "Robtop", 12 },
		{ "Electroman Adventures", "Robtop", 13 },
		{ "Clubstep", "Robtop", 14 },
		{ "Electrodynamix", "Robtop", 15 },
		{ "Hexagon Force", "Robtop", 16 },
		{ "Blast Processing", "Robtop", 17 },
		{ "Theory Of Everything 2", "Robtop", 18 },
		{ "Geometrical Dominator", "Robtop", 19 },
		{ "Deadlocked", "Robtop", 20 },
		{ "Fingerdash", "Robtop", 21 },
		{ "Performance Test", "OpenGD", 22 }
	});
	
	//TODO: add getters on level page because they are actually owning the stuff

	std::vector<Layer*> layers;
	layers.reserve(levelData.size());
	
	for (const auto [name, creator, id] : levelData)
	{
		auto level = GJGameLevel::createWithMinimumData(name, creator, id);
		layers.push_back(LevelPage::create(level));
	}
	_bsl = BoomScrollLayer::create(layers, page);
	addChild(_bsl);

	auto btnMenu = Menu::create();
	addChild(btnMenu, 5);

	//bool controller = PlatformToolbox::isControllerConnected();
	bool controller = false;

	auto left =
		Sprite::createWithSpriteFrameName(controller ? "controllerBtn_DPad_Left_001.png" : "navArrowBtn_001.png");
	if (!controller) left->setFlippedX(true);

	MenuItemSpriteExtra* leftBtn = MenuItemSpriteExtra::create(left, [this](Node* btn) {
		_bsl->changePageLeft();
	});
	btnMenu->addChild(leftBtn);

	//leftBtn->setScale(2.0f);
	leftBtn->setPosition(btnMenu->convertToNodeSpace({ 25.0f, winSize.height / 2 }));

	auto right = Sprite::createWithSpriteFrameName(controller ? "controllerBtn_DPad_Right_001.png" : "navArrowBtn_001.png");

	MenuItemSpriteExtra* rightBtn = MenuItemSpriteExtra::create(right, [this](Node* btn) {
		_bsl->changePageRight();
	});
	btnMenu->addChild(rightBtn);

	//right->setScale(2.0f);
	rightBtn->setPosition(btnMenu->convertToNodeSpace({ winSize.width - 25.0f, winSize.height / 2 }));

	auto back = Sprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
		MenuItemSpriteExtra* backBtn =
			MenuItemSpriteExtra::create(back, [](Node* btn) { 
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
	});
	//backBtn->setScale(1.6f);

	Menu* backMenu = Menu::create();
	addChild(backMenu, 1);

	backMenu->addChild(backBtn);

	backMenu->setPosition({ 0 + 25.0f, winSize.height - 22.0f });

	// //GM->0x298 = 0;

	auto infoMenu = Menu::create();
	addChild(infoMenu);

	Sprite* info = Sprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	MenuItemSpriteExtra* infoBtn = MenuItemSpriteExtra::create(info, [&](Node* btn) {
		//auto a = GJMoreGamesLayer::create();
		//addChild(a);
	});
	infoMenu->addChild(infoBtn, 1);

	infoMenu->setPosition({ winSize.width - 20.0f, winSize.height - 20.0f });

	auto listener = EventListenerKeyboard::create();

	// int currentlevel = 0;

	listener->onKeyPressed = [this](EventKeyboard::KeyCode code, Event*) {
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE) {
			auto scene = MenuLayer::scene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
		} else if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			_bsl->changePageLeft();
		} else if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			_bsl->changePageRight();
		} else if (code == EventKeyboard::KeyCode::KEY_SPACE) {
			// ax::AudioEngine::stopAll();
			// ax::AudioEngine::play2d("playSound_01.ogg", false, 0.2f);
			// auto currentLevelPage = dynamic_cast<LevelPage*>(bsl->_layers.at(bsl->_currentPage));
			// if (currentLevelPage) {
			// 	auto level = currentLevelPage->_level;
			// 	ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, PlayLayer::scene(level)));
			// 	LevelPage::replacingScene = true;
			// 	MenuLayer::music = false;
			// }
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//if (controller) GameToolbox::addBackButton(this, backBtn);
	return true;
}