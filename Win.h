// Title.h
#include "Scene.h"

class Win : public Scene {
public:
    Win();
    ~Win();

    void initialise() override;
    void update(float delta_time, int& g_lives) override;
    void render(ShaderProgram* program) override;
};