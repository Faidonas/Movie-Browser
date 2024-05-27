#include "gui.h"
#include "Movie.h"
#include "MovieBrowser.h"
#include <iostream>
#include "scancodes.h"

using namespace std;
using namespace graphics;

int Widget::m_next_uid = 1;
int Widget::m_focus_id = 0;


Widget::Widget(float x, float y, float w, float h, const graphics::Brush& br)
{
	pos_x = x;
	pos_y = y;
	height = h;
	width = w;
	wbr = br;
	m_highlighted = false;
	m_uid = m_next_uid++;
}

WidgetMovie::WidgetMovie(float x, float y, float w, float h, Movie* m)
{
	pos_x = x;
	pos_y = y;
	height = h;
	width = w;
	m_highlighted = false;
	movie = m;
	
}



void WidgetMovie::draw() {
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 100);//use glow for the outline color
	float c = 1.0f * m_highlighted * glow;
	

	wbr.outline_color[0] =  c ; // we want the outline color of the movie widget to glow
	wbr.outline_color[1] =  c ;
	wbr.outline_color[2] =  c ;

	graphics::drawRect(pos_x+width/2, pos_y+height/2, width, height, wbr);

	Widget::draw();
}



void WidgetMovie::update() {

	Widget::update();
}


bool Widget::requestFocus()
{
	if (!m_focus_id || m_focus_id != m_uid) {
		m_focus_id = m_uid;
		return true;
	}
	return false;
}

bool Widget::hasFocus()
{
	return m_focus_id == m_uid; // check if the focus id eqauls to the unique id of the widget
}

void Widget::releaseFocus()
{
	if (m_focus_id == m_uid) { // if the widget has the focus, set focus id to 0 to release it
		m_focus_id = 0;
	}
}

void Widget::SetPos(float x, float y) // set the position of the widget 
{
	pos_x = x;
	pos_y = y;
}

void Widget::SetSize(float w, float h) // set  the size of the widget
{
	height = h;
	width = w;
}

void Widget::GetPos(float& x, float& y) // return the position of the widget
{
	x = pos_x;
	y = pos_y;
}

void Widget::GetSize(float& w, float& h) // return the size of the widget
{
	h = height;
	w = width;
}


void Widget::SetBrush(const graphics::Brush& br) // set the brush of the widget
{
	wbr = br;
}

bool Widget::contains(int mouse_x, int mouse_y) 
{
	// convert mouse coords to canvas units
	float mx = graphics::windowToCanvasX((float)mouse_x);
	float my = graphics::windowToCanvasY((float)mouse_y);
	// return true if the mouse coords are inside the widget
	return (mx >= pos_x && mx <= pos_x + width &&
		my >= pos_y && my <= pos_y + height);
}

bool Button::contains(int mouse_x, int mouse_y) // mouse in pixels
{
	// convert mouse coords to canvas units
	float mx = graphics::windowToCanvasX((float)mouse_x);
	float my = graphics::windowToCanvasY((float)mouse_y);
	if(pParent)
		return (mx >= pParent->GetPosX() + pos_x && mx <= pParent->GetPosX()+pos_x + width && // compare the mouse coords to the button's coords
		my >= pParent->GetPosY() + pos_y && my <= pParent->GetPosY() + pos_y + height);
	else
		return (mx >= pos_x && mx <= pos_x + width &&
			my >= pos_y && my <= pos_y + height);
}

void Widget::scroll_right()
{
	pos_x += WIDGET_SCROLL;
}

void Widget::scroll_left()
{
	pos_x -= WIDGET_SCROLL;
}

RightArrow::RightArrow(float x, float y, float w, float h)
{
	pos_x = x;
	pos_y = y;
	height = h;
	width = w;
}

