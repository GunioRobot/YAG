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


YAGTimeline::YAGTimeline(YAGController *controller, float min, float max, ci::Rectf activeArea) {
    m_controller = controller;
    m_activeArea = activeArea;

    m_dragKeyframe = NULL;
    m_selectedKeyframe = NULL;

    m_min = min;    // <<< find a way to get rid of it, the SLider already has this variable
    m_max = max;    // <<< find a way to get rid of it, the SLider already has this variable
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



    // draw timeline time
//    gl::drawString( toString((float)m_controller->tRender()),
  //                  Vec2f(m_activeArea.x1 - 40, m_activeArea.y1 + 7),
    //                YAGController::m_fontColor, YAGController::m_textFont);


    // draw timeline indicator
    gl::color(YAGController::m_timelineIndicatorColor);
    glBegin(GL_LINES);
    glVertex2f(m_activeArea.x1 + YAGController::m_timelineOffsetInPixels, m_activeArea.y1);
    glVertex2f(m_activeArea.x1 + YAGController::m_timelineOffsetInPixels, m_activeArea.y2);
    glEnd();


    //draw lines
    gl::color(YAGController::m_keyframeColor);
    for (int k=0; k < m_keyframes.size(); k++) {
        Keyframe *kf    = m_keyframes.at(k);
        Vec2f   pos     = getKeyframePos(kf);


        if ( m_activeArea.contains(pos) ) {
            // draw keyframe

            if (m_selectedKeyframe == kf)
                glBegin(GL_POLYGON);
            else
                glBegin(GL_LINE_LOOP);

            for( float j = 0; j <= 3.14 * 2; j += 3.14 / 4 ){
                glVertex2f(pos.x + 5 * sin(j), pos.y + 5 * cos(j) );
            }
            glEnd();

            if ( k == 0 && pos.x > m_activeArea.x1) {
                glBegin(GL_LINE_STRIP);
                glVertex2f(m_activeArea.x1, pos.y);         // origin
                glVertex2f(pos.x, pos.y);                   // first keyframe
                glEnd();
            }

            if ( k == m_keyframes.size() - 1 && pos.x <= m_activeArea.x2) {
                glBegin(GL_LINE_STRIP);
                glVertex2f(pos.x, pos.y);                   // last keyframe
                glVertex2f(m_activeArea.x2, pos.y);         // end
                glEnd();
            }

            if ( k < m_keyframes.size() - 1) {
                Keyframe *nextKf    = m_keyframes.at(k + 1);
                Vec2f    nextPos    = getKeyframePos(nextKf);
                nextPos.x           = math<float>::min(nextPos.x, m_activeArea.x2);
                glBegin(GL_LINE_STRIP);
                glVertex2f(pos.x, pos.y);
                glVertex2f(nextPos.x, nextPos.y);
                glEnd();
            }
        }

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

    console() << "getKeyframe " << time << " " << value << std::endl;

    // find an existing keyframe
    vector<Keyframe*>::iterator it = m_keyframes.begin();
    while(it != m_keyframes.end() && kf == NULL) {
        if ( (*it)->m_time == time && (*it)->m_value == value)
            kf = (*it);
        *it++;
    }

    // create a new one otherwise
    if (kf == NULL) {
        kf = new Keyframe(time, value);
        m_keyframes.push_back(kf);
        sort(m_keyframes.begin(), m_keyframes.end(), sortKeyframesHelper);
        console() << "keyframe created(" << m_keyframes.size() << "): " << kf->m_time << " " << kf->m_value << std::endl;
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

    pos.y   = m_activeArea.y2 - m_activeArea.getHeight() * kf->m_value / (m_max - m_min);

    return pos;
}

