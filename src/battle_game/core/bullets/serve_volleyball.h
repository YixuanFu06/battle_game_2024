#pragma once
#include "battle_game/core/bullet.h"

namespace battle_game::bullet {
class ServeVolleyball : public Bullet {
 public:
  ServeVolleyball(GameCore *core,
             uint32_t id,
             uint32_t unit_id,
             uint32_t player_id,
             glm::vec2 position,
             float rotation,
             float damage_scale,
             glm::vec2 velocity);
  ~ServeVolleyball() override;
  void Render() override;
  void Update() override;

 private:
  glm::vec2 velocity_{};
  std::vector<uint32_t> HadHit_;
  float radius_{};
  float center_distance_;
};
}  // namespace battle_game::bullet
