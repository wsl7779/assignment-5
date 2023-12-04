// LevelA.cpp
#include "Win.h"
#include "Utility.h"

Win::Win() {}


Win::~Win()
{
    //Mix_FreeChunk(m_state.jump_sfx);
    //Mix_FreeMusic(m_state.bgm);
}

void Win::initialise()
{

    /**
     BGM and SFX
     */
     /*
     Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

     m_state.bgm = Mix_LoadMUS("assets/music/dolce.mp3");
     Mix_PlayMusic(m_state.bgm, -1);
     Mix_VolumeMusic(MIX_MAX_VOLUME);
     */
}

void Win::update(float delta_time, int& g_lives)
{
}


void Win::render(ShaderProgram* program)
{
    GLuint text_texture_id = Utility::load_texture("assets/font1.png");
    Utility::draw_text(program, text_texture_id, std::string("You win."), 0.25f, 0.0f, glm::vec3(-3.0f, 0.0f, 0.0f));
}