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

		addChild(m_titleLabel);
		resetTitle();
	}
	void resetTitle() {
		if (!m_titleLabel) 
			return;
		cocos2d::Size buttonSize = getContentSize();

		float maxHeight = buttonSize.height * 0.66f;
		float maxWidth = buttonSize.width * 0.85f;


		m_titleLabel->setScale(1);
		float scale = maxWidth / m_titleLabel->getBoundingBox().size.width;
		m_titleLabel->setScale(scale);
		
		if (m_titleLabel->getBoundingBox().size.height > maxHeight) {
			m_titleLabel->setScale(1);
			scale = maxHeight / m_titleLabel->getBoundingBox().size.height;
			m_titleLabel->setScale(scale);
		}


		m_titleLabel->setPosition(buttonSize / 2);
		// отступ текста снизу для кривых шрифтов
		float marginBottom = 0.15f;
		m_titleLabel->setPositionY(m_titleLabel->getPositionY() + (m_titleLabel->getBoundingBox().size.height * marginBottom));
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
		resetTitle();
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

	virtual void onTouchBegan() {
		runAction(cocos2d::EaseBounceOut::create(cocos2d::ScaleTo::create(0.4f, 1.08)));
	}
	virtual void onTouchEnded() {
		runAction(cocos2d::EaseBounceOut::create(cocos2d::ScaleTo::create(0.4f, 1.f)));
	}
	virtual void onTouchCancelled() {
		runAction(cocos2d::EaseBounceOut::create(cocos2d::ScaleTo::create(0.4f, 1.f)));
	}
	virtual void onTouchMoved() {}
};
