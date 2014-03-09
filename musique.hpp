/*===============================================================================================
 3d Example
 Copyright (c), Firelight Technologies Pty, Ltd 2004-2011.

 This example shows how to basic 3d positioning
===============================================================================================*/
#include "build/api/inc/fmod.hpp"
#include "build/api/inc/fmod_errors.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "wincompat.h"

#include <string>

//const int INTERFACE_UPDATETIME = 50;        // 50ms update for interface

const std::string musiques_tetris[3]={
  "../Ressources/tetris1.ogg",
  "../Ressources/tetris2.ogg",
  "../Ressources/tetris3.ogg"
};

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

class Music
{
public:
  FMOD::System    *system;
  FMOD::Sound     *sound;
  FMOD::Channel   *channel;
  FMOD_RESULT      result;
  unsigned int     version;

  bool paused;
  int current_sound;

  Music()
  {
    paused=false;
    current_sound=0;
    channel=0;
    //  Create a System object and initialize.
    result = FMOD::System_Create(&system);
    ERRCHECK(result);
    result = system->getVersion(&version);
    ERRCHECK(result);
    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        exit(0);
    }
    result = system->init(10, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);
  }
  ~Music()
  {
    //  Shut down
    result = sound->release();
    ERRCHECK(result);
    result = system->close();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);
  }
    
  void load(const char* name)
  {
    // Load some sounds
    result = system->createSound(name, FMOD_SOFTWARE | FMOD_3D, 0, &sound);
    ERRCHECK(result);
    result = sound->setMode(FMOD_LOOP_NORMAL);
    ERRCHECK(result);
  }
  void load(const int ref){this->load(musiques_tetris[ref].c_str());}
  void play()
  {
    // Play sounds
    paused=false;
    result = system->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
    ERRCHECK(result);
    result = channel->setPaused(paused);
    ERRCHECK(result);
  }
  void switchpause()
  {
    paused=!paused;
    result = channel->setPaused(paused);
    ERRCHECK(result);
  }
  void next()
  {
    current_sound=(current_sound+1)%3;
    result = sound->release();
    ERRCHECK(result);
    load(current_sound);
    play();
  }
};


int fmod_exemple_main(int argc, char *argv[])
{
  Music musique;
  musique.load("2.ogg");

  // Main loop
  musique.play();
  sleep(5);
  musique.switchpause();
  sleep(5);
  musique.switchpause();
  sleep(5);


  return 0;
}
