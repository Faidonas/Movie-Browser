#include "graphics.h"
#include "MovieBrowser.h"
#include "Config.h"



// The custom callback function that the library calls 
// to check for and set the current application state.
void update(float ms)
{
    MovieBrowser* moviebr = reinterpret_cast <MovieBrowser*>(graphics::getUserData());
    moviebr->update();

}



// The window content drawing function.
void draw()
{
    MovieBrowser* moviebr = reinterpret_cast <MovieBrowser*>(graphics::getUserData());
    moviebr->draw();
}



int main()
{
    MovieBrowser mb;

    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Movie Browser");


    graphics::setUserData(&mb);

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);


    


    graphics::startMessageLoop();

    return 0;
}