void RightArrow::draw()
{
	graphics::MouseState ms;
	graphics::getMouseState(ms);
	float mx = graphics::windowToCanvasX((float)ms.cur_pos_x);
	float my = graphics::windowToCanvasY((float)ms.cur_pos_y);
	float center_x = pos_x + width / 2.0f;
	float center_y = pos_y + height / 2.0f;
	float d = std::min(distance(center_x, center_y, mx, my), (float)CANVAS_HEIGHT / 2) / ((float)CANVAS_HEIGHT / 2);
	float highlight = 1.0f - d;

	float c = 1.0f * highlight;

	wbr.fill_opacity = std::max(.4f, c);
	graphics::setOrientation(-180.f);
	wbr.outline_opacity = 0.f;
	graphics::drawRect(pos_x + width / 2.f, pos_y + height / 2.f, width, height, wbr);
	graphics::setOrientation(0.f);

	Widget::draw();
}

void RightArrow::update()
{	

	Widget::update();
}

LeftArrow::LeftArrow(float x, float y, float w, float h)
{
	pos_x = x;
	pos_y = y;
	height = h;
	width = w;
}


void LeftArrow::draw()
{
	graphics::MouseState ms;
	graphics::getMouseState(ms);
	float mx = graphics::windowToCanvasX((float)ms.cur_pos_x);
	float my = graphics::windowToCanvasY((float)ms.cur_pos_y);
	float center_x = pos_x + width / 2.0f;
	float center_y = pos_y + height / 2.0f;
	float d = std::min(distance(center_x, center_y, mx, my), (float)CANVAS_HEIGHT / 2) / ((float)CANVAS_HEIGHT / 2);
	float highlight = 1.0f - d;
	float c = 1.0f * highlight;

	wbr.fill_opacity = std::max(.4f, c);
	wbr.outline_opacity = 0.f;
	graphics::drawRect(pos_x+width/2.f, pos_y+height/2.f, width, height, wbr);

	Widget::draw();
}

void LeftArrow::update()
{
	Widget::update();
}

Dock::Dock(float x, float y, float w, float h)
{
	pos_x = x;
	pos_y = y;
	height = h;
	width = w;
}

Dock::~Dock() // destructor for the dock:delete all the pointers used in the dock
{
	if (pClearFilters)
		delete pClearFilters;
	if (pActionFilter)
		delete pActionFilter;
	if (pDramaFilter)
		delete pDramaFilter;
	if (pSci_Fi_Filter)
		delete pSci_Fi_Filter;
	if (pAdventureFilter)
		delete pAdventureFilter;
	if (pComedyFilter)
		delete pComedyFilter;
	if (pAnimationFilter)
		delete pAnimationFilter;
	if (pFamilyFilter)
		delete pFamilyFilter;
	if (pBiographyFilter)
		delete pBiographyFilter;
	if (pCrimeFilter)
		delete pCrimeFilter;
	if (pYear)
		delete pYear;
	if (pGenreStatic)
		delete pGenreStatic;
	if (pYearStatic)
		delete pYearStatic;
	if (pFilterStatic)
		delete pFilterStatic;

}

