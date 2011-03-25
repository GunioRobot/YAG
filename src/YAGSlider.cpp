//
//  YAGSlider.cpp
//  YAG2
//
//  Created by Q on 16/03/2011.
//  Copyright 2011 . All rights reserved.
//


#include "cinder/Utilities.h"
#include "YAGController.h"
#include "YAGSlider.h"

using namespace ci;
using namespace ci::app;
using namespace std;


YAGSlider::YAGSlider(YAGController *controller, float *var, float min, float max, Rectf sliderActiveArea, Rectf timelineActiveArea) {
	init(controller, var, min, max, sliderActiveArea, timelineActiveArea, FLOAT);
}

YAGSlider::YAGSlider(YAGController *controller, int *var, float min, float max, Rectf sliderActiveArea, Rectf timelineActiveArea) {
	init(controller, var, min, max, sliderActiveArea, timelineActiveArea, INT);
}

void YAGSlider::init(YAGController *controller, void *var, float min, float max, Rectf sliderActiveArea, Rectf timelineActiveArea, SliderType type) {
	m_controller = controller;
	m_var = var;
	m_min = min;
	m_max = max;
	m_activeArea = sliderActiveArea;
	m_timeline = new YAGTimeline(this, min, max, timelineActiveArea);
	m_type = type;
}



void YAGSlider::draw() {
    
    // draw background
    gl::color(YAGController::m_sliderBgColor);
    gl::drawSolidRect(m_activeArea);
    
    // draw foreground
    gl::color(YAGController::m_sliderFgColor);
    int fgWidth;
    
    if (m_type == INT)
        fgWidth = m_activeArea.x1 + m_activeArea.getWidth() * ((*static_cast<int*>(m_var)) - m_min) / (m_max - m_min);
    else
        fgWidth = m_activeArea.x1 + m_activeArea.getWidth() * ((*static_cast<float*>(m_var)) - m_min) / (m_max - m_min);

    
    gl::drawSolidRect( Rectf(m_activeArea.x1,
                             m_activeArea.y1,
                             fgWidth,
                             m_activeArea.y2 ));

    // draw var value
    string valString = (m_type == INT) ? toString(*static_cast<int*>(m_var)) : toString(*static_cast<float*>(m_var));
    gl::drawString(valString, Vec2f(m_activeArea.x1 + 2, m_activeArea.y2 - 9), YAGController::m_fontColor, YAGController::m_textFont);
    
    // draw timeline and keyframes
    m_timeline->draw();
    
}


void YAGSlider::updateSlider(Vec2f pos) {    

    pos -= Vec2f(m_activeArea.x1, m_activeArea.y1);
    float normValue = pos.x / m_activeArea.getWidth();
    normValue = math<float>::max(0.0f, math<float>::min(normValue, 1.0f));                  // clamp 0-1
    
    Keyframe *kf = m_timeline->m_selectedKeyframe;
    
    if (m_type == INT) {
        
        int *varInt = static_cast<int*>(m_var);                                             // cast from void* to int*
        (*varInt) = normValue * (m_max - m_min) + m_min + 0.5f;
        if (kf != NULL)
            kf->m_value = *varInt;
        else
            m_timeline->m_selectedKeyframe = m_timeline->getKeyframe(m_controller->tRender(), *varInt);
        
    } else {
        
        float *varFloat = static_cast<float*>(m_var);                                       // cast from void* to float*
        (*varFloat) = normValue * (m_max - m_min) + m_min;
        if (kf != NULL)
            kf->m_value = *varFloat;
        else
            m_timeline->m_selectedKeyframe = m_timeline->getKeyframe(m_controller->tRender(), *varFloat);
        
    }

}


void YAGSlider::updateTimeline(MouseEvent event, double tRender, Vec2f mousePrev) {    
    Vec2f pos           = event.getPos();  
    
    if (event.isAltDown()) {                                                    // shift timeline
        double shiftOffset = ( mousePrev.x - pos.x ) / m_controller->getTimelineBeatWidth();
        m_controller->shiftTimeline(shiftOffset);
        return;
    }
    
    float stepOffset    = tRender * 4;
    float stepRel       = ( pos.x - m_timeline->m_activeArea.x1 - YAGController::m_timelineOffsetInPixels ) / m_controller->getTimelineStepWidth();
    int step            = stepRel + stepOffset + 0.4f;
    double kf_time      = (double)step / 4;
    double kf_maxTime   = (int)(stepOffset + (float)(YAGController::m_timelineWidth - YAGController::m_timelineOffsetInPixels) / m_controller->getTimelineStepWidth());
    kf_maxTime          /= 4;
    kf_time             = math<float>::max(0.0f, math<float>::min(kf_time, kf_maxTime));
    
    float posY          = m_timeline->m_activeArea.y2 - pos.y;
    float kf_value      = m_min + (m_max - m_min) * posY / m_timeline->m_activeArea.getHeight();
    kf_value            = math<float>::max(m_min, math<float>::min(kf_value, m_max));
    
    if (event.isRight()) {                                                      // delete keyframe
        m_timeline->deleteKeyframe(kf_time, kf_value);
        return;
    }
    
    if (m_timeline->m_dragKeyframe != NULL){                                    // update Keyframe
        m_timeline->m_dragKeyframe->m_time = kf_time;                           // update time
        
        if (m_type == INT)
            m_timeline->m_dragKeyframe->m_value = (int)(kf_value + 0.5f);       // update value
        else
            m_timeline->m_dragKeyframe->m_value = kf_value;                     // update value
        
        
    } else {
        if (m_type == INT)
            m_timeline->m_dragKeyframe  = m_timeline->getKeyframe(kf_time, (int)(kf_value + 0.5f));
        else
            m_timeline->m_dragKeyframe  = m_timeline->getKeyframe(kf_time, kf_value);
        
        m_timeline->m_selectedKeyframe  = m_timeline->m_dragKeyframe;
    }
    
    if (m_type == INT) {
        int *varInt = static_cast<int*>(m_var);
        (*varInt) = kf_value + 0.5f;
    } else {
        float *varFloat = static_cast<float*>(m_var);
        (*varFloat) = kf_value;
    }
    
}


bool YAGSlider::onMouseUp(MouseEvent event) {
    m_timeline->m_dragKeyframe = NULL;                                          // reset drag keyframe
    m_timeline->sortKeyframes();                                                // sort keyframes
	return true;
}


void YAGSlider::drag(Vec2f offset) {
    m_activeArea.offset(offset);                                                // drage slider
    m_timeline->m_activeArea.offset(offset);                                    // drag timeline
}


void YAGSlider::update(double time) {
    if (m_timeline->m_keyframes.empty())
        return;
	
    if (m_type == INT) {
        int *varInt = static_cast<int*>(m_var);
        (*varInt) = m_timeline->getValueAt(time); + 0.5f;
    } else {
        float *varFloat = static_cast<float*>(m_var);
        (*varFloat) = m_timeline->getValueAt(time);;
    }
}

