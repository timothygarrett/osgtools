/*
	curtainwidget.cpp
	Widget showing curtains over the scenegraph
	
	Tim Garrett	(garrettt@iastate.edu)
	2015.07.20
*/

#include "curtainwidget.h"

osgtools::CurtainWidget::CurtainWidget() :
	_left(-1),
	_right(-1),
	_startX(0),
	_startY(0),
	_endX(0),
	_endY(0),
	_activeLeft(-1),
	_activeRight(-1)
{}

osgtools::CurtainWidget::CurtainWidget( int width, int height ) :
	Widget( width, height, 0, 0, width, height ),
	_left(0),
	_right(width),
	_startX(0),
	_startY(0),
	_endX(width),
	_endY(height),
	_activeLeft(0),
	_activeRight(width)
{
	// Create the switches
	_leftCurtainSwitch = new osg::Switch();
	_rightCurtainSwitch = new osg::Switch();

	// Create the geodes
	_leftCurtain = new osg::Geode();
	_rightCurtain = new osg::Geode();
	_leftCurtain->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
	_rightCurtain->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );

	_leftCurtainSwitch->addChild( _leftCurtain.get() );
	_rightCurtainSwitch->addChild( _rightCurtain.get() );
	addChild( _leftCurtainSwitch.get() );
	addChild( _rightCurtainSwitch.get() );

	update();
}

osg::Geometry* osgtools::CurtainWidget::createCurtain( int x, int y, int width, int height )
{
	osg::ref_ptr<osg::Geometry> pGeo = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> pVertices = new osg::Vec3Array();
	
	// Set the vertices
	pVertices->push_back(osg::Vec3(x, y, 0));
	pVertices->push_back(osg::Vec3(x+width, y, 0));
	pVertices->push_back(osg::Vec3(x+width, y+height, 0));
	pVertices->push_back(osg::Vec3(x, y+height, 0));
	pGeo->setVertexArray( pVertices.get() );

	// Set the color
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array();
	pColors->push_back(osg::Vec4(0.0, 0.0, 0.0, 0.85));
	pGeo->setColorArray( pColors.get() );
	pGeo->setColorBinding( osg::Geometry::BIND_OVERALL );
	pGeo->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, 4) );

	return pGeo.release();
}

void osgtools::CurtainWidget::setEndPoints( int left, int right )
{
	_left = (left < 0 ? 0 : left);
	_right = (right > _width ? _width : right);
	
	// Update the nodes
	update();
}

void osgtools::CurtainWidget::update()
{
	if (_left == -1 && _right == -1)
		return;		

	if (!_leftCurtain.valid()) {
		_leftCurtain = new osg::Geode();
		addChild( _leftCurtain.get() );
	}
	if (!_rightCurtain.valid()) {
		_rightCurtain = new osg::Geode();
		addChild( _rightCurtain.get() );
	}

	// Clear geometry
	_leftCurtain->removeDrawables(0, _leftCurtain->getNumDrawables());
	_rightCurtain->removeDrawables(0, _rightCurtain->getNumDrawables());

	// Add new curtains
	_leftCurtain->addDrawable( createCurtain(_startX, _startY, _left, _height) );
	_rightCurtain->addDrawable( createCurtain(_startX + _right, _startY, _width - _right, _height) );

	// Turn off
	if (_left <= _activeLeft)
		_leftCurtainSwitch->setAllChildrenOff();
	else
		_leftCurtainSwitch->setAllChildrenOn();
	if (_right >= _activeRight)
		_rightCurtainSwitch->setAllChildrenOff();
	else
		_rightCurtainSwitch->setAllChildrenOn();
}

void osgtools::CurtainWidget::setOffset(int startX, int startY, int endX, int endY)
{
	_startX = startX;
	_startY = startY;
	_endX = endX;
	_endY = endY;
	_width = (_width <= _endX - _startX ? _width : _endX - _startX);
	_height = (_height <= _endY - _startY ? _height : _endY - _startY);
	update();

}

void osgtools::CurtainWidget::setActiveRange( int left, int right )
{
	_activeLeft = left;
	_activeRight = right;
}

void osgtools::CurtainWidget::setHistogramActiveRange( osgtools::Histogram* pHistogram )
{
	if (!pHistogram)
		return;

	// Get the dimensions
	int xMin, yMin, width, height;
	float xN, yN, xP, yP;
	pHistogram->getPlotDimensions(xMin, yMin, width, height);
	pHistogram->getRange(xN, yN, xP, yP);

	// Set the range
	setActiveRange(pHistogram->getXValuePixel(xN + 1) - xMin, pHistogram->getXValuePixel(xP) - xMin);
}

void osgtools::CurtainWidget::reset()
{
	setEndPoints(0, _width);
}