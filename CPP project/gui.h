#pragma once
#include "graphics.h"
#include "Movie.h"

/*
	Widget class is a base class with the main functionallity of the widget controls.
	Supports position and size parameters, and brush.
	Each widget has a unique ID; this is implemented using the static member variable m_next_uid
*/

class Widget {

protected:
	float pos_x, pos_y, width, height; // position and size of widget
	graphics::Brush wbr; // widget brush

	bool m_highlighted = false; // is widget highlighted?

	int m_uid = 0; // unique ID of widget

	static int m_next_uid; // unique widget counter
	static int m_focus_id; // current ID of widget with focus

public:
	Widget(float x, float y, float w, float h, const graphics::Brush& br); // constructor

	// default constructor
	Widget() {
		pos_x = 0;
		pos_y = 0;
		width = 0;
		height = 0;
		m_highlighted = false;
		m_uid = m_next_uid++;
	};

	virtual void draw() = 0 {}; // it will be implemented by child classes
	virtual void update() = 0 {}; // it will be implemented by child classes

	void SetPos(float x, float y); //sets the position of the widget
	void SetSize(float w, float h); //sets the size of the widget
	void GetPos(float& x, float& y); // returns the position of the widget
	float GetPosX() const { return pos_x; } // returns the x position of the widget
	float GetPosY() const { return pos_y; } // returns the y position of the widget
	void GetSize(float& w, float& h); // returns the size of the widget
	void SetBrush(const graphics::Brush& br); //sets the brush of the widget
	void setHighlighted(bool bh); // set the widget highlighted true/false
	bool isHightlighted() const { return m_highlighted; }// highlighted or no?
	bool contains(int mouse_x, int mouse_y); // check if the mouse coords are inside the widget
	void scroll_right(); // scroll the widget right
	void scroll_left(); // scroll the widget left
	bool requestFocus(); // request the focus
	bool hasFocus(); // focus or no?
	void releaseFocus(); // release the focus
	int GetID() const { 	return m_uid;	} // return the unique id of the widget
};

/*
	WidgetMovie provides the functionality of the Movie widgets.
	The class implements draw()/update() members and adds a new Movie* member
*/

class WidgetMovie : public Widget
{
protected:
	Movie* movie;
public:
	WidgetMovie(float x, float y, float w, float h, Movie* m);// constructor

	WidgetMovie() { //  default constructor
		pos_x = 0;
		pos_y = 0;
		width = 0;
		height = 0;
		m_highlighted = false;
		movie = NULL;
	};

	// set the movie of the Widget
	void SetMovie(Movie* m) {
		movie = m;
	}

	// implement widget specific draw()/update()
	void draw();
	void update();
};

/*
	Button class provides a button control functionality. 
	Adds a pParent member to the parent Widget and control specific
	members for storing and displaying text within the button.
*/
class Button : public Widget {

protected:
	Widget* pParent = nullptr; // widget type pointer for the parent of the button
	std::string text; // string for the text of the button
	float text_offset_x = 0.f; // offsets for the position of the button's text
	float text_offset_y = 0.f; 

public:
	Button(float x, float y, float w, float h, Widget* _pParent); // constructor

	Button() { // default constructor
		pos_x = 0;
		pos_y = 0;
		width = 0;
		height = 0;
		pParent = NULL;
	};

	bool contains(int mouse_x, int mouse_y); // check if the mouse coords are inside the button
	// implement widget specific draw()/update()
	void draw();
	void update();

	void SetParent(Widget* _pParent); // set the button's parent
	void SetText(std::string _text); // set the button's text
	std::string GetText() const {return text;} // return the button's text
	void SetTextOffsetX(float tox) { text_offset_x = tox; }// set the text offset-x
	float GetTextOffsetX() const { return text_offset_x; }// return the text offset-x
	void SetTextOffsetY(float toy) { text_offset_y = toy; }// set the text offset-y
	float GetTextOffsetY() const { return text_offset_y; }// return the text offset-y
};

/*
	StaticText class provides a static-text control functionality.
	Adds new members for font definition: font, font_size, font_brush.
*/
class StaticText : public Button {

protected:
	std::string font;  // the string of the font
	float font_size = 0; // the size of the font
	graphics::Brush font_brush; // the brush of the font (different from widget brush)

public:
	StaticText(float x, float y, float w, float h); //constructor

