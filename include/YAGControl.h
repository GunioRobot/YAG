//
//  YAGControl.h
//  YAG2
//
//  Created by Q on 16/03/2011.
//  Copyright 2011 . All rights reserved.
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include <vector>
#include "YAGSlider.h"
#include "YAGTimeline.h"

class YAGController;


class YAGControl {
    enum ControlType {
        INT,
        FLOAT,
        VEC2F,
        VEC3F
    };

	friend class YAGController;

public:
    YAGControl(YAGController *controller, int *var, std::string name, float min, float max, ci::Vec2f pos);
    YAGControl(YAGController *controller, float *var, std::string name, float min, float max, ci::Vec2f pos);
    YAGControl(YAGController *controller, ci::Vec2f *var, std::string name, float min, float max, ci::Vec2f pos);
    YAGControl(YAGController *controller, ci::Vec3f *var, std::string name, float min, float max, ci::Vec2f pos);

    ~YAGControl() {};

    void draw();

    void init(YAGController *controller, std::string name, ControlType type);

    bool	onMouseDown(ci::app::MouseEvent event);
	bool	onMouseUp(ci::app::MouseEvent event);
	bool	onMouseDrag(ci::app::MouseEvent event);
    bool    onMouseWheel(ci::app::MouseEvent event);

    bool    isOnControl(ci::Vec2f pos);
    void    drag(ci::Vec2f pos);
    void    updateValues();

	std::string getName() { return m_name; };

	std::string serialize();


protected:
    std::string             m_name;
    std::vector<YAGSlider*> m_sliders;
    YAGSlider               *m_selectedSlider;
    YAGTimeline             *m_selectedTimeline;
    ControlType             m_type;
    ci::Vec2f               m_mousePrev;
    YAGController           *m_controller;
    ci::Rectf               m_controlArea;
    bool                    m_isSelected;
};