void Dock::draw()
{
	static bool bStartAnim = true;
	static float dt = 0;
	wbr.outline_opacity = 0;
	wbr.texture = std::string(ASSET_PATH) + "dock.png";

	if (!isOpen())
	{
		float dy = (DOCK_HEIGHT-44) ;// the arrow size in dock
		float y = pos_y - dy / 2.0f;
		graphics::drawRect(pos_x + width / 2.f, y, width, height, wbr);
		bStartAnim = true;
	}
	else
	{
		if (bStartAnim)
			pos_y = -DOCK_HEIGHT;
		dt += getDeltaTime();
		if (dt >= 10 && pos_y < 0)
		{
			pos_y += 10;
			if (pos_y > 0)
				pos_y = 0;
			dt = 0;
			bStartAnim = false;
		}

		graphics::drawRect(pos_x + width / 2.f, pos_y + height / 2.f, width, height, wbr);
		//if the dock is open draw all the filters
		if (pClearFilters)
			pClearFilters->draw();
		if (pActionFilter)
			pActionFilter->draw();
		if (pDramaFilter)
			pDramaFilter->draw();
		if (pSci_Fi_Filter)
			pSci_Fi_Filter->draw();
		if (pComedyFilter)
			pComedyFilter->draw();
		if (pFamilyFilter)
			pFamilyFilter->draw();
		if (pBiographyFilter)
			pBiographyFilter->draw();
		if (pAdventureFilter)
			pAdventureFilter->draw();
		if (pAnimationFilter)
			pAnimationFilter->draw();
		if (pCrimeFilter)
			pCrimeFilter->draw();
		if (pYear)
			pYear->draw();
		if (pGenreStatic)
			pGenreStatic->draw();
		if (pYearStatic)
			pYearStatic->draw();
		if (pFilterStatic)
			pFilterStatic->draw();
	}

	Widget::draw();
}

