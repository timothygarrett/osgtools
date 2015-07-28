/*
	widget.h
	2D widget for OSG
	
	Tim Garrett (garrettt@iastate.edu)
	2015.07.02
*/
#pragma once

// OSG
#include <osg/ref_ptr>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/Switch>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/ImageStream>
#include <osg/Switch>
#include <osg/Camera>


namespace osgtools {
	
	class Widget : public osg::Switch {
	protected:
		int _windowWidth;			/*!< Width of the window in pixels */
		int _windowHeight;			/*!< Height of the window in pixels */
		int _width;					/*!< Width of the widget in pixels */
		int _height;				/*!< Height of the widget in pixels */
		float _x;					/*!< x-coordinate of the widget */
		float _y;					/*!< y-coordinate of the widget */
		
		osg::ref_ptr<osg::Camera> _camera;						/*!< HUD camera */

		osg::ref_ptr<osg::Geode> _background;					/*!< OSG geode node */
		osg::ref_ptr<osg::Texture2D> _backgroundTexture;		/*!< OSG background texture */
		osg::ref_ptr<osg::Image> _backgroundImage;				/*!< Background image */
		osg::ref_ptr<osg::ImageStream> _backgroundImageStream;	/*!< Animated background */

		/*!
		 *	Creates a geode for the widget
		 *	\return	A pointer to a geode
		 */
		osg::Geode* createGeode();

		/*!
		 *	Creates the HUD camera
		 *	\return A pointer to a camera
		 */
		osg::Camera* createCamera();
		
	public:
		Widget() {}
		Widget( int windowWidth, int windowHeight, float x, float y, int width, int height );
		
		
		// Getters 
		int getWidth() { return _width; }
		int getHeight() { return _height; }
		int getX() { return _x; }
		int getY() { return _y; }
		
		// Setters
		void setWidth(int width) { _width = width; }
		void setHeight(int height) { _height = height; }
		void setX( float x ) { _x = x; }
		void setY( float y ) { _y = y; }
		
		bool setBackgroundImage( std::string& filePath );
		void show() { setAllChildrenOn(); }
		void hide() { setAllChildrenOff(); }
		
		bool addChild( osg::Node* pChild );
		
		
	};	
}