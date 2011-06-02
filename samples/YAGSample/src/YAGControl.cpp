//
//  YAGControl.cpp
//  YAG2
//
//  Created by Q on 16/03/2011.
//  Copyright 2011 . All rights reserved.
//

#include <boost/algorithm/string.hpp>
#include "YAGController.h"
#include "YAGControl.h"

using namespace ci;
using namespace ci::app;
using namespace std;


YAGControl::YAGControl(YAGController *controller, float *var, string name, float min, float max, Vec2f pos) {       // FLOAT
    m_controlArea = Rectf(pos.x, 
                          pos.y, 
                          pos.x + YAGController::m_sliderSize.x + YAGController::m_controlPadding.x1 + YAGController::m_controlPadding.x2, 
                          pos.y + YAGController::m_sliderSize.y + YAGController::m_controlPadding.y1 + YAGController::m_controlPadding.y2 + YAGController::m_handleHeight );
    
    Rectf sliderArea    = Rectf(pos.x + YAGController::m_controlPadding.x1, 
                                pos.y + YAGController::m_controlPadding.y1 + YAGController::m_handleHeight, 
                                pos.x + YAGController::m_sliderSize.x + YAGController::m_controlPadding.x1, 
                                pos.y + YAGController::m_sliderSize.y + YAGController::m_controlPadding.y1 + YAGController::m_handleHeight);

    Rectf timelineArea  = Rectf(m_controlArea.x2, 
                                m_controlArea.y1,
                                m_controlArea.x2 + YAGController::m_timelineWidth, 
                                m_controlArea.y2);
    
    m_sliders.push_back( new YAGSlider(controller, var, min, max, sliderArea, timelineArea) );
    
    init(controller, name, FLOAT);
}


YAGControl::YAGControl(YAGController *controller, int *var, string name, float min, float max, Vec2f pos) {         // INT
    m_controlArea = Rectf(pos.x, 
                          pos.y, 
                          pos.x + YAGController::m_sliderSize.x + YAGController::m_controlPadding.x1 + YAGController::m_controlPadding.x2, 
                          pos.y + YAGController::m_sliderSize.y + YAGController::m_controlPadding.y1 + YAGController::m_controlPadding.y2 + YAGController::m_handleHeight );
    
    Rectf sliderArea    = Rectf(pos.x + YAGController::m_controlPadding.x1, 
                                pos.y + YAGController::m_controlPadding.y1 + YAGController::m_handleHeight, 
                                pos.x + YAGController::m_sliderSize.x + YAGController::m_controlPadding.x1, 
                                pos.y + YAGController::m_sliderSize.y + YAGController::m_controlPadding.y1 + YAGController::m_handleHeight);
    
    Rectf timelineArea  = Rectf(m_controlArea.x2, 
                                m_controlArea.y1,
                                m_controlArea.x2 + YAGController::m_timelineWidth, 
                                m_controlArea.y2);
    
    m_sliders.push_back( new YAGSlider(controller, var, min, max, sliderArea, timelineArea) );
    
    init(controller, name, INT);
}


YAGControl::YAGControl(YAGController *controller, Vec2f *var, string name, float min, float max, Vec2f pos) {       // VEC2F
    m_controlArea = Rectf(pos.x, 
                          pos.y, 
                          pos.x + YAGController::m_sliderSizeSmall.x + YAGController::m_controlPadding.x1 + YAGController::m_controlPadding.x2, 
                          pos.y + YAGController::m_sliderSizeSmall.y * 2 + 3 + YAGController::m_controlPadding.y1 + YAGController::m_controlPadding.y2 + YAGController::m_handleHeight );
    
    Rectf sliderAreaX   = Rectf(pos.x + YAGController::m_controlPadding.x1, 
                                pos.y + YAGController::m_controlPadding.y1 + YAGController::m_handleHeight, 
                                pos.x + YAGController::m_sliderSizeSmall.x + YAGController::m_controlPadding.x1, 
                                pos.y + YAGController::m_sliderSizeSmall.y + YAGController::m_controlPadding.y1 + YAGController::m_handleHeight);
    
    Rectf timelineAreaX = Rectf(m_controlArea.x2, 
                                sliderAreaX.y1,
                                m_controlArea.x2 + YAGController::m_timelineWidth, 
                                sliderAreaX.y2);
    
    Rectf sliderAreaY   = Rectf(pos.x + YAGController::m_controlPadding.x1, 
                                sliderAreaX.y2 + 3, 
                                pos.x + YAGController::m_sliderSizeSmall.x + YAGController::m_controlPadding.x1, 
                                sliderAreaX.y2 + YAGController::m_sliderSizeSmall.y + 3);
    
    Rectf timelineAreaY = Rectf(m_controlArea.x2, 
                                sliderAreaY.y1,
                                m_controlArea.x2 + YAGController::m_timelineWidth, 
                                sliderAreaY.y2);
    
    m_sliders.push_back( new YAGSlider(controller, &(var->x), min, max, sliderAreaX, timelineAreaX) );
    m_sliders.push_back( new YAGSlider(controller, &(var->y), min, max, sliderAreaY, timelineAreaY) );
    
    init(controller, name, VEC2F);
}


