/*
	busywidget.h
	2D busy cursor widget for OSG
	
	Tim Garrett (garrett@iastate.edu)
	2015.07.02
*/
#pragma once

// STL
#include <string>

// Local
#include "widget.h"

namespace osgtools {
	
	class BusyWidget : public Widget {
		
		std::string _sImgPath;					/*!< Path for the busy cursor image */


		
	public:
		BusyWidget() {}
		BusyWidget( int width, int height );
		BusyWidget( int width, int height, std::string& imagePath );
	};
	
}