/*
	Plot.cpp
	Creates a plot in OSG
	
	Tim Garrett (garrettt@iastate.edu)
	2015.06.16
*/

#include "plot.h"

#include <osgDB/ReadFile>

// Constants
const int osgtools::Plot::BOTTOM_SPACING = 60;
const int osgtools::Plot::TOP_SPACING = 10;
const int osgtools::Plot::LEFT_SPACING = 60;
const int osgtools::Plot::RIGHT_SPACING = 10;

const float osgtools::Plot::BACKGROUND_Z = -10.0;
const float osgtools::Plot::MAJORMINORAXES_Z = -9.9;
const float osgtools::Plot::TEXT_Z = -8.0;

osgtools::Plot::Plot() :
	_width(0),
	_height(0),
	_bInitialized( false )
{

}

osgtools::Plot::Plot( int width, int height, std::string xLabel, std::string yLabel ) :
	_width(width),
	_height(height),
	_xLabel(xLabel),
	_yLabel(yLabel)
{
	// Set the window dimensions
	resize( width, height );

	initialize();
	_bInitialized = true;
}

void osgtools::Plot::initialize()
{
	// Set the default range ( 0 < x < 10; 0 < y < 10)
	_range[0] = -1.0;
	_range[1] = -1.0;
	_range[2] = 10.0;
	_range[3] = 10.0;

	// Set the major axis grid
	_majorAxisGrid[0] = _majorAxisGrid[1] = 2;
	_minorAxisGrid[0] = _minorAxisGrid[1] = 1;

	// Clear the plot dimensions
	memset(_plotDim,0,sizeof(int)*4);

	// Create the background
	_pPlotGeode = new osg::Geode();
	_pBackgroundGeo = createBackground();
	_pPlotGeode->addDrawable( _pBackgroundGeo.get() );

	// Create the grid lines
	if (_majorAxisGrid[0] > 0 || _majorAxisGrid[1] > 0) {
		_pMajorAxisGridLinesGeo = createMajorAxisGridLines();
		_pPlotGeode->addDrawable( _pMajorAxisGridLinesGeo.get() );
	}
	if (_minorAxisGrid[0] > 0 || _minorAxisGrid[1] > 0) {
		_pMinorAxisGridLinesGeo = createMinorAxisGridLines();
		_pPlotGeode->addDrawable( _pMinorAxisGridLinesGeo.get() );
	}

	// Create the axis labels
	_pXLabelGeo = createXLabel();
	_pPlotGeode->addDrawable( _pXLabelGeo.get() );
	_pYLabelGeo = createYLabel();
	_pPlotGeode->addDrawable( _pYLabelGeo.get() );

	// Create the major axis tick lines
	_pTickLinesGeo = createTickMarks();
	_pPlotGeode->addDrawable( _pTickLinesGeo.get() );

	// Add the major axis labels
	createAddXLabels( _pPlotGeode.get(), _xLabelsGeo );
	createAddYLabels( _pPlotGeode.get(), _yLabelsGeo );

	// Add the plot geode to the scene graph
	addChild(_pPlotGeode.get());
}

osg::Geometry* osgtools::Plot::createBackground()
{
	osg::ref_ptr<osg::Geometry> pRectangle = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> pVertices = new osg::Vec3Array();
	
	// Set the spacing for the height and width
	_plotDim[0] = (int)((_yLabel.compare("")==0) ? RIGHT_SPACING : LEFT_SPACING);
	_plotDim[1] = (int)((_xLabel.compare("")==0) ? TOP_SPACING : BOTTOM_SPACING);
	_plotDim[2] = (int)((_width - (RIGHT_SPACING)) - _plotDim[0]);
	_plotDim[3] = (int)((_height - (TOP_SPACING)) - _plotDim[1]);

	// Set the vertices
	pVertices->push_back(osg::Vec3(_plotDim[0], _plotDim[1], BACKGROUND_Z));
	pVertices->push_back(osg::Vec3(_plotDim[0] + _plotDim[2], _plotDim[1], BACKGROUND_Z));
	pVertices->push_back(osg::Vec3(_plotDim[0] + _plotDim[2], _plotDim[1] + _plotDim[3], BACKGROUND_Z));
	pVertices->push_back(osg::Vec3(_plotDim[0], _plotDim[1] + _plotDim[3], BACKGROUND_Z));
	pRectangle->setVertexArray( pVertices.get() );

	// Set the color
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array();
	pColors->push_back(osg::Vec4(0.898, 0.898, 0.898, 1.0));
	pRectangle->setColorArray( pColors.get() );
	pRectangle->setColorBinding( osg::Geometry::BIND_OVERALL );
	pRectangle->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, 4) );
	
	return pRectangle.release();
}

