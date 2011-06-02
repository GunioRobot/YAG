//
//  YAGController.h
//  YAG
//
//  Created by Q on 10/03/2011.
//  Copyright 2011 . All rights reserved.
//

#pragma once

#include <vector>
#include "cinder/app/App.h"
#include "cinder/Text.h"

#include "YAGControl.h"



class YAGController {

public:
    YAGController();
    YAGController(ci::app::App* app);
    ~YAGController();
    
	void	init(ci::app::App* app);
    
    void    update();
    void    draw();
    
    void    enable() { m_isEnabled = true; };
    void    disable() { m_isEnabled = false; };
    bool    isEnabled() { return m_isEnabled; };
        
    bool	onMouseDown(ci::app::MouseEvent event);
	bool	onMouseUp(ci::app::MouseEvent event);
	bool	onMouseDrag(ci::app::MouseEvent event);
    bool    onMouseWheel(ci::app::MouseEvent event);
    bool	onKeyDown(ci::app::KeyEvent event);

    void    addControl(YAGControl *control);
    
    bool    isPlaying() { return m_isPlaying; }; 
    
    int     getTimelineStepWidth() { return m_timelineStepWidth; };             // the step width in pixels
    int     getTimelineBeatWidth() { return m_timelineStepWidth * 4; };         // the beat witdh in pixels
    
    double  tRender() { return m_tRender; }
    void    shiftTimeline(double offset);
    
    static ci::ColorA   m_controlBgColor;
    static ci::ColorA   m_controlBorderColor;
    static ci::ColorA   m_sliderBgColor;
    static ci::ColorA   m_sliderFgColor;
//    static ci::ColorA   m_sliderIndicatorColor;
    static ci::ColorA   m_timelineBgColour;
    static ci::ColorA   m_timelineBarColor;
    static ci::ColorA   m_timelineBeatColor;
    static ci::ColorA   m_timelineStepColor;
    static ci::ColorA   m_timelineIndicatorColor;
    static ci::ColorA   m_keyframeColor;
    static ci::ColorA   m_fontColor;
    static ci::Font     m_textFont;
    static ci::Font     m_textFontMini;
    static ci::Font     m_textFontBig;
    static ci::Rectf    m_controlPadding;
    static int          m_timelineOffsetInPixels;
    static ci::Vec2f    m_sliderSize;
    static ci::Vec2f    m_sliderSizeSmall;
    static ci::Vec2f    m_toggleControlSize;
    static int          m_handleHeight;
    static int          m_timelineWidth;

private:    
    bool            m_isEnabled;
    
    YAGControl*     m_selectedControl;
    
    ci::CallbackId  m_cbMouseDown;
    ci::CallbackId	m_cbMouseUp;
    ci::CallbackId  m_cbMouseDrag;
    ci::CallbackId  m_cbMouseWheel;
    ci::CallbackId  m_cbKeyDown;
    
    ci::Vec2f       m_position;
    
    std::vector<YAGControl*> m_controls;
    
    bool    m_isPlaying;
    double  m_frameLastTime;
    double  m_tRender;
    int     m_timelineStepWidth;
    
};