void Dock::update()
{
	// create static text boxes
	if (!pGenreStatic)
	{
		pGenreStatic = new StaticText;
		pGenreStatic->SetPos(DOCK_WIDTH / 9.f , 60.f);
		pGenreStatic->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
		pGenreStatic->SetParent(this);
		pGenreStatic->SetText("Genres:");
		graphics::Brush gbrush;
		gbrush.outline_opacity = 0.f;
		gbrush.fill_opacity = 0.f;
		pGenreStatic->SetBrush(gbrush);
		gbrush.outline_opacity = 1.f;
		gbrush.fill_opacity = 1.f;
		gbrush.fill_color[0] = 1.f;
		gbrush.fill_color[1] = 1.f;
		gbrush.fill_color[2] = 1.f;
		pGenreStatic->SetFont(std::string(ASSET_PATH) + "OpenSans-Regular.ttf", 30, gbrush);
		pGenreStatic->SetTextOffsetY(-5);
	}

	if (!pYearStatic)
	{
		pYearStatic = new StaticText;
		pYearStatic->SetPos(DOCK_WIDTH / 9.f , 230.f);
		pYearStatic->SetSize(BUTTON_WIDTH, BUTTON_HEIGHT);
		pYearStatic->SetParent(this);
		pYearStatic->SetText("Year:");
		graphics::Brush gbrush;
		gbrush.outline_opacity = 0.f;
		gbrush.fill_opacity = 0.f;
		pYearStatic->SetBrush(gbrush);
		gbrush.outline_opacity = 1.f;
		gbrush.fill_opacity = 1.f;
		gbrush.fill_color[0] = 1.f;
		gbrush.fill_color[1] = 1.f;
		gbrush.fill_color[2] = 1.f;
		pYearStatic->SetFont(std::string(ASSET_PATH) + "OpenSans-Regular.ttf", 30, gbrush);
		pYearStatic->SetTextOffsetY(-5);
	}
	if (!pFilterStatic)
	{
		pFilterStatic = new StaticText;
		pFilterStatic->SetPos(3, 10);
		pFilterStatic->SetSize(DOCK_WIDTH-6, 30);
		pFilterStatic->SetParent(this);
		pFilterStatic->SetText("Filter movies");
		graphics::Brush gbrush;
		gbrush.outline_opacity = 0.f;
		gbrush.fill_opacity = 1.f;
		gbrush.fill_color[0] = 0.4f;
		gbrush.fill_color[1] = 0.4f;
		gbrush.fill_color[2] = 0.4f;
		pFilterStatic->SetBrush(gbrush);
		gbrush.outline_opacity = 1.f;
		gbrush.fill_opacity = 1.f;
		gbrush.fill_color[0] = 1.f;
		gbrush.fill_color[1] = 1.f;
		gbrush.fill_color[2] = 1.f;
		pFilterStatic->SetFont(std::string(ASSET_PATH) + "OpenSans-Regular.ttf", 20, gbrush);
		pFilterStatic->SetTextOffsetX(300);
		pFilterStatic->SetTextOffsetY(-5);

	}


	//create clear filters button
	if (!pClearFilters)
	{
		pClearFilters = new ClearButton;
		n_base_button_id = pClearFilters->GetID(); // one time only, retrieve the first ID of the first Button = ClearButton
		pClearFilters->SetPos(DOCK_WIDTH-200, 95);
		pClearFilters->SetSize(BUTTON_WIDTH+30, BUTTON_HEIGHT+20);
		pClearFilters->SetParent(this);
		pClearFilters->SetText("Clear Filters");
		pClearFilters->SetTextOffsetX(2);
		pClearFilters->SetTextOffsetY(-5);
	}
	//create filters button
	if (!pActionFilter)
	{
		pActionFilter = new Button;
		pActionFilter->SetPos(DOCK_WIDTH / 9.f, 95.f);
		pActionFilter->SetSize(BUTTON_WIDTH-40, BUTTON_HEIGHT);
		pActionFilter->SetParent(this);
		pActionFilter->SetText("Action");
		pActionFilter->SetTextOffsetX(5);
		pActionFilter->SetTextOffsetY(-5);
	}
	if (!pDramaFilter)
	{
		pDramaFilter = new Button;
		pDramaFilter->SetPos(DOCK_WIDTH / 9.f + BUTTON_WIDTH -34, 95.f);
		pDramaFilter->SetSize(BUTTON_WIDTH-38, BUTTON_HEIGHT);
		pDramaFilter->SetParent(this);
		pDramaFilter->SetText("Drama");
		pDramaFilter->SetTextOffsetX(7);
		pDramaFilter->SetTextOffsetY(-5);
	}
	if (!pSci_Fi_Filter)
	{
		pSci_Fi_Filter = new Button;
		pSci_Fi_Filter->SetPos(DOCK_WIDTH / 9.f+2*BUTTON_WIDTH -65, 95.f);
		pSci_Fi_Filter->SetSize(BUTTON_WIDTH-52, BUTTON_HEIGHT);
		pSci_Fi_Filter->SetParent(this);
		pSci_Fi_Filter->SetText("Sci-Fi");
		pSci_Fi_Filter->SetTextOffsetX(5);
		pSci_Fi_Filter->SetTextOffsetY(-5);
	}
	if (!pComedyFilter)
	{
		pComedyFilter = new Button;
		pComedyFilter->SetPos(DOCK_WIDTH / 9.f+3 * BUTTON_WIDTH -110 , 95.f);
		pComedyFilter->SetSize(BUTTON_WIDTH-25, BUTTON_HEIGHT);
		pComedyFilter->SetParent(this);
		pComedyFilter->SetText("Comedy");
		pComedyFilter->SetTextOffsetX(7);
		pComedyFilter->SetTextOffsetY(-5);
	}
	if (!pCrimeFilter)
	{
		pCrimeFilter = new Button;
		pCrimeFilter->SetPos(DOCK_WIDTH / 9.f+3 * BUTTON_WIDTH -28 , 95.f);
		pCrimeFilter->SetSize(BUTTON_WIDTH - 44, BUTTON_HEIGHT);
		pCrimeFilter->SetParent(this);
		pCrimeFilter->SetText("Crime");
		pCrimeFilter->SetTextOffsetX(6);
		pCrimeFilter->SetTextOffsetY(-5);
	}
	if (!pFamilyFilter)
	{
		pFamilyFilter = new Button;
		pFamilyFilter->SetPos(DOCK_WIDTH / 9.f, 125.f);
		pFamilyFilter->SetSize(BUTTON_WIDTH -43, BUTTON_HEIGHT);
		pFamilyFilter->SetParent(this);
		pFamilyFilter->SetText("Family");
		pFamilyFilter->SetTextOffsetX(6);
		pFamilyFilter->SetTextOffsetY(-5);
	}
	if (!pBiographyFilter)
	{
		pBiographyFilter = new Button;
		pBiographyFilter->SetPos(DOCK_WIDTH / 9.f+ BUTTON_WIDTH -37, 125.f);
		pBiographyFilter->SetSize(BUTTON_WIDTH-10, BUTTON_HEIGHT);
		pBiographyFilter->SetParent(this);
		pBiographyFilter->SetText("Biography");
		pBiographyFilter->SetTextOffsetX(8);
		pBiographyFilter->SetTextOffsetY(-5);
	}
	if (!pAdventureFilter)
	{
		pAdventureFilter = new Button;
		pAdventureFilter->SetPos(DOCK_WIDTH / 9.f+2 * BUTTON_WIDTH -41, 125.f);
		pAdventureFilter->SetSize(BUTTON_WIDTH-4, BUTTON_HEIGHT);
		pAdventureFilter->SetParent(this);
		pAdventureFilter->SetText("Adventure");
		pAdventureFilter->SetTextOffsetX(7);
		pAdventureFilter->SetTextOffsetY(-5);
	}
	if (!pAnimationFilter)
	{
		pAnimationFilter = new Button;
		pAnimationFilter->SetPos(DOCK_WIDTH / 9.f+3 * BUTTON_WIDTH -38 , 125.f);
		pAnimationFilter->SetSize(BUTTON_WIDTH-10, BUTTON_HEIGHT);
		pAnimationFilter->SetParent(this);
		pAnimationFilter->SetText("Animation");
		pAnimationFilter->SetTextOffsetX(6);
		pAnimationFilter->SetTextOffsetY(-5);
	}
	// create editbox button
	if (!pYear)
	{
		pYear = new EditBox;
		pYear->SetPos(DOCK_WIDTH / 9.f, 265.f);
		pYear->SetSize(DOCK_WIDTH / 3.f, BUTTON_HEIGHT);
		pYear->SetParent(this);
	}


	graphics::MouseState ms;
	graphics::getMouseState(ms);
	if (!isOpen())
	{
		// check cursor against "closed" dock
		if (contains2(ms.cur_pos_x, ms.cur_pos_y))
		{
			requestFocus();
			SetOpen(true);
		}
	}
	else
	{
		// check cursor against "open" dock
		if (pos_y>=0 && !contains(ms.cur_pos_x, ms.cur_pos_y))
		{
			releaseFocus();
			SetOpen(false);
		}
	}

	// is dock is open then update children controls
	if (isOpen())
	{
		pClearFilters->update();
		pActionFilter->update();
		pDramaFilter->update();
		pSci_Fi_Filter->update();
		pComedyFilter->update();
		pFamilyFilter->update();
		pBiographyFilter->update();
		pAdventureFilter->update();
		pAnimationFilter->update();
		pCrimeFilter->update();
		pYear->update();
		pGenreStatic->update();
		pYearStatic->update();
		pFilterStatic->update();
	}

	Widget::update();
}

