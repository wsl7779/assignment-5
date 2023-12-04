// Title.h
#include "Scene.h"

class Title : public Scene {
public:
    ~Title();

    void initialise() override;
    void update(float delta_time, int& g_lives) override;
    void render(ShaderProgram* program) override;
};