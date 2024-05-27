#include "MovieBrowser.h"
#include "graphics.h"
#include "Config.h"
#include <fstream>
#include "gui.h"

using namespace std;


void MovieBrowser::update()
{
	// get mouse state
	graphics::MouseState ms;
	graphics::getMouseState(ms);

	// first time here?
	if (!movies_initialized) {

		// read movies from file
		std::string line;
		std::string filename = "ReadMe.txt";
		std::ifstream file;

		file.open(filename);//open the file

		if (file.is_open())
		{
			Movie* movie = nullptr;
			int count_line = 0;
			while (getline(file, line))
			{
				if (line == "-") {
					movie = new Movie;
					nMovies++; // one more movie in the list
					movie_list.push_back(movie); // add to list
					count_line = 0;
					continue;
				}
				switch (count_line)
				{
				case 0: movie->SetTitle(line); break;
				case 1: movie->SetGenre(line); break;
				case 2: movie->SetDirector(line); break;
				case 3: movie->SetStars(line); break;
				case 4: movie->SetYear(line); break;
				case 5: movie->SetSummary(line); break;
				case 6: movie->SetPicName(line); break;
				}
				count_line++;
			}
			file.close();
		}
		else {
			std::cout << "Could not open the file" << std::endl;
		}

		// initialize movie widget position and size
		int i = 0;
		for (auto iMovie = movie_list.begin(); iMovie != movie_list.end(); ++iMovie)
		{
			const float space_x = WIDGET_SPACE;
			float corner_x = i * (WIDGET_WIDTH + space_x) + space_x;
			float corner_y = 60.f;
			(*iMovie)->init(); // will create MovieWidget
			(*iMovie)->wm->SetPos(corner_x, corner_y);
			(*iMovie)->wm->SetSize(WIDGET_WIDTH, WIDGET_HEIGHT);
			i++;
		}

		// initialize right arrow
		ra = new RightArrow();
		ra->SetPos(CANVAS_WIDTH - 35, 60 + WIDGET_HEIGHT / 2);
		ra->SetSize(30.f, 30.f);
		ra->SetParent(nullptr);
		graphics::Brush ar_br;
		ar_br.outline_opacity = 1.f;
		ar_br.texture = (std::string)ASSET_PATH + "arrow.png";
		ra->SetBrush(ar_br);

		// initialize left arrow
		la = new LeftArrow();
		la->SetPos(5.f, 60 + WIDGET_HEIGHT / 2);
		la->SetSize(30.f, 30.f);
		la->SetParent(nullptr);
		la->SetBrush(ar_br);

		//create dock
		dock = new Dock();
		dock->SetPos(CANVAS_WIDTH / 2.f - DOCK_WIDTH / 2.f, 0);
		dock->SetSize(DOCK_WIDTH, DOCK_HEIGHT);

		SetFilter(_DEFAULT);

		// keep the pointer to MovieBrowser for easy filtering
		graphics::setUserData(this);

		// done with initialization
		movies_initialized = true;
	} // if movies_initialized

	// if dock is !open then we update the widgets of the main window
	if (!dock->isOpen())
	{
		// check mouse state and hightlight widgets
		for (auto iMovie = filtered_movie_list.begin(); iMovie != filtered_movie_list.end(); ++iMovie)
		{
			(*iMovie)->update();
		}
		// highlight arrows
		ra->update();
		la->update();

		// check if left button is clicked, scroll movies
		if (ms.button_left_pressed) {
			if (ra->contains(ms.cur_pos_x, ms.cur_pos_y)) {
				scroll_left();
			}
			else
				if (la->contains(ms.cur_pos_x, ms.cur_pos_y)) {
					scroll_right();
				}
		}
	}
	
	// update dock and is open dock is visible; 
	
		// update dock and its controls
		dock->update();
	
}

void MovieBrowser::DestroyFilterList()
{
	for (auto iFilter = filter_list.begin(); iFilter != filter_list.end(); iFilter++)
	{
		delete(*iFilter);
	}
	filter_list.clear();
}

