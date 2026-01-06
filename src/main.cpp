#include <raylib.h>
#include <raymath.h>

#include <vector>

#include <iostream>
#include <stdio.h>

class Object {
    public:
        Vector2 center;
        float radius;
        Vector2 velocity;
        Vector2 acceleration;
        Color color;
        
    public:
        Object(Vector2 center, 
            int radius,
            Vector2 velocity,
            Vector2 acceleration, 
            Color color):
                center(center), 
                radius(radius),
                velocity(velocity),
                acceleration(acceleration), 
                color(color) {}

        void draw() {
            DrawCircleV(this->center, this->radius, this->color);
        }

        void update() {
            this->center = Vector2Add(this->center, 
                Vector2Scale(this->velocity, GetFrameTime()));
            
            this->velocity = Vector2Add(this->velocity, 
                Vector2Scale(this->acceleration, GetFrameTime()));
        }
};

float Collide(const Object &obj1, const Object &obj2) {
    float distance = Vector2Distance(obj1.center, obj2.center);
    float minimum = obj1.radius + obj2.radius;
    return minimum - distance;
}

void SolveCollision(Object &obj1, Object &obj2, float dt) {
    
}

void UpdatePhysics(std::vector<Object> &objects, float dt) {
    for (int i = 0; i < objects.size(); i++) {
        for (int j = i + 1; j < objects.size(); j++) {
            if (Collide(objects[i], objects[j]) > 0) {
                SolveCollision(objects[i], objects[j], dt);
            }
        }
    }
}

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(900, 900, "Physics Simulator");

    std::vector<Object> objects;
    objects.push_back(Object(Vector2{200, 400}, 
        50.0f,
        Vector2{0, 0},
        Vector2{0, 100},
        Color{255, 0, 0, 255}
    ));
    
    objects.push_back(Object(
        Vector2{400, 400}, 
        50.0f,
        Vector2{0, 0},
        Vector2{0, 100},
        Color{255, 0, 0, 255}
    ));

    while (!WindowShouldClose()) {        
        UpdatePhysics(objects, GetFrameTime());
        // for (Object &obj: objects) obj.update();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (Object &obj: objects) obj.draw();
        EndDrawing();
    }

    CloseWindow();
}
