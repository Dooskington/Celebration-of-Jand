# Celebration of Jand

> "Brothers! Jand is once again upon us, and we haven't even begun the sacrificial bonfire! If we don't finish it in time... I shudder to think of what horrors will befall our land..."
>  &nbsp;&nbsp;&nbsp;&nbsp;- Aeret Drazius, High Priest of Aramoor

Celebration of Jand is a game that I made in 48 hours for Ludum Dare 34. Due to a tie in the theme voting, there were two themes: "Growing" and "Two button controls". This game follows both of those themes.

![Celebration of Jand](http://dooskington.com/static/images/screenshots/celebration-of-jand-screenshot.png)

The goal of the game is to command your peons, gather enough resources, and prepare for Jand by building the sacrificial bonfire. Trees are worth less resources than stone, but are more plentiful.

Using your mouse, you can left click to select individual peons, or do a box selection. Once you have some peons selected, you can right click on a resource to tell them to gather it.

To gain additional peons, you can sacrifice one peon and 100 resources by selecting a peon and right clicking on the bonfire.

I wrote an article on the game [here](http://dooskington.com/ludum-dare-34-postmortem-celebration-of-jand/)

Technology     | Purpose
---------------|----------
**C++14**      | Core
**SDL2**       | Windowing, Rendering, & Input
**SDL_mixer**  | Audio
**SDL_ttf**    | Font Rendering
**SDL_image**  | Image Loading
**VS2015**     | Windows Compilation
**make**       | Linux and OSX Compilation