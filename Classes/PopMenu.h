#ifndef __PopMenu__
#define __PopMenu__
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class PopMenu : public Node
{
public:
	bool init();
	CREATE_FUNC(PopMenu);
	void selectedItemEvent(Ref *pSender, ListView::EventType type);
	void selectedEvent(Ref* pSender,CheckBox::EventType type);

	void addButton(const std::string& name, std::function<void()> callback = nullptr); 
	void addSlider(const std::string& name);
	void addCheckBox(const std::string& name);
	void sliderEvent(Ref *pSender, Slider::EventType type);
	CC_SYNTHESIZE(Vec2, _pos, Pos);
	CC_SYNTHESIZE(float, _popTime, PopTime);
	CC_SYNTHESIZE_READONLY(bool, _isEntering, IsEntering);
	CC_SYNTHESIZE_READONLY(bool, _isEntered, IsEntered);
	CC_SYNTHESIZE(bool, _isPopDown, IsPopDown);
	CC_SYNTHESIZE(int, _margin, Margin);
	CC_SYNTHESIZE(int, _maxButtonShown, MaxButtonShown);
	CC_SYNTHESIZE(float, _maxHeight, MaxHeight);
	CC_SYNTHESIZE(float, _maxWidth, MaxWidth);
	void popExit();
	void popEnter();
	void popToggle();
	void setCallback(const std::string& name, std::function<void()> callback);
	const Size& getListViewContentSize() const;
	void reName(const std::string& oldName, const std::string& newName);
	int getButtonCount();
	void updateSize(const Size& size);
private:
	ListView* _listView;
	Layout* _layout;
	std::unordered_map<std::string, std::function<void()>> _callbacks;
	float _buttonScale;
	int _titleFontSize;
	
//	Vec2 _pos;
};

#endif