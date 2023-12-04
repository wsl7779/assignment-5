﻿/**
* Author: Will Lee
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_LEFT_EDGE 0.5f

#include "Utility.h"
#include "Scene.h"
#include "Title.h"
#include "LevelUno.h"
#include "LevelDos.h"
#include "LevelTres.h"
#include "Win.h"
#include <SDL_mixer.h>

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

// ————— CONSTANTS ————— //
const int   WINDOW_WIDTH = 1080,
WINDOW_HEIGHT = 720;

const float BG_RED = 0.1922f,
BG_BLUE = 0.549f,
BG_GREEN = 0.9059f,
BG_OPACITY = 1.0f;

const int   VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char  V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;


// ————— GLOBALS ————— //

Scene* g_current_scene;
Title* g_title;
LevelUno* g_level_uno;
LevelDos* g_level_dos;
LevelTres* g_level_tres;
Win* g_win;

Scene* g_levels[5];

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;
glm::vec3 g_camera_pos = glm::vec3(0.0f);
int g_lives = 3;
bool g_dead = false;
Mix_Chunk* g_dead_sfx;

void switch_to_scene(Scene* scene)
{
    g_current_scene = scene;
    g_current_scene->initialise();
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("im jumping everywhere",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    // ————— SET-UP ————— //

    g_title = new Title();
    g_level_uno = new LevelUno(g_lives);
    g_level_dos = new LevelDos(g_lives);
    g_level_tres = new LevelTres(g_lives);
    g_win = new Win();

    g_levels[0] = g_title;
    g_levels[1] = g_level_uno;
    g_levels[2] = g_level_dos;
    g_levels[3] = g_level_tres;
    g_levels[4] = g_win;

    switch_to_scene(g_levels[0]);


    // ————— GENERAL ————— //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    if (g_current_scene == g_title ) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: g_game_is_running = false;
                    break;
                case SDLK_RETURN: switch_to_scene(g_levels[1]);
                    break;
                default:
                    break;
                }
            }
        }
    }
    else if (g_current_scene != g_win) {
        g_current_scene->m_state.player->set_movement(glm::vec3(0.0f));
        

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: g_game_is_running = false;
                    break;

                case SDLK_SPACE:
                    if (g_current_scene->m_state.player->m_collided_bottom)
                    {
                        g_current_scene->m_state.player->m_is_jumping = true;
                    }
                    break;

                default:
                    break;
                }
            }
        }


        const Uint8* key_state = SDL_GetKeyboardState(NULL);

        if (key_state[SDL_SCANCODE_LEFT])
        {
            g_current_scene->m_state.player->move_left();
        }
        else if (key_state[SDL_SCANCODE_RIGHT])
        {
            g_current_scene->m_state.player->move_right();
        }
        if (key_state[SDL_SCANCODE_UP])
        {

        }

        if (glm::length(g_current_scene->m_state.player->get_movement()) > 1.0f)
        {
            g_current_scene->m_state.player->set_movement(glm::normalize(g_current_scene->m_state.player->get_movement()));
        }
    }
    else
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: g_game_is_running = false;
                    break;


                default:
                    break;
                }
            }
        }
    }
}

void update()
{
    if (g_lives) {
        float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
        float delta_time = ticks - g_previous_ticks;
        g_previous_ticks = ticks;

        // ————— FIXED TIMESTEP ————— //
        // STEP 1: Keep track of how much time has passed since last step
        delta_time += g_accumulator;

        // STEP 2: Accumulate the ammount of time passed while we're under our fixed timestep
        if (delta_time < FIXED_TIMESTEP)
        {
            g_accumulator = delta_time;
            return;
        }

        // STEP 3: Once we exceed our fixed timestep, apply that elapsed time into the objects' update function invocation
        while (delta_time >= FIXED_TIMESTEP)
        {
            g_current_scene->update(FIXED_TIMESTEP, g_lives);

            delta_time -= FIXED_TIMESTEP;
        }

        g_accumulator = delta_time;

        g_view_matrix = glm::mat4(1.0f);

        if (g_current_scene != g_title && g_current_scene != g_win) {
                g_camera_pos.x = -g_current_scene->m_state.player->get_position().x + LEVEL1_LEFT_EDGE;
                g_view_matrix = glm::translate(g_view_matrix, glm::vec3(g_camera_pos.x, 0, 0));
        }
    }
    else if (!g_dead) {
        Mix_PlayChannel(-1, g_current_scene->m_state.dead_sfx, 0);
        g_dead = true;
    }
}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
    GLuint text_texture_id = Utility::load_texture("assets/font1.png");

    glClear(GL_COLOR_BUFFER_BIT);

    // ————— RENDERING THE SCENE (i.e. map, character, enemies...) ————— //
    g_current_scene->render(&g_shader_program);

    if (g_current_scene != g_title && g_current_scene != g_win) {
        Utility::draw_text(&g_shader_program, text_texture_id, std::string("Lives:"), 0.25f, 0.0f, glm::vec3(g_current_scene->m_state.player->get_position().x + 1.0f, 3.0f, 0.0f));
        Utility::draw_text(&g_shader_program, text_texture_id, std::string(std::to_string(g_lives)), 0.25f, 0.0f, glm::vec3((g_current_scene->m_state.player->get_position().x + 3.0f), 3.0f, 0.0f));
        if (!g_lives) {
            Utility::draw_text(&g_shader_program, text_texture_id, std::string("YOU LOSE"), 0.5f, 0.0f, glm::vec3(g_current_scene->m_state.player->get_position().x - 1.0f, 0.0f, 0.0f));
        }
    }

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

    delete g_title;
    delete g_level_uno;
    delete g_level_dos;
    delete g_level_tres;
    delete g_win;

    Mix_FreeChunk(g_dead_sfx);
}

// ————— DRIVER GAME LOOP ————— /
int main(int argc, char* argv[])
{
    initialise();

    while (g_game_is_running)
    {
        process_input();
        update();

        if (g_current_scene->m_state.next_scene_id >= 0) switch_to_scene(g_levels[g_current_scene->m_state.next_scene_id]);

        render();
    }

    shutdown();
    return 0;
}