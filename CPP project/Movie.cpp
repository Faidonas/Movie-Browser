#include "Movie.h"
#include "graphics.h"
#include <fstream>
#include <string>
#include <iostream>
#include "gui.h"

using namespace std;

// remove white spaces left/right of str
void TrimSting(std::string& str)
{
	const char* typeOfWhitespaces = " ";
	str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
	str.erase(0, str.find_first_not_of(typeOfWhitespaces));
}

Movie::Movie(const std::string _title, const std::string _genre, const std::string _director, const std::string _stars, const std::string _year, const std::string _summary, const std::string _pic_name)
{
	title = _title;
	genre = _genre;
	director = _director;
	stars = _stars;
	year = _year;
	summary = _summary;
	pic_name = _pic_name;
	wm = NULL;
}


Movie::~Movie()
{
	if (wm != NULL)
		delete wm;
}

void Movie::update()
{
	graphics::MouseState ms;
	graphics::getMouseState(ms);
	wm->setHighlighted(wm->contains(ms.cur_pos_x, ms.cur_pos_y));// if the cursor is in the widgetmovie, highlight it
}


void Movie::draw()
{
	if (!wm)
		return; // not ready to draw yet

	wm->draw();	

	if (wm->isHightlighted()) { //if the cursor is on the widget of the movie; draw specs 
		graphics::Brush brush;
		brush.outline_opacity = 1.f;
		graphics::setFont(std::string(ASSET_PATH) + "OpenSans-Semibold.ttf");
		drawText(TEXT_LEFT, TEXT_BOTTOM+20, TEXT_SIZE_TITLE, title, brush);
		graphics::setFont(std::string(ASSET_PATH) + "OpenSans-Regular.ttf");
		drawText(TEXT_LEFT, TEXT_BOTTOM + 2 * TEXT_SIZE_LINE, TEXT_SIZE_OTHER, "Genre:              " + genre, brush);//14 spaces
		drawText(TEXT_LEFT, TEXT_BOTTOM + 3 * TEXT_SIZE_LINE, TEXT_SIZE_OTHER, "Director:        " + director, brush);
		drawText(TEXT_LEFT, TEXT_BOTTOM + 4 * TEXT_SIZE_LINE, TEXT_SIZE_OTHER, "Stars:                " + stars, brush);
		drawText(TEXT_LEFT, TEXT_BOTTOM + 5 * TEXT_SIZE_LINE, TEXT_SIZE_OTHER, "Year:                  " + year, brush);
		drawText(TEXT_LEFT, TEXT_BOTTOM + 7 * TEXT_SIZE_LINE, TEXT_SIZE_OTHER, "Summary:", brush);
		// draw multiline summary
		int nLine = 0;
		const int nLineChars = 90;
		graphics::setFont(std::string(ASSET_PATH) + "OpenSans-Regular.ttf");
		for (int nChar = 0; nChar < summary.length(); nChar += nLineChars)
		{
			string oneline; oneline.resize(nLineChars);
			summary.copy((char*)oneline.c_str(), nLineChars, nChar);
			drawText(TEXT_LEFT, TEXT_BOTTOM+170.f + (2.f + nLine++) * TEXT_SIZE_LINE, TEXT_SIZE_OTHER-3, oneline, brush);
		}
	}
}

void Movie::init()
{
	if (wm != NULL) // make sure wm exists before deleting it
		delete wm;
	
	// start a new WidgetMovie
	wm = new WidgetMovie;
	wm->SetMovie(this);
	
	// set up the brush
	graphics::Brush br;
	br.texture = ASSET_PATH + pic_name;
	wm->SetBrush(br);
}



void Movie::SetGenre(std::string _genre)
{
	int leftpos = _genre.find(":"); //trim the line to get only the genres of the film
	genre.resize(_genre.length() - leftpos + 1);
	_genre.copy((char*)genre.c_str(), _genre.length() - leftpos, leftpos+1);
	TrimSting(genre);
}

bool Movie::isGenre(std::string _genre)
{
	std::string uppergenre = genre;
	_strupr_s((char*)uppergenre.c_str(), uppergenre.length());
	return uppergenre.find(_genre) != std::string::npos;
}

void Movie::SetYear(std::string _year)
{
	int leftpos = _year.find(":"); //trim the line to get only the year of the film
	year.resize(_year.length() - leftpos + 1);
	_year.copy((char*)year.c_str(), _year.length() - leftpos, leftpos + 1);
	TrimSting(year);
	nyear = (int)strtol((char*)year.c_str(), nullptr, 10);
}

void Movie::SetTitle(std::string _title)
{
	title = _title;
}

void Movie::SetDirector(std::string _director)
{
	int leftpos = _director.find(":");//trim the line to get only the year of the director
	director.resize(_director.length() - leftpos + 1);
	_director.copy((char*)director.c_str(), _director.length() - leftpos, leftpos + 1);
	TrimSting(director);
	
}

void Movie::SetStars(std::string _stars)
{
	int leftpos = _stars.find(":");//trim the line to get only the stars of the film
	stars.resize(_stars.length() - leftpos + 1);
	_stars.copy((char*)stars.c_str(), _stars.length() - leftpos, leftpos + 1);
	TrimSting(stars);
}

void Movie::SetPicName(std::string _pic_name)
{
	pic_name = _pic_name;
}

void Movie::SetSummary(std::string _summary)
{
	int leftpos = _summary.find(":");//trim the line to get only the summary of the film
	summary.resize(_summary.length() - leftpos + 1);
	_summary.copy((char*)summary.c_str(), _summary.length() - leftpos, leftpos + 1);
	TrimSting(summary);
}