void Dock::EnableFilter(Button* pButton, bool bEnable) // bEnable is true when we enable a filter, false otherwise)
{
	MovieBrowser* pMovieBrowser = (MovieBrowser*)graphics::getUserData();
	if (pButton->GetID() == n_base_button_id) // this is clear button of Genres
	{
		pMovieBrowser->SetFilter(_DEFAULT);
		pClearFilters->setHighlighted(false);
		pActionFilter->setHighlighted(false);
		pDramaFilter->setHighlighted(false);
		pSci_Fi_Filter->setHighlighted(false);
		pComedyFilter->setHighlighted(false);
		pCrimeFilter->setHighlighted(false);
		pFamilyFilter->setHighlighted(false);
		pBiographyFilter->setHighlighted(false);
		pAdventureFilter->setHighlighted(false);
		pAnimationFilter->setHighlighted(false);
		pYear->SetText("");
		pYear->setHighlighted(false);		
	}
	else // enable/disable genres
		if (pButton->GetID() == n_base_button_id + 1) // this is action button
		{
			if(bEnable)
				pMovieBrowser->AddFilter(_GENRE, ACTION);
			else
				pMovieBrowser->ClearFilter(_GENRE, ACTION);
		}
		else
			if (pButton->GetID() == n_base_button_id + 2) // this is drama button
			{
				if (bEnable)
					pMovieBrowser->AddFilter(_GENRE, DRAMA);
				else
					pMovieBrowser->ClearFilter(_GENRE, DRAMA);
			}
			else
				if (pButton->GetID() == n_base_button_id + 3) // this is sci-fi button
				{
					if (bEnable)
						pMovieBrowser->AddFilter(_GENRE, SCI_FI);
					else
						pMovieBrowser->ClearFilter(_GENRE, SCI_FI);
				}
				else
					if (pButton->GetID() == n_base_button_id + 4) // this is comedy button
					{
						if (bEnable)
							pMovieBrowser->AddFilter(_GENRE, COMEDY);
						else
							pMovieBrowser->ClearFilter(_GENRE, COMEDY);
					}
					else
							if (pButton->GetID() == n_base_button_id + 5) // this is crime button
							{
								if (bEnable)
									pMovieBrowser->AddFilter(_GENRE, CRIME);
								else
									pMovieBrowser->ClearFilter(_GENRE, CRIME);
							}
							else
						if (pButton->GetID() == n_base_button_id + 6) // this is family button
						{
							if (bEnable)
								pMovieBrowser->AddFilter(_GENRE, FAMILY);
							else
								pMovieBrowser->ClearFilter(_GENRE, FAMILY);
						}
						else
							if (pButton->GetID() == n_base_button_id + 7) // this is biography button
							{
								if (bEnable)
									pMovieBrowser->AddFilter(_GENRE, BIOGRAPHY);
								else
									pMovieBrowser->ClearFilter(_GENRE, BIOGRAPHY);
							}
							else
								if (pButton->GetID() == n_base_button_id + 8) // this is adventure button
								{
									if (bEnable)
										pMovieBrowser->AddFilter(_GENRE, ADVENTURE);
									else
										pMovieBrowser->ClearFilter(_GENRE, ADVENTURE);
								}
								else
									if (pButton->GetID() == n_base_button_id + 9) // this is animation button
									{
										if (bEnable)
											pMovieBrowser->AddFilter(_GENRE, ANIMATION);
										else
											pMovieBrowser->ClearFilter(_GENRE, ANIMATION);
									}
									else
										if (pButton->GetID() == n_base_button_id + 10) // this is the year edit box
										{
											int nyear = (int)strtol((char*)pButton->GetText().c_str(), nullptr, 10);
											if (bEnable)
												pMovieBrowser->AddFilter(_YEAR, "", nyear);
											else
												pMovieBrowser->ClearFilter(_YEAR, "", nyear);
										}

}

