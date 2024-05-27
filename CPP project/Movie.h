#pragma once
#include "Config.h"
#include "string"

class WidgetMovie;

/*
	Class Movie implements draw, update and init functions.
	The class also has set and get methods that set/return the movie's specs.
*/

class Movie {

protected:
	std::string director, stars, year, title, genre, summary, pic_name;
	int nyear = 0;

public:
	WidgetMovie* wm;
	//constructor
	Movie(const std::string _title, const std::string _genre, const std::string _director, 
		const std::string _stars, const std::string _year, const std::string _summary, const std::string _pic_name);

	// default constructor
	Movie() {
		title = "";
		genre = "";
		director = "";
		stars = "";
		year = "";
		summary = "";
		pic_name = "";
		wm = NULL;
	}
	~Movie();

	void update();
	void draw();
	void init(); // creates the widgetmovie pointer and initialize the brush with the movie image as texture

	
	void SetGenre(std::string _genre); //set the genre
	bool isGenre(std::string _genre); // returns if is _genre 

	void SetYear(std::string _year); // set the year in string format
	std::string  GetYear() const { return year; } // return the year in string format
	int GetYear() { return nyear; } // return the year in number format
	bool isYear(int _nyear) { return nyear == _nyear; }//returns if is _nyear

	void SetTitle(std::string _title); //set the title
	std ::string GetTitle() { return title; } //return the title
	void SetDirector(std::string _director); // set the director
	void SetStars(std::string _stars); // set the stars
	void SetPicName(std::string _pic_name); // set the picture's path
	void SetSummary(std::string _summary); // set the summary
};
