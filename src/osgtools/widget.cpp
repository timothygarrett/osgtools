/*
	widget.h
	2D widget for OSG
	
	Tim Garrett (garrettt@iastate.edu)
	2015.07.02
*/

#include "widget.h"

osgtools::Widget::Widget( int windowWidth, int windowHeight, float x, float y, int width, int height ) :
	_windowWidth( windowWidth ),
	_windowHeight( windowHeight ),
	_x(x),
	_y(y),
	_width(width),
	_height(height)
{
	// Create the HUD camera
	_camera = createCamera();
	osg::Switch::addChild( _camera.get() );

	// Create the background
	//_background = createGeode();
	//addChild( _background.get() );
}


bool osgtools::Widget::setBackgroundImage( std::string& filePath )
{
	// Make sure the camera exists
	if (!_camera.valid()) {
		_camera = createCamera();
		osg::Switch::addChild( _camera.get() );
	}
	
	// Make sure the background exists
	if (!_background.valid()) {
		_background = createGeode();
		addChild( _background.get() );
	}		
	if (!_backgroundTexture.valid())
		_backgroundTexture = new osg::Texture2D;
	
	// Attempt to load image
	_backgroundImage = osgDB::readImageFile( filePath );
	if (!_backgroundImage.valid())
		return false;

	// Create the stream
	_backgroundImageStream = dynamic_cast<osg::ImageStream*>( _backgroundImage.get() );
	if (_backgroundImageStream.valid())
		_backgroundImageStream->play();
	
	// Set the image
	_backgroundTexture->setImage( _backgroundImage.get() );
	
	osg::ref_ptr<osg::StateSet> stateset = _background->getOrCreateStateSet();
	
	// Assign the texture to unit 0
	stateset->setTextureAttributeAndModes(0, _backgroundTexture.get(), osg::StateAttribute::ON);
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	
	return true;
}

osg::Camera* osgtools::Widget::createCamera()
{
	osg::ref_ptr<osg::Camera> pCamera = new osg::Camera();
	
	// Set the projection
	pCamera->setProjectionMatrix(osg::Matrix::ortho2D(0,_windowWidth,0,_windowHeight));
	pCamera->setViewport(0,0,_windowWidth,_windowHeight);

	// Set the view
	pCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	pCamera->setViewMatrix(osg::Matrix::identity());

	// Clear the depth
	pCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

	return pCamera.release();
}

osg::Geode* osgtools::Widget::createGeode()
{
	osg::ref_ptr<osg::Geode> pGeode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> pRectangle = new osg::Geometry();

	osg::ref_ptr<osg::Vec3Array> pVecArray = new osg::Vec3Array();
	pVecArray->push_back( osg::Vec3(_x, _y, 0) );
	pVecArray->push_back( osg::Vec3(_x + _width, _y, 0) );
	pVecArray->push_back( osg::Vec3(_x + _width, _y + _width, 0) );
	pVecArray->push_back( osg::Vec3(_x, _y + _width, 0) );
	pRectangle->setVertexArray( pVecArray.get() );

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	colors->push_back(osg::Vec4(1,1,1,0));
	pRectangle->setColorArray( colors.get() );
	pRectangle->setColorBinding( osg::Geometry::BIND_OVERALL );
	pRectangle->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, 4) );

	osg::ref_ptr<osg::Vec2Array> pTextureCoords = new osg::Vec2Array(4);
	pTextureCoords->at(0).set(0.0, 0.0);
	pTextureCoords->at(1).set(1.0, 0.0);
	pTextureCoords->at(2).set(1.0, 1.0);
	pTextureCoords->at(3).set(0.0, 1.0);
	pRectangle->setTexCoordArray(0, pTextureCoords.get());
	
	pGeode->addDrawable( pRectangle.get() );

	return pGeode.release();
}

bool osgtools::Widget::addChild( osg::Node* pChild )
{
	if (!_camera.valid())
		return false;
	return _camera->addChild( pChild );
}