bool Dock::contains2(int mouse_x, int mouse_y) // mouse in pixels; no focus; closed window
{
	// convert mouse coords to canvas units
	float mx = graphics::windowToCanvasX((float)mouse_x);
	float my = graphics::windowToCanvasY((float)mouse_y);
	float dy = graphics::windowToCanvasY(24); // the arrow size in dock
	return (mx >= pos_x && mx <= pos_x + width &&
		my >= 0 && my <= dy);
}

bool Dock::contains(int mouse_x, int mouse_y) // mouse in pixels; with focus; with open window
{
	return Widget::contains(mouse_x, mouse_y);
}


Button::Button(float x, float y, float w, float h, Widget* _pParent)
{
	pos_x = x;
	pos_y = y;
	height = h;
	width = w;
	pParent = _pParent;
}

void Widget::setHighlighted(bool bh)
{ 
	m_highlighted = bh; 
}

void Button::update() 
{
	graphics::MouseState ms;
	graphics::getMouseState(ms);
	if (contains(ms.cur_pos_x, ms.cur_pos_y)) {
		requestFocus();// if the cursor is inside the button request focus
		
		if (ms.button_left_pressed) {// if the button is pressed:play sound and highlight
			graphics::playSound(string(ASSET_PATH) + "button.wav", 1.0f, false);
			setHighlighted(!isHightlighted());
			if (!isHightlighted())
			{
				((Dock*)pParent)->EnableFilter(this, false); // if the button isnt highlighted, clear the filter and release focus			
				releaseFocus();
			}
			else
			{
				((Dock*)pParent)->EnableFilter(this, true); // if the button is highlighted, add filter
			}
		}
	}
	else
	{
		releaseFocus();
	}


	Widget::update();
}


