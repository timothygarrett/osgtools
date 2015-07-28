/*
	curtainwidget.h
	Widget showing curtains over the scenegraph
	
	Tim Garrett	(garrettt@iastate.edu)
	2015.07.20
*/
#pragma once


// OSG
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ref_ptr>
#include <osg/Switch>

// Local
#include "osgtools.h"
#include "widget.h"
#include "histogram.h"

namespace osgtools {
	
	class OSGTOOLS CurtainWidget : public Widget
	{
	protected:
		int _left;
		int _right;

		// Active range
		int _activeLeft;
		int _activeRight;
		
		// Offset
		int _startX;
		int _startY;
		int _endX;
		int _endY;

		// Curtain switches and geodes
		osg::ref_ptr<osg::Switch> _leftCurtainSwitch;
		osg::ref_ptr<osg::Switch> _rightCurtainSwitch;
		osg::ref_ptr<osg::Geode> _leftCurtain;
		osg::ref_ptr<osg::Geode> _rightCurtain;

		/*!
		 *	Helper to create a curtain geometry
		 */
		osg::Geometry* createCurtain(int x, int y, int width, int height);
		
	public:
		CurtainWidget();
		CurtainWidget( int width, int height );
		
		void reset();
		void setHistogramActiveRange( osgtools::Histogram* pHistogram );
		void setActiveRange( int left, int right );
		void setOffset( int startX, int startY, int endX, int endY );
		void setEndPoints( int left, int right );
		void update();
	};
}