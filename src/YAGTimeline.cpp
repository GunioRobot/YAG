//
//  YAGTimeline.cpp
//  YAG2
//
//  Created by Q on 16/03/2011.
//  Copyright 2011 . All rights reserved.
//

#include "cinder/Utilities.h"
#include "YAGController.h"
#include "YAGTimeline.h"

using namespace ci;
using namespace ci::app;
using namespace std;


bool sortKeyframesHelper(Keyframe *a, Keyframe *b) { return ( a->m_time < b->m_time ); }


YAGTimeline::YAGTimeline(YAGSlider *slider, float min, float max, ci::Rectf activeArea) {
	m_slider		= slider;
    m_controller	= slider->m_controller;
    m_activeArea	= activeArea;
    
    m_dragKeyframe = NULL;
    m_selectedKeyframe = NULL;
}


void YAGTimeline::draw() {
    
    // draw background
    gl::color(YAGController::m_timelineBgColour);
    gl::drawSolidRect(m_activeArea);
	
    int timelineStartInSteps  = m_controller->tRender() * 4 - YAGController::m_timelineOffsetInPixels / m_controller->getTimelineStepWidth();
    int timelineEndInSteps    = (m_controller->tRender() + (m_activeArea.getWidth() / m_controller->getTimelineBeatWidth()) ) * 4 
	- YAGController::m_timelineOffsetInPixels / m_controller->getTimelineStepWidth() + 1;
    
    // draw bars and beats
    for(int step = timelineStartInSteps; step < timelineEndInSteps; step++) {
        int stepPos = step * m_controller->getTimelineStepWidth() + m_activeArea.x1 - m_controller->tRender() * m_controller->getTimelineBeatWidth();
        stepPos += YAGController::m_timelineOffsetInPixels;
		
        if ( step >= 0 && stepPos > m_activeArea.x1 && stepPos < m_activeArea.x2 ) {
            if (step%16 == 0) {
				gl::drawString(toString(step/4), Vec2f(stepPos + 2, m_activeArea.y1 + 5), YAGController::m_fontColor, YAGController::m_textFont);
                gl::color(YAGController::m_timelineBarColor);
            } else if (step%4 == 0) {
                gl::color(YAGController::m_timelineBeatColor);
            } else {
                gl::color(YAGController::m_timelineStepColor);
            }
            glBegin(GL_LINES);
            glVertex2f(stepPos, m_activeArea.y1);
            glVertex2f(stepPos, m_activeArea.y2);
            glEnd();
        }     
    }
    
    // draw timeline indicator
    gl::color(YAGController::m_timelineIndicatorColor);
    glBegin(GL_LINES);
    glVertex2f(m_activeArea.x1 + YAGController::m_timelineOffsetInPixels, m_activeArea.y1);
    glVertex2f(m_activeArea.x1 + YAGController::m_timelineOffsetInPixels, m_activeArea.y2);
    glEnd();
		
	// draw keyframes and lines
	if (m_keyframes.empty())
		return;

	vector<Keyframe*> kfs = getTimelineKeyframes();
		
	// draw lines
    gl::color(YAGController::m_keyframeColor);
	double tFirst	= m_controller->tRender() - (float)YAGController::m_timelineOffsetInPixels / m_controller->getTimelineBeatWidth();
	double tLast	= tFirst + (float)m_activeArea.getWidth() / m_controller->getTimelineBeatWidth();
	float firstVal	= m_activeArea.y2 - m_activeArea.getHeight() * getValueAt(tFirst) / (m_slider->m_max - m_slider->m_min);
	float lastVal	= m_activeArea.y2 - m_activeArea.getHeight() * getValueAt(tLast) / (m_slider->m_max - m_slider->m_min);
	
	glBegin(GL_LINE_STRIP);
	glVertex2f(m_activeArea.x1, firstVal);
	for (int k = 0; k < kfs.size(); k++) {		
		Keyframe *kf    = kfs.at(k);
		Vec2f   pos     = getKeyframePos(kf);
		glVertex2f(pos.x, pos.y);
	}
	glVertex2f(m_activeArea.x2, lastVal);		
	glEnd();
	
	// draw keyframes
	for (int k = 0; k < kfs.size(); k++) {
		Keyframe *kf    = kfs.at(k);
        Vec2f   pos     = getKeyframePos(kf);
		
		if (m_selectedKeyframe == kf) 
			glBegin(GL_POLYGON);
		else        
			glBegin(GL_LINE_LOOP);
		
		for( float j = 0; j <= 3.14 * 2; j += 3.14 / 4 ) {
			glVertex2f(pos.x + YAGController::m_keyframeSize * sin(j), pos.y + YAGController::m_keyframeSize * cos(j) );
		}
		glEnd();
	}
	
    
}


