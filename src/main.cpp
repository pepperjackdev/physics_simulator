#include <raylib.h>
#include <raymath.h>

#include <vector>

#include <iostream>
#include <stdio.h>

Vector2 GetGravitationalField(Vector2 position) {
    return Vector2Scale(Vector2Normalize(Vector2Subtract(Vector2{450, 450}, position)), 100);
}

class Object {
    public:
        float mass;
        Vector2 center;
        float radius;
        Vector2 velocity;
        Vector2 acceleration;
        Color color;
        
    public:
        Object(float mass,
            Vector2 center, 
            int radius,
            Vector2 velocity,
            Vector2 acceleration, 
            Color color):
                mass(mass),
                center(center), 
                radius(radius),
                velocity(velocity),
                acceleration(acceleration), 
                color(color) {}

        void draw() {
            DrawCircleV(this->center, this->radius, this->color);
            //DrawLineV(this->center, Vector2Add(this->center, this->velocity), RED);
            //DrawLineV(this->center, Vector2Add(this->center, this->acceleration), BLUE);
        }

        void update(float dt) {
            this->acceleration = GetGravitationalField(this->center);

            this->center = Vector2Add(this->center, 
                Vector2Scale(this->velocity, dt));
            
            this->velocity = Vector2Add(this->velocity, 
                Vector2Scale(this->acceleration, dt));
        }
};

bool Collide(const Object &obj1, const Object &obj2) {
    return (obj1.radius + obj2.radius) - Vector2Distance(obj1.center, obj2.center) > 0;
}

void SolveOverlapping(Object &obj1, Object &obj2, float dt) {
    Vector2 overlap = Vector2Scale(Vector2Normalize(Vector2Subtract(obj1.center, obj2.center)), obj1.radius + obj2.radius);
    obj1.center = Vector2Add(obj1.center, Vector2Scale(overlap, 0.5 * dt));
    obj2.center = Vector2Add(obj2.center, Vector2Scale(overlap, -0.5 * dt));
}

void SolveMomentum(Object &obj1, Object &obj2) {
    Vector2 velocity1 = Vector2Scale(
        Vector2Add(
            Vector2Scale(obj2.velocity, 2*obj2.mass), 
            Vector2Scale(obj1.velocity, obj1.mass - obj2.mass)), 
        1.0f32/(obj1.mass + obj2.mass));
    Vector2 velocity2 = Vector2Scale(
        Vector2Add(
            Vector2Scale(obj1.velocity, 2*obj1.mass), 
            Vector2Scale(obj2.velocity, obj2.mass - obj1.mass)), 
        1.0f32/(obj1.mass + obj2.mass));
    obj1.velocity = velocity1;
    obj2.velocity = velocity2;
}

void UpdatePhysics(std::vector<Object> &objects, float dt) {
    for (Object &obj: objects) obj.update(dt);
    for (int i = 0; i < objects.size(); i++) {
        for (int j = i + 1; j < objects.size(); j++) {
            if (Collide(objects[i], objects[j])) {
                SolveOverlapping(objects[i], objects[j], dt);
                SolveMomentum(objects[i], objects[j]);
            }
        }
    }
}

void UpdatePhysics() {
    
};

void UpdatePhysicsSubsteps(std::vector<Object> &objects, float dt, float steps) {
    float dt_step = dt / steps;
    for (;steps--;) UpdatePhysics(objects, dt_step);
}

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(900, 900, "Physics Simulator");

    std::vector<Object> objects;

    while (!WindowShouldClose()) {
        DrawText(TextFormat("%d objects", objects.size()), 10, 0, 30, BLACK);
        DrawText(TextFormat("%dFPS", GetFPS()), 10, 35, 30, BLACK);
        UpdatePhysicsSubsteps(objects, GetFrameTime(), 100);
        if /*(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))*/ (true) {
            objects.push_back(Object(
                1.0f,
                GetMousePosition(), 
                10.0f,
                Vector2{0, 0},
                Vector2{0, 100},
                Color{0, 255, 0, 255}
            ));
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (Object &obj: objects) obj.draw();
        EndDrawing();
    }

    CloseWindow();
}