osg::Geometry* osgtools::Plot::createMajorAxisGridLines()
{
	osg::ref_ptr<osg::Geometry> pLines = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> pVertices = new osg::Vec3Array();


	// Create the x axis major grid lines
	if (_majorAxisGrid[0] > 0) {

		float xVal = _range[0] - _majorAxisGrid[0] * (int)(_range[0]/_majorAxisGrid[0]);
		xVal = (_range[0] <= 0 ? _range[0] - xVal : _range[0] + (_majorAxisGrid[0] - xVal));

		while (xVal < _range[2]) {
			// Add the major line
			pVertices->push_back(osg::Vec3( getXValuePixel(xVal), _plotDim[1], MAJORMINORAXES_Z));
			pVertices->push_back(osg::Vec3( getXValuePixel(xVal), _plotDim[1] + _plotDim[3], MAJORMINORAXES_Z));
			
			// Update x
			xVal += _majorAxisGrid[0];
		}
	}
		// Create the y axis major grid lines
	if (_majorAxisGrid[1] > 0) {

		float yVal = _range[1] - _majorAxisGrid[1] * (int)(_range[1]/_majorAxisGrid[1]);
		yVal = (_range[1] <= 0 ? _range[1] - yVal : _range[1] + (_majorAxisGrid[1] - yVal));

		while (yVal < _range[3]) {
			// Add the major line
			pVertices->push_back(osg::Vec3( _plotDim[0], getYValuePixel(yVal), MAJORMINORAXES_Z));
			pVertices->push_back(osg::Vec3( _plotDim[0] + _plotDim[2], getYValuePixel(yVal), MAJORMINORAXES_Z));
			
			// Update y
			yVal += _majorAxisGrid[1];
		}
	}
	
	// Set the vertices
	pLines->setVertexArray( pVertices.get() );

	// Set the color
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array();
	pColors->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	pLines->setColorArray( pColors.get() );
	pLines->setColorBinding( osg::Geometry::BIND_OVERALL );

	// Set the line width
	osg::ref_ptr<osg::LineWidth> pLineWidth = new osg::LineWidth();
	pLineWidth->setWidth(2.0);
	pLines->getOrCreateStateSet()->setAttributeAndModes( pLineWidth.get(), osg::StateAttribute::ON );

	pLines->addPrimitiveSet( new osg::DrawArrays(GL_LINES, 0, pVertices->getNumElements() ));
	
	return pLines.release();
}

osg::Geometry* osgtools::Plot::createMinorAxisGridLines()
{
	osg::ref_ptr<osg::Geometry> pLines = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> pVertices = new osg::Vec3Array();

	// Create the x axis minor grid lines
	if (_minorAxisGrid[0] > 0) {

		float xVal = _range[0] - _majorAxisGrid[0] * (int)(_range[0]/_majorAxisGrid[0]);
		xVal = (_range[0] <= 0 ? _range[0] - xVal : _range[0] + (_majorAxisGrid[0] - xVal));

		while (xVal < _range[2]) {
			// Add the minor line
			pVertices->push_back(osg::Vec3( getXValuePixel(xVal), _plotDim[1], MAJORMINORAXES_Z));
			pVertices->push_back(osg::Vec3( getXValuePixel(xVal), _plotDim[1] + _plotDim[3], MAJORMINORAXES_Z));
			
			// Update x
			xVal += _minorAxisGrid[0];
		}
	}

		// Create the y axis minor grid lines
	if (_minorAxisGrid[1] > 0) {

		float yVal = _range[1] - _majorAxisGrid[1] * (int)(_range[1]/_majorAxisGrid[1]);
		yVal = (_range[1] <= 0 ? _range[1] - yVal : _range[1] + (_majorAxisGrid[1] - yVal));

		while (yVal < _range[3]) {
			// Add the minor line
			pVertices->push_back(osg::Vec3( _plotDim[0], getYValuePixel(yVal), MAJORMINORAXES_Z));
			pVertices->push_back(osg::Vec3( _plotDim[0] + _plotDim[2], getYValuePixel(yVal), MAJORMINORAXES_Z));
			
			// Update y
			yVal += _minorAxisGrid[1];
		}
	}
	
	// Set the vertices
	pLines->setVertexArray( pVertices.get() );

	// Set the color
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array();
	pColors->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	pLines->setColorArray( pColors.get() );
	pLines->setColorBinding( osg::Geometry::BIND_OVERALL );

	// Set the line width
	osg::ref_ptr<osg::LineWidth> pLineWidth = new osg::LineWidth();
	pLineWidth->setWidth(1.0);
	pLines->getOrCreateStateSet()->setAttributeAndModes( pLineWidth.get(), osg::StateAttribute::ON );

	pLines->addPrimitiveSet( new osg::DrawArrays(GL_LINES, 0, pVertices->getNumElements() ));
	
	return pLines.release();
}

