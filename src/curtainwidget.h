/*
	curtainwidget.h
	Widget showing curtains over the scenegraph
	
	Tim Garrett	(garrettt@iastate.edu)
	2015.07.20
*/
#pragma once

// Local
#include "widget.h"

// OSG
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ref_ptr>


namespace osgtools {
	
	class CurtainWidget : public Widget
	{
	protected:
		int _left;
		int _right;
		
		// Offset
		int _startX;
		int _startY;
		int _endX;
		int _endY;

		osg::ref_ptr<osg::Geode> _leftCurtain;
		osg::ref_ptr<osg::Geode> _rightCurtain;

		/*!
		 *	Helper to create a curtain geometry
		 */
		osg::Geometry* createCurtain(int x, int y, int width, int height);
		
	public:
		CurtainWidget();
		CurtainWidget( int width, int height );
		
		void setOffset( int startX, int startY, int endX, int endY );
		void setEndPoints( int left, int right );
		void update();
	};
}