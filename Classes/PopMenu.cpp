#include "PopMenu.h"
#include "PhysicsManager.h"


bool PopMenu::init()
{
	_listView = ListView::create();
	_listView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(PopMenu::selectedItemEvent, this));
	_listView->setBackGroundColor(Color3B::ORANGE);
//	_listView->setColor(Color3B::ORANGE);
//	_listView->setBackGroundColorOpacity(200);
	_listView->setBounceEnabled(true);
	_layout = Layout::create();
	_layout->addChild(_listView);
	_layout->setBackGroundColor(Color3B::BLUE);
	this->addChild(_layout);
	_buttonScale = 2.0f;
	_titleFontSize = 18;
	_pos = Vec2(0, 0);
	_popTime = 0.3f;
	_isEntered = false;
	_isEntering = false;
	_margin = 0;
	_maxButtonShown = 4;
	_maxHeight = 240.0f;
	_maxWidth = 1000;
	this->setVisible(false);
//	this->setContentSize(_listView->getContentSize());
	_isPopDown = true;
	return true;
}

void PopMenu::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
    switch (type)
    {
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_START:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            CC_UNUSED_PARAM(listView);
            CCLOG("select child start index = %ld", listView->getCurSelectedIndex());
            break;
        }
        case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END:
        {
            ListView* listView = static_cast<ListView*>(pSender);
            CC_UNUSED_PARAM(listView);
			ssize_t index = listView->getCurSelectedIndex();
            CCLOG("select child end index = %ld", index);
			auto item = listView->getItem(index);
			auto name = item->getChildren().at(0)->getName();
			if(_callbacks[name])
				_callbacks[name]();
			
//			PhysicsManager::getInstance()->setSideNum(listView->getCurSelectedIndex() + 3);
//			_sideNum = listView->getCurSelectedIndex() + 3;
            break;
        }
        default:
            break;
    }
}

void PopMenu::addButton(const std::string& name, std::function<void()> callback)
{
	_callbacks[name] = callback;
    Button* custom_button = Button::create("cocosui/backtotoppressed.png", "cocosui/backtotopnormal.png");
    custom_button->setName(name);
	custom_button->setTitleText(name);
	custom_button->setTitleFontName("fonts/Marker Felt.ttf");
    custom_button->setScale9Enabled(true);
    custom_button->setContentSize(custom_button->getContentSize() * _buttonScale);
	custom_button->setTitleFontSize(_titleFontSize);
	//auto func = [&](Ref* pSender, Widget::TouchEventType type){
	//	if(callback)
	//		callback();
	//};
	//custom_button->addTouchEventListener(func);
            
    Layout *custom_item = Layout::create();
	custom_item->setName(name);
    custom_item->setContentSize(custom_button->getContentSize());
    custom_button->setPosition(Vec2(custom_item->getContentSize().width / 2.0f, custom_item->getContentSize().height / 2.0f));
    custom_item->addChild(custom_button);           
    _listView->addChild(custom_item);

	updateSize(custom_item->getContentSize());
}

void PopMenu::updateSize(const Size& size)
{
	auto width = _listView->getContentSize().width>size.width?_listView->getContentSize().width:size.width;
	auto height =_listView->getContentSize().height + size.height;
	if(width <= _maxWidth && height <= _maxHeight)
	{
		_listView->setContentSize(Size(width, height));
		_layout->setContentSize(_listView->getContentSize());
	}
}

void PopMenu::addSlider(const std::string& name)
{
	float value = PhysicsManager::getInstance()->getPropertyByName(name);
	Vec2 range = PhysicsManager::getInstance()->getRangeByName(name);
	String* str = String::createWithFormat("%s:%.1f", name.c_str(), value);
	auto valueLabel = Text::create(str->getCString(),"",20);
//    auto valueLabel = Text::create("sth...","Arial",24);
//    valueLabel->setAnchorPoint(Vec2(0.5f, -1));

    // Create the slider
    Slider* slider = Slider::create();
	slider->setName(name);
    slider->loadBarTexture("cocosui/sliderTrack.png");
    slider->loadSlidBallTextures("cocosui/sliderThumb.png", "cocosui/sliderThumb.png", "");
    slider->loadProgressBarTexture("cocosui/sliderProgress.png");
    slider->setPosition(Vec2(slider->getLayoutSize()/2) + Vec2(0, 0));
	float percent = 100*(value - range.x)/(range.y - range.x);
	slider->setPercent((int)(percent));
    slider->addEventListener(CC_CALLBACK_2(PopMenu::sliderEvent, this));
	valueLabel->setPosition(Vec2(slider->getLayoutSize().width/2, valueLabel->getContentSize().height/2 + slider->getLayoutSize().height));
	Layout *custom_item = Layout::create();
	custom_item->setName(name);
	float width = valueLabel->getContentSize().width > slider->getLayoutSize().width?valueLabel->getContentSize().width:slider->getContentSize().width;
	float height = valueLabel->getContentSize().height + slider->getLayoutSize().height;
    custom_item->setContentSize(Size(width, height));
 //   custom_button->setPosition(Vec2(custom_item->getContentSize().width / 2.0f, custom_item->getContentSize().height / 2.0f));
	custom_item->addChild(valueLabel);
	custom_item->addChild(slider);

    _listView->addChild(custom_item);

	updateSize(custom_item->getContentSize());
}