	StaticText() { // default constructor
		pos_x = 0;
		pos_y = 0;
		width = 0;
		height = 0;
		text = "";
	};

	void SetFont(const std::string& _font, float _font_size, graphics::Brush _font_brush) {
		font = _font; font_size = _font_size; font_brush = _font_brush; }
	std::string GetFont() const { return font; } //return the font
	float GetFontSize() const { return font_size; } // get the font size
	graphics::Brush GetFontBrush() const { return font_brush; } // return the font's brush

	// implement widget specific draw()/update()
	void draw();
	void update();
};


/*
	ClearButton class implements a specific draw function that varies from the button's draw
*/
class ClearButton : public Button { //button to clear all the filters
public:
	ClearButton(float x, float y, float w, float h, Widget* _pParent); //constructor

	ClearButton() { // default constructor
		pos_x = 0;
		pos_y = 0;
		width = 0;
		height = 0;
		pParent = NULL;
	};
	void draw();	
};


/*
	Class RightArrow draws a right arrow which is used for shifting the movies in the moviebrowser
*/
class RightArrow : public Button { 

public:
	RightArrow(float x, float y, float w, float h);// constructor

	RightArrow() { // default constructor
		pos_x = 0;
		pos_y = 0;
		width = 0;
		height = 0;
	};

	void draw();
	void update();

};


/*
	Class LeftArrow draws a left arrow which is used for shifting the movies in the moviebrowser
*/
class LeftArrow : public Button { 

public:
	LeftArrow(float x, float y, float w, float h); //constructor

	LeftArrow() {// default constructor
		pos_x = 0;
		pos_y = 0;
		width = 0;
		height = 0;
	};

	void draw();
	void update();

};

/*
	Class EditBox creates an editbox where the user can write the year filter.
	The class has a CheckKeyboard function that fills/erase digits from the text member.
	The update function is implemented to check for user's mouse and keyboard input.
*/

class EditBox : public Button { 

public:
	EditBox(float x, float y, float w, float h); //constructor

	EditBox() { // default constructor
		pos_x = 0;
		pos_y = 0;
		width = 0;
		height = 0;
		text = "";
	};

	void draw();
	void update();
	void CheckKeyboard(); // checks user's input; translates characters to digits; sets/removes year filter
};

/*
	Class Dock provides a window for all the filters.
	All filters are implemented as dynamic control members and they are initialized when the dock is open for the first time.
*/


class Dock : public Widget { 

protected:
	ClearButton* pClearFilters = nullptr; //declare the clear button pointer

	//declare all the button pointers to create the genre filters
	Button* pActionFilter = nullptr; 
	Button* pDramaFilter = nullptr;
	Button* pSci_Fi_Filter = nullptr;
	Button* pAdventureFilter = nullptr;
	Button* pComedyFilter = nullptr;
	Button* pAnimationFilter = nullptr;
	Button* pFamilyFilter = nullptr;
	Button* pBiographyFilter = nullptr;
	Button* pCrimeFilter = nullptr;

	// declare editbox pointer to create the year filter
	EditBox* pYear = nullptr; 

	// declare the static text pointers to create the static text of the dock 
	StaticText* pGenreStatic = nullptr; 
	StaticText* pYearStatic = nullptr;
	StaticText* pFilterStatic = nullptr;
	

	bool _isOpen = false; // holds the status of the dock
	int n_base_button_id = 0; // variable to set the base id of the buttons, so that we can distinguish the type of the filters by their ids.

public:
	Dock(float x, float y, float w, float h);
	~Dock();

	Dock() {
		pos_x = 0;
		pos_y = 0;
		width = 0;
		height = 0;
	};

	bool contains(int mouse_x, int mouse_y); // mouse in pixels; check if mouse pointer is within the dock when it is open.
	bool contains2(int mouse_x, int mouse_y); // mouse in pixels; check if mouse pointer is within the dock when it is closed.


	void draw(); // draws the dock and all the children controls
	void update(); // updates the dock and all the children controls

	bool isOpen() const {return _isOpen;}  // returns if the dock is open or not
	void SetOpen(bool bOpen) { _isOpen = bOpen; } // sets the dock open/closed

	void EnableFilter(Button* pButton, bool bEnable); // bEnable is true when we enable a filter, false otherwise
};
