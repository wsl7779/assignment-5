// LevelA.h
#include "Scene.h"

class LevelDos : public Scene {
public:
    int ENEMY_COUNT = 1;

    LevelDos(int g_lives);
    ~LevelDos();

    void initialise() override;
    void update(float delta_time, int& g_lives) override;
    void render(ShaderProgram* program) override;
};