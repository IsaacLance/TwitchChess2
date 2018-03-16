# Twitch Plays Chess
### CIS 330 project by Justin Robles and Isaac Lance

## Requirements
Python 2.7 and above
Selenium (included in repo)
Chromedriver (included in repo)
Twitch API

## Project Goals
We want to be able to create a livesteam on twitch of chess.com where users can input commands into the chat to vote on the next move. This will be done using Python and C++ in tandem where Python will be used to make the pieces move and C++ will be used to read the chat inputs and tell python where to move pieces.

## How to build
Using the Makefile in the driver directory use the command
```
make Reset
```
If this fails it is most likely because the python path is incorrect. For example, on my laptop I need to include "Python/Python.h" but when I researched it other people include just "Python.h".

### Note:
Running the project will not stream to twitch but a chrome window will open and if you go to my channel (https://www.twitch.tv/yung_jrobes) you should be able to use the chat to test it even if it offline.


# Authors
* Issac Lance - Ilance@uoregon.edu
* Justin Robles - Jrobles@uoregon.edu