void YAGTimeline::deleteKeyframe(double time, float value) {
    for(vector<Keyframe*>::iterator it = m_keyframes.begin(); it != m_keyframes.end(); it ++) {
        if ( (*it)->m_time == time && (*it)->m_value == value ) {
            delete (*it);
            m_keyframes.erase(it);
            m_selectedKeyframe  = NULL;
            m_dragKeyframe      = NULL;
            return;
        }
    }
}


Keyframe* YAGTimeline::getKeyframe(double time, float value) {  
    Keyframe *kf = NULL;
    
	float tollerance = YAGController::m_keyframeSize / (m_slider->m_max - m_slider->m_min);

    // find an existing keyframe 
    vector<Keyframe*>::iterator it = m_keyframes.begin();
    while(it != m_keyframes.end() && kf == NULL) {
		//        if ( (*it)->m_time == time && (*it)->m_value == value) 
        if ( (*it)->m_time == time && (*it)->m_value >= value - tollerance && (*it)->m_value <= value + tollerance)
			kf = (*it);
        *it++;
    }
    
    // create a new one otherwise
    if (kf == NULL) {
        kf = new Keyframe(time, value);
        m_keyframes.push_back(kf);
        sort(m_keyframes.begin(), m_keyframes.end(), sortKeyframesHelper);
    }
    return kf;
}


void YAGTimeline::sortKeyframes() {
    sort(m_keyframes.begin(), m_keyframes.end(), sortKeyframesHelper);                  // sort keyframe by time
}


Keyframe* YAGTimeline::findPrevKeyframe(double time) {
    Keyframe *kf;
    for(int k = m_keyframes.size() - 1; k >= 0; k--) {                                  // return the first keyframe found
        kf = m_keyframes.at(k);
        if ( kf->m_time < time )                               
            return (kf);
    }
    return NULL;
}


Keyframe* YAGTimeline::findNextKeyframe(double time) {
    Keyframe *kf = NULL;
    for(int k=0; k < m_keyframes.size(); k++) {                                         // return the first keyframe found
        kf = m_keyframes.at(k);
        if ( kf->m_time > time )                               
            return (kf);
    }
    return NULL;
}


Vec2f YAGTimeline::getKeyframePos(Keyframe *kf) {
    Vec2f   pos;
    pos.x   = kf->m_time * m_controller->getTimelineBeatWidth() 
            + m_activeArea.x1 + YAGController::m_timelineOffsetInPixels 
            - m_controller->tRender() * m_controller->getTimelineBeatWidth();
    
    pos.y   = m_activeArea.y2 - m_activeArea.getHeight() * kf->m_value / (m_slider->m_max - m_slider->m_min);    
    
    return pos;
}



vector<Keyframe*> YAGTimeline::getTimelineKeyframes() {
	vector<Keyframe*> kfs;
	double lowVal	= m_controller->tRender() - (float)YAGController::m_timelineOffsetInPixels / m_controller->getTimelineBeatWidth();
	double highVal	= lowVal + (float)YAGController::m_timelineWidth / m_controller->getTimelineBeatWidth();
	
	vector<Keyframe*>::iterator it = m_keyframes.begin();
	while(it != m_keyframes.end()) {
		if ( (*it)->m_time >= lowVal && (*it)->m_time <= highVal )
			kfs.push_back((*it));
		it++;
	}
	sort(kfs.begin(), kfs.end(), sortKeyframesHelper);
	return kfs;
}


float YAGTimeline::getValueAt(double time) {
	Keyframe *kfPrev = findPrevKeyframe(time);
	Keyframe *kfNext = findNextKeyframe(time);
	
	if (kfNext == NULL && kfPrev == NULL)
		return 0;								// fix this!!!!!fix this!!!!!fix this!!!!!fix this!!!!!fix this!!!!!fix this!!!!!fix this!!!!!
	
	if (kfNext == NULL)                                                         // no more keyframes, don't update the value
		return kfPrev->m_value;
	
	if (kfPrev == NULL)                                                         // no previous keyframe, get the next one
		return kfNext->m_value;

	// keyframes found, calculate the time based value
	float val = kfPrev->m_value + (kfNext->m_value - kfPrev->m_value)  * (time - kfPrev->m_time) / (kfNext->m_time - kfPrev->m_time);

	return val;
}