void PopMenu::sliderEvent(Ref *pSender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
		auto name = slider->getName();
        int percent = slider->getPercent();
		Vec2 range = PhysicsManager::getInstance()->getRangeByName(name);
		Text* text = dynamic_cast<Text*>(slider->getParent()->getChildren().at(0));
		float value = (range.x + percent/100.0f*(range.y - range.x));
		text->setString(String::createWithFormat("%s:%.1f", name.c_str(),value)->getCString());
		PhysicsManager::getInstance()->setPropertyByName(name, value); 
//        _valueLabel->setString(String::createWithFormat("Percent %d", percent)->getCString());
    }
}

void PopMenu::addCheckBox(const std::string& name)
{      
	bool bval = PhysicsManager::getInstance()->getPropertyByNameBool(name);
    // Add the alert
    Text* alert = Text::create(name,"",20 );
//    alert->setColor(Color3B(159, 168, 176));
    alert->setPosition(Vec2(alert->getContentSize().width / 2.0f, alert->getContentSize().height / 2.0f));
           
        
    // Create the checkbox
    CheckBox* checkBox = CheckBox::create("cocosui/check_box_normal.png",
                                            "cocosui/check_box_normal_press.png",
                                            "cocosui/check_box_active.png",
                                            "cocosui/check_box_normal_disable.png",
                                            "cocosui/check_box_active_disable.png");
	checkBox->setName(name);
    checkBox->setPosition(Vec2(alert->getContentSize().width + checkBox->getContentSize().width / 2.0f, checkBox->getContentSize().height / 2.0f));
    checkBox->setSelectedState(bval);   
    checkBox->addEventListener(CC_CALLBACK_2(PopMenu::selectedEvent, this));
    

	Layout *custom_item = Layout::create();
	float height = alert->getContentSize().height > checkBox->getLayoutSize().height?alert->getContentSize().height:checkBox->getLayoutSize().height;
	float width = alert->getContentSize().width + checkBox->getLayoutSize().width;
    custom_item->setContentSize(Size(width, height));
	custom_item->addChild(alert);
	custom_item->addChild(checkBox);

	_listView->addChild(custom_item);

	updateSize(custom_item->getContentSize());
}

void PopMenu::selectedEvent(Ref* pSender,CheckBox::EventType type)
{
	std::string name = "";
	CheckBox* checkBox = nullptr;
    switch (type)
    {
        case CheckBox::EventType::SELECTED:
			checkBox = dynamic_cast<CheckBox*>(pSender);
			name = checkBox->getName();
			PhysicsManager::getInstance()->setPropertyByNameBool(name, true);
            break;
            
        case CheckBox::EventType::UNSELECTED:
			checkBox = dynamic_cast<CheckBox*>(pSender);
			name = checkBox->getName();
			PhysicsManager::getInstance()->setPropertyByNameBool(name, false);
        default:
            break;
    }
    
}

void PopMenu::popEnter()
{
	if(this->getIsEntered() || this->getIsEntering())
	{
		return;
	}
	auto height = _listView->getContentSize().height;
	auto dir = _isPopDown?1:-1;
	auto move = MoveBy::create(_popTime, dir*Vec2(0, -height - _margin));
	auto fuc =[&](){
		this->_isEntering = false;
		this->_isEntered = true;
		auto zOrder = this->getLocalZOrder();
		this->setLocalZOrder(zOrder - 10);
	}; 
	auto callback = CallFunc::create(fuc);
	auto seq = Sequence::create(move, callback, nullptr);
	_listView->runAction(seq);
	this->setVisible(true);
	this->_isEntering=true;
	auto zOrder = this->getLocalZOrder();
	this->setLocalZOrder(zOrder + 10);
}

void PopMenu::popExit()
{
	if(_isEntered)
	{
		auto height = _listView->getContentSize().height;
		auto dir = _isPopDown?1:-1;
		auto move = MoveBy::create(_popTime, dir*Vec2(0, height + _margin));
		auto fuc =[&](){
			this->_isEntering = false;
			this->_isEntered = false;
			this->setVisible(false);
		}; 
		auto callback = CallFunc::create(fuc);
		auto seq = Sequence::create(move, callback, nullptr);
		_listView->runAction(seq);

		this->_isEntering=true;
	}
}

void PopMenu::popToggle()
{
	if(!_isEntering)
	{
		if(_isEntered)
		{
			popExit();
		}
		else
		{
			popEnter();
		}	
	}

}

void PopMenu::setCallback(const std::string& name, std::function<void()> callback)
{
	_callbacks[name] = callback;
}

const Size& PopMenu::getListViewContentSize() const
{
	return _listView->getContentSize();
}

void PopMenu::reName(const std::string& oldName, const std::string& newName)
{
	auto layout =_listView->getChildByName(oldName);
	layout->setName(newName);
	auto item = dynamic_cast<Button*>(layout->getChildren().at(0));
	item->setName(newName);
	item->setTitleText(newName);
}

int PopMenu::getButtonCount()
{
	return _listView->getChildrenCount();
}