YAGControl::YAGControl(YAGController *controller, Vec3f *var, string name, float min, float max, Vec2f pos) {       // VEC3F
    m_controlArea = Rectf(pos.x, 
                          pos.y, 
                          pos.x + YAGController::m_sliderSizeSmall.x + YAGController::m_controlPadding.x1 + YAGController::m_controlPadding.x2, 
                          pos.y + YAGController::m_sliderSizeSmall.y * 3 + 6 + YAGController::m_controlPadding.y1 + YAGController::m_controlPadding.y2 + YAGController::m_handleHeight );
    
    Rectf sliderAreaX   = Rectf(pos.x + YAGController::m_controlPadding.x1, 
                                pos.y + YAGController::m_controlPadding.y1 + YAGController::m_handleHeight, 
                                pos.x + YAGController::m_sliderSizeSmall.x + YAGController::m_controlPadding.x1, 
                                pos.y + YAGController::m_sliderSizeSmall.y + YAGController::m_controlPadding.y1 + YAGController::m_handleHeight);
    
    Rectf timelineAreaX = Rectf(m_controlArea.x2, 
                                sliderAreaX.y1,
                                m_controlArea.x2 + YAGController::m_timelineWidth, 
                                sliderAreaX.y2);
    
    Rectf sliderAreaY   = Rectf(pos.x + YAGController::m_controlPadding.x1, 
                                sliderAreaX.y2 + 3, 
                                pos.x + YAGController::m_sliderSizeSmall.x + YAGController::m_controlPadding.x1, 
                                sliderAreaX.y2 + YAGController::m_sliderSizeSmall.y + 3);
    
    Rectf timelineAreaY = Rectf(m_controlArea.x2, 
                                sliderAreaY.y1,
                                m_controlArea.x2 + YAGController::m_timelineWidth, 
                                sliderAreaY.y2);
    
    Rectf sliderAreaZ   = Rectf(pos.x + YAGController::m_controlPadding.x1, 
                                sliderAreaY.y2 + 3, 
                                pos.x + YAGController::m_sliderSizeSmall.x + YAGController::m_controlPadding.x1, 
                                sliderAreaY.y2 + YAGController::m_sliderSizeSmall.y + 3);
    
    Rectf timelineAreaZ = Rectf(m_controlArea.x2, 
                                sliderAreaZ.y1,
                                m_controlArea.x2 + YAGController::m_timelineWidth, 
                                sliderAreaZ.y2);
    
    m_sliders.push_back( new YAGSlider(controller, &(var->x), min, max, sliderAreaX, timelineAreaX) );
    m_sliders.push_back( new YAGSlider(controller, &(var->y), min, max, sliderAreaY, timelineAreaY) );
    m_sliders.push_back( new YAGSlider(controller, &(var->z), min, max, sliderAreaZ, timelineAreaZ) );
    
    init(controller, name, VEC3F);
}


void YAGControl::init(YAGController *controller, string name, ControlType type) {
    boost::to_upper(name);
    m_name              = name;
    m_type              = type;
    m_selectedSlider    = NULL;
    m_selectedTimeline  = NULL;
    m_controller        = controller;
    m_isSelected        = false;
    m_mousePrev         = Vec2f::zero();
}


