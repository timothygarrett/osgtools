# osgtools project

# Tim Garrett (garrettt@iastate.edu)
# 2015.07.22


# Find OSG
find_package( OpenSceneGraph REQUIRED )
FIND_PACKAGE( osg REQUIRED )
FIND_PACKAGE( osgDB REQUIRED )
FIND_PACKAGE( osgGA REQUIRED )
FIND_PACKAGE( osgViewer REQUIRED )
FIND_PACKAGE( osgUtil REQUIRED )
FIND_PACKAGE( osgAnimation REQUIRED )
FIND_PACKAGE( osgText REQUIRED )

# Includes
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

# OSG Libraries
set(OSG_LIBS
	debug ${OSG_LIBRARY_DEBUG}
	debug ${OSGGA_LIBRARY_DEBUG}
	debug ${OSGDB_LIBRARY_DEBUG}
	debug ${OSGVIEWER_LIBRARY_DEBUG}
	debug ${OSGUTIL_LIBRARY_DEBUG}
	debug ${OSGANIMATION_LIBRARY_DEBUG}
	debug ${OPENTHREADS_LIBRARY_DEBUG}
	debug ${OSGTEXT_LIBRARY_DEBUG}
	optimized ${OSG_LIBRARY}
	optimized ${OSGGA_LIBRARY}
	optimized ${OSGDB_LIBRARY}
	optimized ${OSGVIEWER_LIBRARY}
	optimized ${OSGUTIL_LIBRARY}
	optimized ${OSGANIMATION_LIBRARY}
	optimized ${OPENTHREADS_LIBRARY}
	optimized ${OSGTEXT_LIBRARY}
)

set(MAIN_SRC
	osgtools.h
)

set(UI_SRC
	histogram.h
	histogram.cpp
	plot.h
	plot.cpp
	curtainwidget.h
	curtainwidget.cpp
	widget.h
	widget.cpp
	busywidget.h
	busywidget.cpp
)

# Create source groups
source_group(main FILES ${MAIN_SRC})
source_group(gui FILES ${UI_SRC})

# Add the source to the exe
add_library( osgtools SHARED
	${MAIN_SRC}
	${UI_SRC}
)

# Link to the libraries
target_link_libraries( osgtools 
	${OSG_LIBS}
)

# Set the debug postfix
set_target_properties( osgtools PROPERTIES DEBUG_POSTFIX "_d")
