// LevelA.h
#include "Scene.h"

class LevelUno : public Scene {
public:
    int ENEMY_COUNT = 1;

    ~LevelUno();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};