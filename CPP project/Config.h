#pragma once

#include <iostream>
#include <math.h>

#define ASSET_PATH "assets\\"

// Main window/canvas size
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 800
#define CANVAS_WIDTH 1400
#define CANVAS_HEIGHT 800

// all units below in canvas units

// widget default size
#define WIDGET_WIDTH 300
#define WIDGET_HEIGHT 400

// empty space between widgets
#define WIDGET_SPACE 40 

// scroll distance for each widget
#define WIDGET_SCROLL WIDGET_WIDTH+WIDGET_SPACE

// text parameters used in dock controls and canvas
#define TEXT_LEFT 40 
#define TEXT_BOTTOM WIDGET_HEIGHT+100
#define TEXT_SIZE_LINE 28
#define TEXT_SIZE_TITLE 45
#define TEXT_SIZE_OTHER 25

// size of dock window
#define DOCK_WIDTH 2.5*WIDGET_WIDTH
#define DOCK_HEIGHT WIDGET_HEIGHT

// default button size and space in-between
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 25
#define BUTTON_SPACE 25

//Genre filters
const char ACTION[] = "ACTION";
const char CRIME[] = "CRIME";
const char ADVENTURE[] = "ADVENTURE";
const char FAMILY[] = "FAMILY";
const char SCI_FI[] = "SCI-FI";
const char COMEDY[] = "COMEDY";
const char DRAMA[] = "DRAMA";
const char BIOGRAPHY[] = "BIOGRAPHY";
const char ANIMATION[] = "ANIMATION";

//filter types; used in dock
// _DEFAULT has the same functinality with _CLEAR_ALL
enum _FILTER_TYPE {

	_DEFAULT = 100, _GENRE, _YEAR, _CLEAR_ALL
};

// FILTER class defines a set active filters
// Each filter has a type , a genre parameter and a year parameter;
class FILTER {
public:
	_FILTER_TYPE filter_type;	// < The filter type

	std::string genre_param;				// < The filter parameter for genre
	int year_param;					// < The filter parameter for year

	FILTER() {
		filter_type = _DEFAULT; genre_param = ""; year_param = -1;
	}	// < default constructor
	FILTER(_FILTER_TYPE _filter_type, const std::string _genre_param, int _year_param)   // < constructor with type and param
	{
		filter_type = _filter_type; genre_param = _genre_param; year_param = _year_param;
	}
};

// measure distance between two points
inline float distance(float x0, float y0, float x1, float y1) {
	return sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}
