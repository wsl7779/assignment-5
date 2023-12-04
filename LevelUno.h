/**
* Author: Will Lee
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Scene.h"

class LevelUno : public Scene {
public:
    const int ENEMY_COUNT = 1;

    LevelUno(int g_lives);
    ~LevelUno();

    void initialise() override;
    void update(float delta_time, int& g_lives) override;
    void render(ShaderProgram* program) override;
};