void Button::draw() 
{	//draw the filter button
	wbr.outline_opacity = 1.f;
	wbr.outline_color[0] = .8f;
	wbr.outline_color[1] = .8f;
	wbr.outline_color[2] = .8f;
	if (hasFocus() || isHightlighted())
	{
		wbr.fill_color[0] = .8f;
		wbr.fill_color[1] = .8f;
		wbr.fill_color[2] = .8f;
	}
	else{
		wbr.fill_color[0] = .5f;
		wbr.fill_color[1] = .5f;
		wbr.fill_color[2] = .5f;
	}
	float x = 0, y = 0;
	if (pParent)
		pParent->GetPos(x, y); // get position of parent upper/left corner

	// add local offset of current button
	x += pos_x;
	y += pos_y;
	graphics::drawRect(x + width / 2, y + height / 2, width, height, wbr);

	//draw the filters title
	graphics::Brush brush;
	graphics::setFont(std::string(ASSET_PATH) + "OpenSans-Light.ttf");
	brush.fill_color[0] = 0.f;
	brush.fill_color[1] = 0.f;
	brush.fill_color[2] = 0.f;
	graphics::drawText(x+ text_offset_x, y + height+ text_offset_y, 19, text, brush);

	Widget::draw();
}
void Button::SetParent(Widget* _pParent)
{
	pParent = _pParent;
}

void Button::SetText(std::string _text)
{
	text = _text;
}

EditBox::EditBox(float x, float y, float w, float h)
{
	x = y = w = h = 0.f;
	text = "";
}

void EditBox::draw()
{
	if (hasFocus() || isHightlighted())
	{
		wbr.outline_color[0] = 1.f;
		wbr.outline_color[1] = 1.f;
		wbr.outline_color[2] = 1.f;
	}
	else {
		wbr.outline_color[0] = .5f;
		wbr.outline_color[1] = .5f;
		wbr.outline_color[2] = .5f;
	}
	wbr.fill_opacity = 0.f; // make transparent
	float x = 0, y = 0;
	if (pParent)
		pParent->GetPos(x, y); // get position of parent upper/left corner

	// add local offset of current button
	x += pos_x;
	y += pos_y;
	graphics::drawRect(x + width / 2, y + height / 2, width, height, wbr);

	//draw the editbox text
	graphics::Brush brush;
	graphics::setFont(std::string(ASSET_PATH) + "OpenSans-Regular.ttf");
	brush.fill_color[0] = 1.f;
	brush.fill_color[1] = 1.f;
	brush.fill_color[2] = 1.f;

	graphics::drawText(x + 2, y + height - 5, 19, text, brush);
}

void EditBox::update()
{
	graphics::MouseState ms;
	graphics::getMouseState(ms);
	if (contains(ms.cur_pos_x, ms.cur_pos_y)) {
		requestFocus();

		if (ms.button_left_pressed) {
			setHighlighted(!isHightlighted());
			if (!isHightlighted())
			{
				releaseFocus();
			}
		}
	}
	else
	{
		releaseFocus();
	}

	static float dt = 0;
	dt += getDeltaTime();
	if (isHightlighted() && dt>=70)
	{
		CheckKeyboard(); // goes to checkKeyboard after a certain time to avoid multiple keywords
		dt = 0;
	}

	Widget::update();
}