void YAGControl::draw() {
    
    // draw control box
    gl::color(YAGController::m_controlBgColor);
    gl::drawSolidRect(m_controlArea);
/*
    // draw control border    
    gl::color(YAGController::m_controlBorderColor);
    glBegin(GL_LINE_LOOP);
    glVertex2f(m_controlArea.x1 - 1, m_controlArea.y1 - 1);
    glVertex2f(m_controlArea.x2 + 1 + YAGController::m_timelineWidth, m_controlArea.y1 - 1);
    glVertex2f(m_controlArea.x2 + 1 + YAGController::m_timelineWidth, m_controlArea.y2 + 1);
    glVertex2f(m_controlArea.x1 - 1, m_controlArea.y2 + 1);
    glEnd();
*/

    // draw toggle button
    //m_toggleControlSize
    
    // draw label
    gl::drawString(m_name, 
                   Vec2f(m_controlArea.x1 + YAGController::m_toggleControlSize.x + YAGController::m_controlPadding.x1, m_controlArea.y1 + YAGController::m_controlPadding.y1 + 5), 
                   YAGController::m_fontColor, YAGController::m_textFont);
    
    
    
    vector<YAGSlider*>::iterator  it = m_sliders.begin();
    while( it != m_sliders.end() )
        (*it++)->draw();
}



bool YAGControl::onMouseUp(MouseEvent event) {
    if (m_selectedSlider != NULL) {
        m_selectedSlider->onMouseUp(event);
        m_selectedSlider = NULL;
        m_selectedTimeline = NULL;
    }
    m_isSelected = false;
}


bool YAGControl::onMouseWheel(MouseEvent event) {}

bool YAGControl::isOnControl(Vec2f pos) {

    
}


bool YAGControl::onMouseDown(MouseEvent event) {
    
    m_mousePrev = event.getPos();
    
    
    vector<YAGSlider*>::iterator  it = m_sliders.begin();
    while( it != m_sliders.end() && m_selectedSlider == NULL ) {
        if ( (*it)->isOnSlider(m_mousePrev) ) {                 // is on slider
            m_selectedSlider = (*it);
            onMouseDrag(event);
            m_isSelected = true;
            return true;
        }
        if ( (*it)->isOnTimeline(m_mousePrev) ) {               // is on timeline
            m_selectedSlider    = (*it);
            m_selectedTimeline  = (*it)->getTimeline();
            onMouseDrag(event);
            m_isSelected = true;
            return true;
        }
        it++;
    }

    if (m_controlArea.contains(m_mousePrev)) {                  // is on control
        m_isSelected = true;
        onMouseDrag(event);
        return true;
    }
    
    return false;
}


bool YAGControl::onMouseDrag(MouseEvent event) {
    Vec2f pos = event.getPos();  
 
    if (m_selectedSlider != NULL && m_selectedTimeline == NULL) {               // is on slider
        m_selectedSlider->updateSlider(pos);
    } else if (m_selectedTimeline != NULL) {                                    // is on timeline
        m_selectedSlider->updateTimeline(event, m_controller->tRender(), m_mousePrev);
    } else if (m_isSelected) {
        drag(pos);                                                              // is on control handle
    }
    
    m_mousePrev = event.getPos();
    m_mousePrev.x  = math<float>::max( 0.0f, math<float>::min(m_mousePrev.x, getWindowWidth() ) );
    m_mousePrev.y  = math<float>::max( 0.0f, math<float>::min(m_mousePrev.y, getWindowHeight() ) );
}


void YAGControl::drag(Vec2f pos) {
    pos.x  = math<float>::max( 0.0f, math<float>::min(pos.x, getWindowWidth() ) );
    pos.y  = math<float>::max( 0.0f, math<float>::min(pos.y, getWindowHeight() ) );
    
    Vec2f offset = pos - m_mousePrev;
    offset.x = math<float>::max( -m_controlArea.x1, math<float>::min(offset.x, getWindowWidth() -m_controlArea.x2 ) );
    offset.y = math<float>::max( -m_controlArea.y1, math<float>::min(offset.y, getWindowHeight() -m_controlArea.y2 ) );
    
    m_controlArea.offset(offset);
    vector<YAGSlider*>::iterator  it = m_sliders.begin();
    while( it != m_sliders.end() )
        (*it++)->drag(offset);
}


void YAGControl::updateValues() {
    double time = m_controller->tRender();
    vector<YAGSlider*>::iterator it = m_sliders.begin();
    while( it != m_sliders.end() )
        (*it++)->update(time);
}
