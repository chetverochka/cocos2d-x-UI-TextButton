#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Definitions.h"

class TextButton : public cocos2d::ui::Button {
protected:
	std::string m_text;
	std::string m_normalSpriteName;
	std::string m_selectedSpriteName;
	std::string m_fontPath;
	cocos2d::Rect m_scale9Rect;
	cocos2d::Label* m_titleLabel;
	std::function<void(TextButton*)> m_callbackDefault;
	std::function<void()> m_callback;
private:
	bool init(std::string text) {
		m_text = text;
		m_normalSpriteName = "Scale9Button_normal.png";
		m_selectedSpriteName = "Scale9Button_selected.png";
		m_scale9Rect = cocos2d::Rect(40, 40, 150, 150);
		m_fontPath = FONTPATH_GOLDEN;

		if (!cocos2d::ui::Button::init(m_normalSpriteName, m_selectedSpriteName, "", cocos2d::ui::Widget::TextureResType::LOCAL)) {
			return false;
		}

		m_callback = nullptr;
		m_callbackDefault = nullptr;

		setScale9Enabled(true);
		setCapInsets(m_scale9Rect);
		addTouchEventListener(CC_CALLBACK_2(TextButton::buttonCallback, this));

		initTitleLabel();

		return true;
	}
	void initTitleLabel() {
		m_titleLabel = cocos2d::Label::createWithBMFont(m_fontPath, m_text);

		if (!m_titleLabel)
			return;

		setTitleLabel(m_titleLabel);
		setTitleText(m_text);
		resetTitleSize();
	}
	void resetTitleSize() {
		if (!m_titleLabel) 
			return;
		
		float kHeight = 0.6;
		float kWidth = 0.8;

		cocos2d::Size buttonSize = getContentSize();

		float scale = (buttonSize.height * kHeight) / m_titleLabel->getBoundingBox().size.height;
		m_titleLabel->setScale(scale);

		cocos2d::Size labelSize = m_titleLabel->getBoundingBox().size;

		if (labelSize.width > buttonSize.width * kWidth) {
			scale = (buttonSize.width * kWidth) / labelSize.width;
			m_titleLabel->setScale(scale);
		}
	}
public:
	static TextButton* create(std::string text) {
		TextButton* btn = new (std::nothrow) TextButton();
		if (btn && btn->init(text)) {
			btn->autorelease();
			return btn;
		}

		CC_SAFE_DELETE(btn);
		return nullptr;
	}
	void setContentSize(const cocos2d::Size& size) override {
		cocos2d::ui::Button::setContentSize(size);
		resetTitleSize();

		if (m_titleLabel) {
			float marginBottom = 0.05f;
			cocos2d::Size labelSize = m_titleLabel->getBoundingBox().size;
			m_titleLabel->setPositionY(m_titleLabel->getPositionY() + (labelSize.height * marginBottom));
		}
	}

	void setCallback(std::function<void(TextButton*)> defaultCallback) {
		m_callbackDefault = defaultCallback;
	}
	void setCallback(std::function<void()> callback) {
		m_callback = callback;
	}
protected:
	virtual void buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
			onTouchBegan();
		}
		else if (type == cocos2d::ui::Widget::TouchEventType::CANCELED) {
			onTouchCancelled();
		}
		else if (type == cocos2d::ui::Widget::TouchEventType::MOVED) {
			onTouchMoved();
		}
		else if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
			onTouchEnded();
			if (m_callback) {
				m_callback();
			}
			if (m_callbackDefault) {
				m_callbackDefault(this);
			}
		}
		else {
			// ...
		}
	}

	virtual void onTouchBegan() {}
	virtual void onTouchEnded() {}
	virtual void onTouchCancelled() {}
	virtual void onTouchMoved() {}
};