void EditBox::CheckKeyboard()
{
	if(text.length()==4)//if the year has 4 or more digits,add filter or check for backspace
	{
		// check if backspace is pressed
		if (graphics::getKeyState(SCANCODE_BACKSPACE))
		{
			((Dock*)pParent)->EnableFilter(this, false); // removefilter and erase the last digit
			text.erase(text.end() - 1, text.end());
		}
		else // otherwise add new filter
		{
			((Dock*)pParent)->EnableFilter(this, true); // add filter
		}
	}
	else //add the pressed digit to the text string
	{
		if (graphics::getKeyState(SCANCODE_1))
			text += "1";
		else
			if (graphics::getKeyState(SCANCODE_2))
				text += "2";
			else
				if (graphics::getKeyState(SCANCODE_3))
					text += "3";
				else
					if (graphics::getKeyState(SCANCODE_4))
						text += "4";
					else
						if (graphics::getKeyState(SCANCODE_5))
							text += "5";
						else
							if (graphics::getKeyState(SCANCODE_6))
								text += "6";
							else
								if (graphics::getKeyState(SCANCODE_7))
									text += "7";
								else
									if (graphics::getKeyState(SCANCODE_8))
										text += "8";
									else
										if (graphics::getKeyState(SCANCODE_9))
											text += "9";
										else
											if (graphics::getKeyState(SCANCODE_0))
												text += "0";
											else
												if (graphics::getKeyState(SCANCODE_BACKSPACE))
													if (text != "")
														text.erase(text.end() - 1, text.end());
	}

}

StaticText::StaticText(float x, float y, float w, float h)
{
	x = y = w = h = 0;
	text = "";
}

void StaticText::draw()
{
	//draw the text
	float x = 0, y = 0;
	if (pParent)
		pParent->GetPos(x, y); // get position of parent upper/left corner

	// add local offset of current button
	x += pos_x;
	y += pos_y;
	graphics::drawRect(x + width / 2, y + height / 2, width, height, wbr);
	graphics::drawText(x + text_offset_x, y + height + text_offset_y, font_size, text, font_brush);

	Widget::draw();
}

void StaticText::update()
{
	// do nothing
	Widget::update();
}

ClearButton::ClearButton(float x, float y, float w, float h, Widget* _pParent)
{
	pos_x = x;
	pos_y = y;
	height = h;
	width = w;
	pParent = _pParent;
}

void ClearButton::draw()
{
	wbr.outline_opacity = 1.f;
	wbr.outline_color[0] = .8f;
	wbr.outline_color[1] = .8f;
	wbr.outline_color[2] = .8f;
	if (hasFocus() || isHightlighted())
	{
		wbr.fill_color[0] = .8f;
		wbr.fill_color[1] = .8f;
		wbr.fill_color[2] = .8f;
	}
	else {
		wbr.fill_color[0] = .5f;
		wbr.fill_color[1] = .5f;
		wbr.fill_color[2] = .5f;
	}
	float x=0, y=0;
	if(pParent)
		pParent->GetPos(x, y); // get position of parent upper/left corner

	// add local offset of current button
	x += pos_x;
	y += pos_y;
	graphics::drawRect(x + width / 2, y + height / 2, width, height, wbr);

	//draw the  text
	graphics::Brush brush;
	graphics::setFont(std::string(ASSET_PATH) + "OpenSans-Semibold.ttf");
	brush.fill_color[0] = 0.f;
	brush.fill_color[1] = 0.f;
	brush.fill_color[2] = 0.f;

	graphics::drawText(x + 7, y + height - 14 , 23, text, brush);

	Widget::draw();
}

