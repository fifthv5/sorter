#include <raylib.h>

#include <cstddef>
#include <execution>
#include <random>
#include <vector>

constexpr float MAX_INITIAL_SPEED{300.0f};
constexpr float MAX_RADIUS{7.0f};
constexpr float MIN_RADIUS{4.0f};
constexpr float DRAG{0.99f};
constexpr float GRAVITY{500.0f};
constexpr float BOINCE{0.8f};

struct Particle {
  Vector2 position{};
  Vector2 velocity{};
  float radius{};
};

float random_range(float min, float max) {
  static std::mt19937 generator{std::random_device()()};
  std::uniform_real_distribution<float> range{min, max};
  return range(generator);
}

class ParticleSimulation {
public:
  static constexpr size_t INITIAL_PARTICLES_AMOUNT{1000};

public:
  ParticleSimulation() {
    for (size_t i{}; i < INITIAL_PARTICLES_AMOUNT; i++) {
      Particle particle{};

      particle.position = Vector2{random_range(0.0f, GetScreenWidth()),
                                  random_range(0.0f, GetScreenHeight())};
      particle.velocity =
          Vector2{random_range(-MAX_INITIAL_SPEED, MAX_INITIAL_SPEED),
                  random_range(-MAX_INITIAL_SPEED, MAX_INITIAL_SPEED)};
      particle.radius = random_range(MIN_RADIUS, MAX_RADIUS);
      m_particles.push_back(particle);
    }
  }

  void draw() {
    for (const auto &particle : m_particles) {
      DrawCircle(particle.position.x, particle.position.y, particle.radius,
                 WHITE);
    }
  }

  void update(float dt) {

    static Vector2 last_window_position{GetWindowPosition()};
    for (auto &particle : m_particles) {
      float dx{GetWindowPosition().x - last_window_position.x};
      float dy{GetWindowPosition().y - last_window_position.y};
      // Apply drag to velocity (not position)
      particle.velocity.x += dx;
      particle.velocity.y += dy;

      particle.velocity.x *= DRAG;
      particle.velocity.y *= DRAG;

      // Apply gravity scaled by dt
      particle.velocity.y += GRAVITY * dt;

      // Update position based on velocity
      particle.position.x += particle.velocity.x * dt;
      particle.position.y += particle.velocity.y * dt;

      bool hit_right{particle.position.x + particle.radius > GetScreenWidth()};
      bool hit_left{particle.position.x - particle.radius < 0};
      bool hit_down{particle.position.y + particle.radius > GetScreenHeight()};
      bool hit_up{particle.position.y - particle.radius < 0};

      if (hit_right) {
        particle.position.x = GetScreenWidth() - particle.radius;
        particle.velocity.x *= -BOINCE;
      }

      if (hit_left) {
        particle.position.x = particle.radius;
        particle.velocity.x *= -BOINCE;
      }

      if (hit_up) {
        particle.position.y = particle.radius;
        particle.velocity.y *= -BOINCE;
      }

      if (hit_down) {
        particle.position.y = GetScreenHeight() - particle.radius;
        particle.velocity.y *= -BOINCE;
      }
    }
    last_window_position = GetWindowPosition();
  }

private:
  std::vector<Particle> m_particles{};
};

int main() {
  InitWindow(1000, 1000, "Particle Simulation");
  SetTargetFPS(60);

  ParticleSimulation particle_simulation{};

  while (!WindowShouldClose()) {
    PollInputEvents();
    BeginDrawing();
    particle_simulation.draw();
    particle_simulation.update(GetFrameTime());
    ClearBackground(BLACK);
    EndDrawing();
  }

  return 0;
}
