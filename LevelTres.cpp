

// LevelA.cpp
#include "LevelTres.h"
#include "Utility.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 5

unsigned int LevelTres_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    53, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 55,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

LevelTres::LevelTres(int g_lives) {
    lives = g_lives;
}

LevelTres::~LevelTres()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.dead_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelTres::initialise()
{
    m_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LevelTres_DATA, map_texture_id, 1.0f, 22, 7);

    // Code from main.cpp's initialise()
     // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(6.0f, 2.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/yuuka trans.png");

    // Walking
    /*
    m_state.player->m_walking[m_state.player->LEFT] = new int[4] { 1, 5, 9, 13 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.player->m_walking[m_state.player->UP] = new int[4] { 2, 6, 10, 14 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[4] { 0, 4, 8, 12 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  // start George looking left
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 4;
    m_state.player->m_animation_rows = 4;
    m_state.player->set_height(0.8f);
    m_state.player->set_width(0.8f);
    */

    // Jumping
    m_state.player->m_jumping_power = 5.0f;

    /**
     Enemies' stuff */

    GLuint enemy_texture_id = Utility::load_texture("assets/yuuka trans.png");


    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(BOSS);
    m_state.enemies[0].set_ai_state(BOSSFIGHT);
    m_state.enemies[0].m_texture_id = enemy_texture_id;
    m_state.enemies[0].set_position(glm::vec3(14.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].set_speed(1.0f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));


    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_state.bgm = Mix_LoadMUS("assets/music/dolce.mp3");
    //Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME);

    m_state.dead_sfx = Mix_LoadWAV("assets/music/dead.mp3");
}

void LevelTres::update(float delta_time, int& g_lives)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, g_lives);
    lives = g_lives;

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.enemies, m_state.player, ENEMY_COUNT, m_state.map, g_lives);
    }

    if (!m_state.enemies[0].get_active()) {
        m_state.next_scene_id = 4;
    }
    if (m_state.player->get_position().y < -10.0f) {
        g_lives--;
        m_state.player->set_position(glm::vec3(0.0f, 2.0f, 0.0f));
    }
}


void LevelTres::render(ShaderProgram* program)
{
    m_state.map->render(program);
    m_state.player->render(program);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }

    GLuint text_texture_id = Utility::load_texture("assets/font1.png");
    Utility::draw_text(program, text_texture_id, std::string("You must kill to win."), 0.25f, 0.0f, glm::vec3(1.0f, 2.0f, 0.0f));
}
