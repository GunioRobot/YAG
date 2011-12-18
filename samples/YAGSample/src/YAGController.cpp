//
//  YAGController.cpp
//  YAG
//
//  Created by Q on 10/03/2011.
//  Copyright 2011 . All rights reserved.
//

#include "cinder/Utilities.h"
#include "YAGController.h"

using namespace ci;
using namespace ci::app;
using namespace std;


ColorA  YAGController::m_controlBgColor             = ColorA(0.0f, 0.0f, 0.0f, 0.5f);
ColorA  YAGController::m_controlBorderColor         = ColorA(0.0f, 0.0f, 0.0f, 0.9f);
ColorA  YAGController::m_sliderBgColor              = ColorA(1.0f, 1.0f, 1.0f, 0.05f);
ColorA  YAGController::m_sliderFgColor              = ColorA(0.85f, 0.035f, 0.2f, 0.3f);
ColorA  YAGController::m_timelineBgColour           = ColorA(0.0f, 0.0f, 0.0f, 0.4f);
ColorA  YAGController::m_fontColor                  = ColorA(1.0f, 1.0f, 1.0f, 1.0f);
ColorA  YAGController::m_timelineBarColor           = ColorA(1.0f, 1.0f, 1.0f, 0.4f);
ColorA  YAGController::m_timelineBeatColor          = ColorA(1.0f, 1.0f, 1.0f, 0.15f);
ColorA  YAGController::m_timelineStepColor          = ColorA(1.0f, 1.0f, 1.0f, 0.05f);
ColorA  YAGController::m_timelineIndicatorColor     = ColorA(0.85f, 0.035f, 0.2f, 1.0f);
ColorA  YAGController::m_keyframeColor              = ColorA(1.0f, 1.0f, 1.0f, 1.0f);
Font    YAGController::m_textFont                   = Font(loadResource("HelveticaNeueRoman.ttf"), 10);
int     YAGController::m_timelineOffsetInPixels     = 70;
Rectf   YAGController::m_controlPadding             = Rectf(4, 2, 4, 4);
Vec2f   YAGController::m_sliderSize                 = Vec2f(200, 25);
Vec2f   YAGController::m_sliderSizeSmall            = Vec2f(200, 20);
Vec2f   YAGController::m_toggleControlSize          = Vec2f(17, 17);
int     YAGController::m_handleHeight               = 17;
int     YAGController::m_timelineWidth              = 700;


YAGController::YAGController() {
}


YAGController::~YAGController() {}


YAGController::YAGController(App* app) {
    init(app);
}


void YAGController::init(App* app) {
    m_selectedControl   = NULL;
    m_cbMouseDown       = app->registerMouseDown( this, &YAGController::onMouseDown );
    m_cbMouseUp         = app->registerMouseUp( this, &YAGController::onMouseUp );
    m_cbMouseDrag       = app->registerMouseDrag( this, &YAGController::onMouseDrag );
    m_cbMouseWheel      = app->registerMouseWheel( this, &YAGController::onMouseWheel );
    m_cbKeyDown         = app->registerKeyDown( this, &YAGController::onKeyDown );

    m_isPlaying         = false;
    m_isEnabled         = true;
    m_frameLastTime     = 0;
    m_timelineStepWidth = 10;
    m_tRender           = 0;
}


bool YAGController::onMouseDown(ci::app::MouseEvent event) {
    if (!m_isEnabled) return false;

    vector<YAGControl*>::iterator it = m_controls.begin();
	while(it != m_controls.end()) {
        YAGControl *control = *it++;
		if (control->onMouseDown(event)) {
			m_selectedControl = control;
			return true;
		}
	}
    return false;
}


bool YAGController::onMouseUp(ci::app::MouseEvent event) {
    if (!m_isEnabled) return false;
    if (m_selectedControl != NULL) {
		m_selectedControl->onMouseUp(event);
		m_selectedControl = NULL;
		return true;
	}
    return false;
}


bool YAGController::onMouseDrag(ci::app::MouseEvent event) {
    if (!m_isEnabled) return false;

	if (m_selectedControl) {
		m_selectedControl->onMouseDrag(event);
		return true;
	}
    return false;
}


bool YAGController::onMouseWheel(ci::app::MouseEvent event) {
    if (event.isAltDown()) {
        m_timelineStepWidth += event.getWheelIncrement();
        m_timelineStepWidth = math<float>::max(5.0f, math<float>::min(m_timelineStepWidth, 50.0f));
        return true;
    }
    return false;
}


bool YAGController::onKeyDown(KeyEvent event) {
    if (!m_isEnabled) return false;
    switch(event.getChar()) {
        case ' ' :
            m_isPlaying = !m_isPlaying;
//            m_timeline->m_dragKeyframe = NULL;                                          // reset drag keyframe

            break;
    }

    return false;
}

void YAGController::update() {
    if (isPlaying()){
        m_tRender += getElapsedSeconds() - m_frameLastTime;             // update tRender
        vector<YAGControl*>::iterator it = m_controls.begin();          // update variables
        while(it != m_controls.end())
            (*it++)->updateValues();
    }


    m_frameLastTime = getElapsedSeconds();

}

void YAGController::draw() {
    gl::setMatricesWindow(getWindowSize());
	gl::disableDepthRead();
	gl::disableDepthWrite();
	gl::enableAlphaBlending();

    vector<YAGControl*>::iterator it = m_controls.begin();
	while(it != m_controls.end())
        (*it++)->draw();


    gl::color(YAGController::m_controlBgColor);

    Vec2f buttonSize(55,25);
    Rectf m_playButtonArea(0, 0, buttonSize.x, buttonSize.y);
    Rectf m_stopButtonArea(m_playButtonArea.x2 + 1, 0, m_playButtonArea.x2 + 1 + buttonSize.x, buttonSize.y);
    Rectf m_tRenderArea(m_stopButtonArea.x2 + 1, 0, m_stopButtonArea.x2 + 1 + 90, buttonSize.y);

    gl::drawSolidRect(m_playButtonArea);
    gl::drawSolidRect(m_stopButtonArea);
    gl::drawSolidRect(m_tRenderArea);

    gl::drawString(toString(m_tRender),
                   Vec2f(m_tRenderArea.x1 + 5, m_tRenderArea.y1 + 11),
                   YAGController::m_fontColor, YAGController::m_textFont);

    gl::drawString("PLAY",
                   Vec2f(m_playButtonArea.x1 + 16, m_playButtonArea.y1 + 11),
                   YAGController::m_fontColor, YAGController::m_textFont);

    gl::drawString("STOP",
                   Vec2f(m_stopButtonArea.x1 + 16, m_stopButtonArea.y1 + 11),
                   YAGController::m_fontColor, YAGController::m_textFont);


    gl::disableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();
	gl::color(ColorA(1,1,1,1));
}


void YAGController::addControl(YAGControl *control) {
    m_controls.push_back(control);
}



void YAGController::shiftTimeline(double offset) {
    m_tRender += offset;
    m_tRender = max<float>(0.0f, m_tRender);
}