void MovieBrowser::SetFilter(_FILTER_TYPE filter, const std::string _genre_param, int _year_param)
{
	int i = 0;
	nMovies = 0;
	nScroll = 0;
	switch (filter)
	{

		// this is the GENRE filter
	case _GENRE: {
		filtered_movie_list.clear();
		DestroyFilterList();
		filter_list.push_back(new FILTER(filter, _genre_param, -1));
		for (auto iMovie = movie_list.begin(); iMovie != movie_list.end(); ++iMovie) {
			if ((*iMovie)->isGenre(_genre_param))
			{
				const float space_x = WIDGET_SPACE;
				float corner_x = i * (WIDGET_WIDTH + space_x) + space_x;
				float corner_y = 60.f;
				(*iMovie)->wm->SetPos(corner_x, corner_y);
				(*iMovie)->wm->SetSize(WIDGET_WIDTH, WIDGET_HEIGHT);
				i++;
				nMovies++;
				filtered_movie_list.push_back(*iMovie);
			}
		}
	}  break;

		// this is the YEAR filter
	case _YEAR: {
		filtered_movie_list.clear();
		DestroyFilterList();
		filter_list.push_back(new FILTER(filter, "", _year_param));
		for (auto iMovie = movie_list.begin(); iMovie != movie_list.end(); ++iMovie) {
			if ((*iMovie)->isYear(_year_param))
			{
				filtered_movie_list.push_back(*iMovie);
				const float space_x = WIDGET_SPACE;
				float corner_x = i * (WIDGET_WIDTH + space_x) + space_x;
				float corner_y = 60.f;
				(*iMovie)->wm->SetPos(corner_x, corner_y);
				(*iMovie)->wm->SetSize(WIDGET_WIDTH, WIDGET_HEIGHT);
				i++;
				nMovies++;
			}
		}


	}break;

	default:
	{
		filtered_movie_list.clear();
		DestroyFilterList();
		for (auto iMovie = movie_list.begin(); iMovie != movie_list.end(); ++iMovie)
		{
			filtered_movie_list.push_back(*iMovie);
			const float space_x = WIDGET_SPACE;
			float corner_x = i * (WIDGET_WIDTH + space_x) + space_x;
			float corner_y = 60.f;
			(*iMovie)->wm->SetPos(corner_x, corner_y);
			(*iMovie)->wm->SetSize(WIDGET_WIDTH, WIDGET_HEIGHT);
			i++;
			nMovies++;
		}
	}

	}
}

bool MovieBrowser::FilterExist(_FILTER_TYPE filter, const std::string _genre_param, int _year_param)
{
	for (auto iFilter = filter_list.begin(); iFilter != filter_list.end(); iFilter++)
	{
		if ((*iFilter)->filter_type == filter && (*iFilter)->genre_param == _genre_param && (*iFilter)->year_param == _year_param)
			return true;
	}
	return false;
}

void MovieBrowser::AddFilter(_FILTER_TYPE filter, const std::string _genre_param, int _year_param)
{
	int i = 0;
	nMovies = 0; // restart movies counter
	nScroll = 0; // reset scrolling
	std::list<Movie*> temp_movie_list; // this is a temp list for storing the filtered movies

	// check type of filter and add new filter
	switch (filter)
	{
		// this is the GENRE filter; param is string
	case _GENRE: {
		filter_list.push_back(new FILTER(filter, _genre_param, -1));
		for (auto iMovie = filtered_movie_list.begin(); iMovie != filtered_movie_list.end(); ++iMovie) {
			if ((*iMovie)->isGenre(_genre_param))
			{
				temp_movie_list.push_back(*iMovie);
				const int space_x = WIDGET_SPACE;
				float corner_x = i * (WIDGET_WIDTH + space_x) + space_x;
				float corner_y = 60.f;
				(*iMovie)->wm->SetPos(corner_x, corner_y);
				(*iMovie)->wm->SetSize(WIDGET_WIDTH, WIDGET_HEIGHT);
				i++;
				nMovies++;
			}
		}
	}  break;

		// this is the YEAR filter; param is int
	case _YEAR: {
		// make sure we are not adding the same year more than one time
		if (FilterExist(filter, _genre_param, _year_param)) // if the same year already exists then do nothing
		{
			temp_movie_list = filtered_movie_list; // copy and out
			break;
		}
		filter_list.push_back(new FILTER(filter, "", _year_param));
		for (auto iMovie = filtered_movie_list.begin(); iMovie != filtered_movie_list.end(); ++iMovie) {
			if ((*iMovie)->isYear(_year_param))
			{
				temp_movie_list.push_back(*iMovie);
				const int space_x = WIDGET_SPACE;
				float corner_x = i * (WIDGET_WIDTH + space_x) + space_x;
				float corner_y = 60.f;
				(*iMovie)->wm->SetPos(corner_x, corner_y);
				(*iMovie)->wm->SetSize(WIDGET_WIDTH, WIDGET_HEIGHT);
				i++;
				nMovies++;
			}
		}
	}break;

	default:
	{
		for (auto iMovie = filtered_movie_list.begin(); iMovie != filtered_movie_list.end(); ++iMovie)
		{
			temp_movie_list.push_back(*iMovie);
			const int space_x = WIDGET_SPACE;
			float corner_x = i * (WIDGET_WIDTH + space_x) + space_x;
			float corner_y = 60.f;
			(*iMovie)->wm->SetPos(corner_x, corner_y);
			(*iMovie)->wm->SetSize(WIDGET_WIDTH, WIDGET_HEIGHT);
			i++;
			nMovies++;
		}
	}

	}
	filtered_movie_list = temp_movie_list;
}

