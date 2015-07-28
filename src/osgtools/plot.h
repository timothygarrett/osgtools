/*
	Plot.h
	Creates a plot in OSG
	
	Tim Garrett (garrettt@iastate.edu)
	2015.06.16
*/

#pragma once

// STL
#include <string>

// OSG
#include <osg/Camera>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/LineWidth>
#include <osg/ref_ptr>
#include <osgText/Text>

// Local
#include "osgtools.h"

namespace osgtools {

	class OSGTOOLS Plot : public osg::Camera {

	protected:

		int _width;
		int _height;

		bool _bInitialized;

		int _range[4];								/*!<	Cartesian plot range: (-x, -y, +x, +y)	*/
		float _majorAxisGrid[2];
		float _minorAxisGrid[2];

		std::string _xLabel;
		std::string _yLabel;

		std::vector< std::string > _xLabels;
		std::vector< std::string > _yLabels;

		osg::ref_ptr<osg::Geode> _pPlotGeode;

		int _plotDim[4];							/*!<	Plot dimensions in pixels: (start x, start y, width x, height y) */

		// Drawables
		osg::ref_ptr<osg::Geometry> _pBackgroundGeo;
		osg::ref_ptr<osg::Geometry> _pMajorAxisGridLinesGeo;
		osg::ref_ptr<osg::Geometry> _pMinorAxisGridLinesGeo;
		osg::ref_ptr<osgText::Text> _pXLabelGeo;
		osg::ref_ptr<osgText::Text> _pYLabelGeo;
		osg::ref_ptr<osg::Geometry> _pTickLinesGeo;
		std::vector<osg::ref_ptr<osgText::Text>> _xLabelsGeo;
		std::vector<osg::ref_ptr<osgText::Text>> _yLabelsGeo;
		

		// Constants
		static const int BOTTOM_SPACING;
		static const int TOP_SPACING;
		static const int LEFT_SPACING;
		static const int RIGHT_SPACING;

		static const float BACKGROUND_Z;
		static const float MAJORMINORAXES_Z;
		static const float TEXT_Z;

		/*!
		*	Initializes the plot
		*/
		void initialize();

		/*!
		 *	Creates the background
		 */
		osg::Geometry* createBackground();

		/*!
		 *	Creates the major axis grid lines
		 */
		osg::Geometry* createMajorAxisGridLines();

		/*!
		 *	Create the minor axis grid lines
		 */
		osg::Geometry* createMinorAxisGridLines();

		/*!
		 *	Create the x axis label
		 */
		osgText::Text* createXLabel();

		/*!
		 *	Create the y axis label
		 */
		osgText::Text* createYLabel();

		/*!
		 *	Create tick marks
		 */
		osg::Geometry* createTickMarks();

		/*!
		 *	Create the labels for the x axis
		 */
		void createAddXLabels( osg::Geode* pGeode, std::vector<osg::ref_ptr<osgText::Text>>& xLabelsGeo );

		/*!
		 *	Create the labels for the y axis
		 */
		void createAddYLabels( osg::Geode* pGeode, std::vector<osg::ref_ptr<osgText::Text>>& yLabelsGeo );

		/*!
		 *	Reset axis labels
		 */
		void resetAxisLabels();

		/*!
		 *	Set the major axis grid lines
		 */
		void setMajorAxisGrid( float xGrid, float yGrid );

		/*!
		 *	Set the minor axis grid lines
		 */
		void setMinorAxisGrid( float xGrid, float yGrid );

	public:
		Plot();
		Plot( int width, int height, std::string xLabel="", std::string yLabel="" );

		/*!
		*	Resizes the pixel dimensions of the plot
		*	\param	width	The width in pixels of the plot
		*	\param	height	The height in pixels of the plot
		*/
		void resize( int width, int height );

		// Getters
		/*!
		*	Gets the width of the plot in pixels
		*	\return	The width of the plot in pixels
		*/
		int getWidth() { return _width; }

		/*!
		*	Gets the height of the plot in pixels
		*	\return	The height of the plot in pixels
		*/
		int getHeight() { return _height; }

		/*!
		 *	Gets the plot coordinate system range
		 *	\param	xmin	The minimum x value
		 *	\param	ymin	The minimum y value
		 *	\param	xmax	The maximum x value
		 *	\param	ymax	The maximum y value
		 */
		void getRange( float& xmin, float& ymin, float& xmax, float& ymax );

		/*!
		 *	Gets the plot dimensions
		 *	\param startX	The starting pixel of the plot
		 *	\param startY	The starting pixel of the plot
		 *	\param	width	The width of the plot
		 *	\param	height	The height of the plot
		 */
		void getPlotDimensions( int& startX, int& startY, int& width, int& height );

		/*!
		 *	Gets the x axis label
		 *	\return the x axis label
		 */
		std::string& getXLabel() { return _xLabel; }

		/*!
		 *	Gets the y axis label
		 *	\return the y axis label
		 */
		std::string& getYLabel() { return _yLabel; }

		/*!
		 *	Get pixel for x value
		 */
		int getXValuePixel( float x );

		/*!
		 *	Get pixel for y value
		 */
		int getYValuePixel( float y );


		/// Setters
		/*!
		*	Sets the width of the plot in pixels
		*	\param	width	The new width of the plot in pixels
		*/
		void setWidth( int width ) { resize(width,_height); }

		/*!
		*	Sets the height of the plot in pixels
		*	\param	height	The new height of the plot in pixels
		*/
		void setHeight( int height ) { resize(_width,height); }

		/*!
		*	Sets the plot coordinate system range
		*	\param	xmin	The minimum x value
		*	\param	ymin	The minimum y value
		*	\param	xmax	The maximum x value
		*	\param	ymax	The maximum y value
		*/
		void setRange( float xmin, float ymin, float xmax, float ymax );

		/*!
		 *	Set the x axis label
		 *	\param	xLabel	The x axis label
		 */
		void setXLabel( std::string& xLabel ) { _xLabel = xLabel; }

		/*!
		 *	Set the y axis label
		 *	\param	yLabel	The y axis label
		 */
		void setYLabel( std::string& yLabel ) { _yLabel = yLabel; }

		/*!
		 *	Redraw function for subclasses
		 */
		virtual void redraw() {}

	};
}