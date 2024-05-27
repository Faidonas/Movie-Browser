#pragma once

#include <list>
#include "graphics.h"
#include "Movie.h"
#include "gui.h"

class MovieBrowser {

protected:
	bool movies_initialized = false;
	bool genre_given = false;
	std::list<Movie*> movie_list; // basic movie list
	std::list<Movie*> filtered_movie_list;// new list with filters on
	std::list<FILTER*> filter_list; // the filters applied so far
	graphics::Brush br_bgnd; // brush for the background
	RightArrow* ra = nullptr; // pointer for right arrow
	LeftArrow* la = nullptr;// pointer for left arrow
	Dock* dock = nullptr;//pointer for the dock
	int nMovies = 0, nScroll = 0;

	// utility function to delete memory allocated in filter_list
	void DestroyFilterList();

public:
	void init(); // one time init of the MovieBrowser class

	// first time: read txt file and create the movie_list database; create all widgets (dock + controls)
	// else: update widgets and dock
	void update();

	// draw all filtered_movie_list movies
	void draw();

	// scroll one position to the right
	void scroll_right();

	// scroll one position to the left
	void scroll_left();

	// set one and only one filter to the movie_list
	void SetFilter(_FILTER_TYPE filter, const std::string _genre_param = "", int _year_param = -1);

	// add a new filter to the filtered_movie_list
	void AddFilter(_FILTER_TYPE filter, const std::string _genre_param = "", int _year_param = -1);
	
	// clear a filter from the filtered_movie_list
	void ClearFilter(_FILTER_TYPE filter, const std::string _genre_param = "", int _year_param = -1);
	
	// check if the filter already exists in the filter_list
	bool FilterExist(_FILTER_TYPE filter, const std::string _genre_param = "", int _year_param = -1);

	MovieBrowser(); // default constructor
	~MovieBrowser(); // default deconstructor; will release memory allocated to the pointers in the three lists, end will kill the dock and all controls
};