osgText::Text* osgtools::Plot::createXLabel()
{
	osg::ref_ptr<osgText::Text> pLabel = new osgText::Text();

	pLabel->setCharacterSize(20);
	pLabel->setFont("/fonts/arial.ttf");
	pLabel->setText(_xLabel.c_str());
	pLabel->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
	pLabel->setAxisAlignment(osgText::Text::SCREEN);
	pLabel->setColor(osg::Vec4(0, 0, 0, 1.0));
	pLabel->setDrawMode(osgText::Text::TEXT);

	// Get the location
	int labelWidth = 7 * _xLabel.size();
	int xPos = _plotDim[0] + (_plotDim[2]/2) - (labelWidth/2);
	pLabel->setPosition(osg::Vec3(xPos, 10, TEXT_Z));
	
	return pLabel.release();
}

osgText::Text* osgtools::Plot::createYLabel()
{
	osg::ref_ptr<osgText::Text> pLabel = new osgText::Text();

	pLabel->setCharacterSize(20);
	pLabel->setFont("/fonts/arial.ttf");
	pLabel->setText(_yLabel.c_str());
	pLabel->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
	pLabel->setAxisAlignment(osgText::Text::SCREEN);
	pLabel->setColor(osg::Vec4(0, 0, 0, 1.0));
	pLabel->setDrawMode(osgText::Text::TEXT);
	pLabel->setRotation(osg::Quat(3.14195/2, osg::Vec3(0.0, 0.0, 1.0)));

	// Get the location
	int labelWidth = 7 * _yLabel.size();
	int yPos = _plotDim[1] + (_plotDim[3]/2) - (labelWidth/2);
	pLabel->setPosition(osg::Vec3(15, yPos, TEXT_Z));
	
	return pLabel.release();
}

osg::Geometry* osgtools::Plot::createTickMarks()
{
	osg::ref_ptr<osg::Geometry> pLines = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> pVertices = new osg::Vec3Array();


	// Create the x axis major grid lines
	if (_majorAxisGrid[0] > 0) {

		float xVal = _range[0] - _majorAxisGrid[0] * (int)(_range[0]/_majorAxisGrid[0]);
		xVal = (_range[0] <= 0 ? _range[0] - xVal : _range[0] + (_majorAxisGrid[0] - xVal));

		while (xVal < _range[2]) {
			// Add the major line
			pVertices->push_back(osg::Vec3( getXValuePixel(xVal), _plotDim[1], MAJORMINORAXES_Z));
			pVertices->push_back(osg::Vec3( getXValuePixel(xVal), _plotDim[1] - 5, MAJORMINORAXES_Z));
			
			// Update x
			xVal += _majorAxisGrid[0];
		}
	}

		// Create the y axis major grid lines
	if (_majorAxisGrid[1] > 0) {

		float yVal = _range[1] - _majorAxisGrid[1] * (int)(_range[1]/_majorAxisGrid[1]);
		yVal = (_range[1] <= 0 ? _range[1] - yVal : _range[1] + (_majorAxisGrid[1] - yVal));

		while (yVal < _range[3]) {
			// Add the major line
			pVertices->push_back(osg::Vec3( _plotDim[0], getYValuePixel(yVal), MAJORMINORAXES_Z));
			pVertices->push_back(osg::Vec3( _plotDim[0] - 5, getYValuePixel(yVal), MAJORMINORAXES_Z));
			
			// Update y
			yVal += _majorAxisGrid[1];
		}
	}
	
	// Set the vertices
	pLines->setVertexArray( pVertices.get() );

	// Set the color
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array();
	pColors->push_back(osg::Vec4(0.0, 0.0, 0.0, 1.0));
	pLines->setColorArray( pColors.get() );
	pLines->setColorBinding( osg::Geometry::BIND_OVERALL );

	// Set the line width
	osg::ref_ptr<osg::LineWidth> pLineWidth = new osg::LineWidth();
	pLineWidth->setWidth(2.0);
	pLines->getOrCreateStateSet()->setAttributeAndModes( pLineWidth.get(), osg::StateAttribute::ON );

	pLines->addPrimitiveSet( new osg::DrawArrays(GL_LINES, 0, pVertices->getNumElements() ));
	
	return pLines.release();
}

