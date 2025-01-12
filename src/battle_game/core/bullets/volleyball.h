#pragma once
#include <bits/stdint-uintn.h>
#include "battle_game/core/bullet.h"

namespace battle_game::bullet {
class Volleyball : public Bullet {
 public:
  Volleyball(GameCore *core,
             uint32_t id,
             uint32_t unit_id,
             uint32_t player_id,
             glm::vec2 position,
             float rotation,
             float damage_scale,
             glm::vec2 velocity);
  ~Volleyball() override;
  void Render() override;
  void Update() override;

 private:
  std::pair<glm::vec2, float> reflect(std::pair<glm::vec2, float> input_velocity, glm::vec2 normal);
  bool enable_rotation_{true};
  glm::vec2 velocity_{};
  float angular_velocity_{};
  uint32_t reflect_count_{};
  const uint32_t max_reflect_count_{5};
  const float radius_{0.15f};
};
}  // namespace battle_game::bullet