void MovieBrowser::ClearFilter(_FILTER_TYPE filter, string _genre_param, int _year_param)
{
	std::list<FILTER*> temp_filter_list;

#ifdef _DEBUG
	cout << "Number of filters before clear = " << filter_list.size() << endl;
#endif

	// copy all filters from filter_list to temp_filter_list except the one given by the user
	for (auto iFilter = filter_list.begin(); iFilter != filter_list.end(); iFilter++)
	{
		if (!( (*iFilter)->filter_type == filter && (*iFilter)->genre_param == _genre_param && (*iFilter)->year_param == _year_param) )
			temp_filter_list.push_back(new FILTER( (*iFilter)->filter_type, (*iFilter)->genre_param, (*iFilter)->year_param) );
	}

	// disable all filters
	SetFilter(_DEFAULT);

	// now add one by one the filters back to the filter_list
	for (auto iFilter = temp_filter_list.begin(); iFilter != temp_filter_list.end(); iFilter++)
	{
		AddFilter((*iFilter)->filter_type, (*iFilter)->genre_param, (*iFilter)->year_param);
	}

	// now release the memory from the temp_filter_list pointers
	for (auto iFilter = temp_filter_list.begin(); iFilter != temp_filter_list.end(); iFilter++)
	{
		delete(*iFilter);
	}
#ifdef _DEBUG
	cout << "Number of filters after clear = " << filter_list.size() << endl;
#endif
}


void MovieBrowser::draw()
{
	//draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br_bgnd);

	//draw movies
	for (auto iMovie = filtered_movie_list.begin(); iMovie != filtered_movie_list.end(); ++iMovie)
	{
		(*iMovie)->draw();
	}
	//draw arrows
	if (ra)
		ra->draw();
	if (la)
		la->draw();
	// draw dock
	if (dock)
		dock->draw();

}

	void MovieBrowser::init()
	{
	}

	void MovieBrowser::scroll_right()
	{
		if (nScroll > 0)
		{
			for (auto iMovie = filtered_movie_list.begin(); iMovie != filtered_movie_list.end(); ++iMovie)
			{
				(*iMovie)->wm->scroll_right();
			}
			nScroll--;
		}
	}

	void MovieBrowser::scroll_left()
	{
		if (nScroll < nMovies-4) // minus the number of movies that can be displayed in canvas
		{
			for (auto iMovie = filtered_movie_list.begin(); iMovie != filtered_movie_list.end(); ++iMovie)
			{
				(*iMovie)->wm->scroll_left();
			}
			nScroll++;
		}
	}

	MovieBrowser::MovieBrowser()
	{
		br_bgnd.texture = std::string(ASSET_PATH) + "background.png";
		br_bgnd.outline_opacity = 0.0f;
		ra = NULL;
		la = NULL;
		nMovies = nScroll = 0;
	}

	MovieBrowser::~MovieBrowser()
	{
		filtered_movie_list.clear();
		for (auto iMovie = movie_list.begin(); iMovie != movie_list.end(); ++iMovie)
		{
			delete(*iMovie);
		}
		if (ra)
			delete ra;
		if (la)
			delete la;
		if (dock)
			delete dock;
		
		DestroyFilterList();
	}