void osgtools::Plot::createAddXLabels( osg::Geode* pGeode, std::vector<osg::ref_ptr<osgText::Text>>& xLabelsGeo )
{
	// Make sure the geode is good
	if (!pGeode)
		return;

	// Use values if no strings are available
	bool bValues = ( _xLabels.size() == 0 ? true : false);
	if (bValues) _xLabels.clear();

	// Clear the values
	if (xLabelsGeo.size() > 0 ) {
		for (int i=0; i < xLabelsGeo.size(); i++) {
			// Remove the label
			pGeode->removeDrawable(xLabelsGeo[i].release());
		}
		xLabelsGeo.clear();
	}

	// Create new labels
	if (_majorAxisGrid[0] > 0) {
		
		float xVal = _range[0] - _majorAxisGrid[0] * (int)(_range[0]/_majorAxisGrid[0]);
		xVal = (_range[0] <= 0 ? _range[0] - xVal : _range[0] + (_majorAxisGrid[0] - xVal));

		int i = 0;
		while (xVal < _range[2]) {
			std::stringstream s;

			if (bValues)
				s << xVal;
			else {
				if (i < _xLabels.size())
					s << _xLabels[i];
				i++;
			}

			osg::ref_ptr<osgText::Text> pLabel = new osgText::Text();

			pLabel->setCharacterSize(16);
			pLabel->setFont("/fonts/arial.ttf");
			pLabel->setText(s.str().c_str());
			pLabel->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
			pLabel->setAxisAlignment(osgText::Text::SCREEN);
			pLabel->setColor(osg::Vec4(0, 0, 0, 1.0));
			pLabel->setDrawMode(osgText::Text::TEXT);

			// Get the location
			int labelWidth = 7 * s.str().size();
			int xPos = getXValuePixel(xVal) - (labelWidth/2);
			pLabel->setPosition(osg::Vec3(xPos, _plotDim[1] - 15, 0));

			// Add the label
			xLabelsGeo.push_back(pLabel);
			pGeode->addDrawable(pLabel);

			// Update x
			xVal += _majorAxisGrid[0];
		}
	}
	
	// Update the geode
	pGeode->dirtyBound();
}

void osgtools::Plot::createAddYLabels( osg::Geode* pGeode, std::vector<osg::ref_ptr<osgText::Text>>& yLabelsGeo )
{
	// Make sure the geode is good
	if (!pGeode)
		return;

	// Use values if no strings are available
	bool bValues = ( _yLabels.size() == 0 ? true : false);
	if (bValues) _yLabels.clear();

	// Clear the values
	if (yLabelsGeo.size() > 0 ) {
		for (int i=0; i < yLabelsGeo.size(); i++) {
			// Remove the label
			pGeode->removeDrawable(yLabelsGeo[i].release());
		}
		yLabelsGeo.clear();
	}

	// Create new labels
	if (_majorAxisGrid[1] > 0) {
		
		float yVal = _range[1] - _majorAxisGrid[1] * (int)(_range[1]/_majorAxisGrid[1]);
		yVal = (_range[1] <= 0 ? _range[1] - yVal : _range[1] + (_majorAxisGrid[1] - yVal));

		int i = 0;
		while (yVal < _range[3]) {
			std::stringstream s;

			if (bValues)
				s << yVal;
			else {
				if (i < _yLabels.size())
					s << _yLabels[i];
				i++;
			}

			osg::ref_ptr<osgText::Text> pLabel = new osgText::Text();

			pLabel->setCharacterSize(16);
			pLabel->setFont("/fonts/arial.ttf");
			pLabel->setText(s.str().c_str());
			pLabel->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
			pLabel->setAxisAlignment(osgText::Text::SCREEN);
			pLabel->setColor(osg::Vec4(0, 0, 0, 1.0));
			pLabel->setDrawMode(osgText::Text::TEXT);

			// Get the location
			int labelWidth = 3 * s.str().size();
			int yPos = getYValuePixel(yVal);
			pLabel->setPosition(osg::Vec3(_plotDim[0] - labelWidth - 15, yPos, 0));

			// Add the label
			yLabelsGeo.push_back(pLabel);
			pGeode->addDrawable(pLabel);

			// Update y
			yVal += _majorAxisGrid[1];
		}
	}
	
	// Update the geode
	pGeode->dirtyBound();
}

