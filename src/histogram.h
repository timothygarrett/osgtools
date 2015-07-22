/*
	histogram.h
	Histogram plot for OSG
	
	Tim Garrett (garrettt@iastate.edu)
	2015.06.16
*/

#pragma once

// OSG
#include <osg/Geode>
#include <osg/Geometry>

// STL
#include <vector>

// Local
#include "plot.h"

namespace osgtools {
	
	class Histogram : public Plot {
	protected:
		std::vector<float> _bins;
		std::vector<float> xValues;
		std::vector<float> yValues;

		osg::ref_ptr<osg::Geode> _pGeode;
		
		std::vector<osg::ref_ptr<osg::Geometry>> _histogramGeo;
	public:
		Histogram() {}
		Histogram( int width, int height );
				
		bool setHistogram( std::vector<float>& bins );
		
		/*!
		 *	Redraw function for subclasses
		 */
		virtual void redraw();

		/*!
		 *	Automatically update data labels
		 */
		void autoUpdateMajorMinorAxes();
	};
}