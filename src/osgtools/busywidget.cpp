/*
	busywidget.cpp
	2D busy cursor widget for OSG
	
	Tim Garrett (garrettt@iastate.edu)
	2015.07.02
*/

#include "busywidget.h"

//#include <osg/ShapeDrawable>

osgtools::BusyWidget::BusyWidget( int width, int height ) :
	Widget(width, height, (width-30)/2, (height-30)/2, 30, 30),
	_sImgPath("../../dep/images/spinning-wait-icons/wait30trans.gif")
{
	// Set the background image
	setBackgroundImage(_sImgPath);

	// Set the background color
	if (_camera.valid())
		_camera->setClearColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
}

osgtools::BusyWidget::BusyWidget( int width, int height, std::string& imagePath) : 
	Widget(width, height, width/2, height/2, 0, 0),
	_sImgPath( imagePath )
{
	// Set the background image
	setBackgroundImage(_sImgPath);

	// Set the background color
	if (_camera.valid())
		_camera->setClearColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	// Update the dimensions
	if (_backgroundImage.valid()) {
		setWidth(_backgroundImage->s());
		setHeight(_backgroundImage->t());
	}
}