void osgtools::Plot::resetAxisLabels()
{
	createAddXLabels(_pPlotGeode.get(), _xLabelsGeo);
	createAddYLabels(_pPlotGeode.get(), _yLabelsGeo);
}

void osgtools::Plot::resize( int width, int height )
{
	if (width < 0 || height < 0)
		return;
	
	// Update the dimensions
	_width = width;
	_height = height;
	
	// Set the projection
	setProjectionMatrix(osg::Matrix::ortho2D(0,_width,0,_height));
	setViewport(0,0,_width,_height);

	// Set the view
	setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	setViewMatrix(osg::Matrix::identity());

	// Clear the depth
	setClearMask(GL_DEPTH_BUFFER_BIT);

	// Disable lighting
	getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
}

void osgtools::Plot::setRange( float xmin, float ymin, float xmax, float ymax )
{
	_range[0] = xmin;
	_range[1] = ymin;
	_range[2] = xmax;
	_range[3] = ymax;

	// Update the grid lines
	setMajorAxisGrid(_majorAxisGrid[0], _majorAxisGrid[1]);
	setMinorAxisGrid(_minorAxisGrid[0], _minorAxisGrid[1]);	

	// Update the axis labels
	createAddXLabels(_pPlotGeode.get(), _xLabelsGeo);
	createAddYLabels(_pPlotGeode.get(), _yLabelsGeo);

	// Update subclass functions
	redraw();
}

void osgtools::Plot::setMajorAxisGrid( float xGrid, float yGrid )
{
	if (xGrid < 0 || yGrid < 0)
		return;
	_majorAxisGrid[0] = xGrid;
	_majorAxisGrid[1] = yGrid;

	// Remove the drawables and refresh
	_pPlotGeode->removeDrawable( _pMajorAxisGridLinesGeo.release() );
	_pPlotGeode->removeDrawable( _pTickLinesGeo.release() );
	
	// Create new grid lines
	_pMajorAxisGridLinesGeo = createMajorAxisGridLines();
	_pTickLinesGeo = createTickMarks();

	// Add
	_pPlotGeode->addDrawable( _pMajorAxisGridLinesGeo.get() );
	_pPlotGeode->addDrawable( _pTickLinesGeo.get() );

	// Reset the labels
	resetAxisLabels();

	_pPlotGeode->dirtyBound();
}

void osgtools::Plot::setMinorAxisGrid( float xGrid, float yGrid )
{
	if (xGrid < 0 || yGrid < 0)
		return;
	_minorAxisGrid[0] = xGrid;
	_minorAxisGrid[1] = yGrid;

	// Remove the drawables and refresh
	_pPlotGeode->removeDrawable( _pMinorAxisGridLinesGeo.release() );
	
	// Create new grid lines
	_pMinorAxisGridLinesGeo = createMinorAxisGridLines();

	// Add
	_pPlotGeode->addDrawable( _pMinorAxisGridLinesGeo.get() );
	_pPlotGeode->dirtyBound();
}

int osgtools::Plot::getXValuePixel( float x )
{
	if (x < _range[0] || x > _range[2])
		return -1;
	float xVal = x - _range[0];
	float pxPerUnit = (float)_plotDim[2]/(_range[2] - _range[0]);
	return _plotDim[0] + (xVal * pxPerUnit);
}

int osgtools::Plot::getYValuePixel( float y )
{
	if (y < _range[1] || y > _range[3])
		return -1;
	float yVal = y - _range[1];
	float pxPerUnit = (float)_plotDim[3]/(_range[3] - _range[1]);
	return _plotDim[1] + (yVal * pxPerUnit);
}

void osgtools::Plot::getRange( float& xmin, float& ymin, float& xmax, float& ymax )
{
	xmin = _range[0];
	ymin = _range[1];
	xmax = _range[2];
	ymax = _range[3];
}

void osgtools::Plot::getPlotDimensions( int& startX, int& startY, int& width, int& height )
{
	startX = _plotDim[0];
	startY = _plotDim[1];
	width = _plotDim[2];
	height = _plotDim[3];
}