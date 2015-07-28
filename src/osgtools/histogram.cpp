/*
	histogram.cpp
	Histogram plot for OSG
	
	Tim Garrett (garrettt@iastate.edu)
	2015.06.16
*/

#include "histogram.h"

osgtools::Histogram::Histogram( int width, int height ) :
	Plot( width, height, "x", "y" )
{
	// Create the geode
	_pGeode = new osg::Geode();
	_pGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	// Add to the scene graph
	addChild( _pGeode.get() );
}

bool osgtools::Histogram::setHistogram( std::vector<float>& bins )
{
	// Handle bad input
	if (bins.size() == 0)
		return false;

	// Update the bins
	_bins.clear();
	_bins = bins;

	// Get the max element
	float maxval = _bins[0];
	for (int i=1; i < _bins.size(); i++) {
		if (maxval < _bins[i])
			maxval = _bins[i];
	}

	// Resize the graph
	//int majorTick = (int)maxval/3;
	//setMajorAxisGrid(1, (float)(majorTick));
	//setMinorAxisGrid(1, (float)majorTick/2);
	autoUpdateMajorMinorAxes();
	setRange(-1, 0, bins.size(), maxval);
	

	return true;
}

void osgtools::Histogram::redraw()
{
	// Clear the active Geometry
	if (_histogramGeo.size() > 0) {
		for (int i=0; i<_histogramGeo.size(); i++) {
			_pGeode->removeDrawable( _histogramGeo[i].release() );
		}
		_histogramGeo.clear();
	}
	
	//Construct the rectangles
	for (int i=0; i < _bins.size(); i++) {
		float startx = getXValuePixel(i-.45);
		float endx = getXValuePixel(i+.45);
		float starty = getYValuePixel(0);
		float endy = getYValuePixel(_bins[i]);

		startx = (startx == -1 ? getXValuePixel(_range[0]) : startx);
		endx = (endx == -1 ? getXValuePixel(_range[2]) : endx);
		starty = (starty == -1 ? getYValuePixel(_range[1]) : starty);
		endy = (endy == -1 ? getYValuePixel(_range[3]) : endy);

		osg::ref_ptr<osg::Geometry> rectangle = new osg::Geometry();
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
		vertices->push_back(osg::Vec3( startx, starty, 0));
		vertices->push_back(osg::Vec3( endx, starty, 0));
		vertices->push_back(osg::Vec3( endx, endy, 0));
		vertices->push_back(osg::Vec3( startx, endy, 0));
		rectangle->setVertexArray(vertices.release());

		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
		colors->push_back(osg::Vec4(0,1,0,1));
		colors->push_back(osg::Vec4(0,.5,0,1));
		colors->push_back(osg::Vec4(0,.5,0,1));
		colors->push_back(osg::Vec4(0,1,0,1));
		rectangle->setColorArray( colors.release() );
		rectangle->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
		rectangle->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, 4) );

		_pGeode->addDrawable( rectangle.get() );
		_histogramGeo.push_back( rectangle );
	}

	_pGeode->dirtyBound();
}

void osgtools::Histogram::autoUpdateMajorMinorAxes()
{
	if (_bins.size() <= 0)
		return;

	// Get the max element
	float maxval = _bins[0];
	for (int i=1; i < _bins.size(); i++) {
		if (maxval < _bins[i])
			maxval = _bins[i];
	}

	setMajorAxisGrid( _bins.size()/ 6, maxval / 6 );
	setMinorAxisGrid( 0, maxval / 12 );
}