//
//  YAGSlider.h
//  YAG2
//
//  Created by Q on 16/03/2011.
//  Copyright 2011 . All rights reserved.
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "YAGController.h"
#include "YAGTimeline.h"

class YAGController;


class YAGSlider {
    enum SliderType {
        INT,
        FLOAT
    };
    
public:
    // template <class T> YAGSlider(T *var, float min, float max, ci::Rectf activeArea);
    template <class T>
    YAGSlider(YAGController *controller, T *var, float min, float max, ci::Rectf sliderActiveArea, ci::Rectf timelineActiveArea) {
        m_controller = controller;
        m_var = var;
        m_min = min;
        m_max = max;
        m_activeArea = sliderActiveArea;
        m_timeline = new YAGTimeline(controller, min, max, timelineActiveArea);
        
        if ( typeid(*var) == typeid(int) )
            m_type = INT;
        else if ( typeid(*var) == typeid(float) )
            m_type = FLOAT;
    };
    
    ~YAGSlider() {};
    
    void draw();
    
    bool isOnSlider(ci::Vec2f pos) { return m_activeArea.contains(pos); };
    bool isOnTimeline(ci::Vec2f pos) { return m_timeline->m_activeArea.contains(pos); };
    
    void updateSlider(ci::Vec2f pos);
    void updateTimeline(ci::app::MouseEvent event, double tRender, ci::Vec2f mousePrev);
    
    bool onMouseUp(ci::app::MouseEvent event);
    
    YAGTimeline* getTimeline() { return m_timeline; };
    
    void    drag(ci::Vec2f offset);
    
    void    update(double time);
    
    
private:
    void            *m_var;
    ci::Rectf       m_activeArea;
    SliderType      m_type;
    YAGTimeline     *m_timeline;
    float           m_min;
    float           m_max;
    YAGController   *m_controller;
};
