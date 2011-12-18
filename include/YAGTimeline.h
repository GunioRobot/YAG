//
//  YAGTimeline.h
//  YAG2
//
//  Created by Q on 16/03/2011.
//  Copyright 2011 . All rights reserved.
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include <vector>

class YAGController;
class YAGSlider;

class Keyframe {
public:

    Keyframe(double time, float value) { m_time = time; m_value = value; };

    double  m_time;
    float   m_value;

};


class YAGTimeline {

   friend class YAGSlider;

public:
    YAGTimeline(YAGSlider *slider, float min, float max, ci::Rectf activeArea);
    ~YAGTimeline() {};

    void draw();

    ci::Rectf               m_activeArea;
    std::vector<Keyframe*>  m_keyframes;

    void deleteKeyframe(double time, float value);
    Keyframe* getKeyframe(double time, float value);
    Keyframe* findPrevKeyframe(double time);
    Keyframe* findNextKeyframe(double time);

    Keyframe    *m_dragKeyframe;
    Keyframe    *m_selectedKeyframe;

    void sortKeyframes();
    ci::Vec2f getKeyframePos(Keyframe *kf);

	std::vector<Keyframe*>	getTimelineKeyframes();

	float					getValueAt(double time);

private:
	YAGSlider		*m_slider;
    YAGController	*m_controller;

};

