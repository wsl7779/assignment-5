// LevelA.h
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