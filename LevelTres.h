// LevelA.h
#include "Scene.h"

class LevelTres : public Scene {
public:
    const int ENEMY_COUNT = 1;

    LevelTres(int g_lives);
    ~LevelTres();

    void initialise() override;
    void update(float delta_time, int& g_lives) override;
    void render(ShaderProgram* program) override;
};