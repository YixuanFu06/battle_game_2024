#pragma once
#include <glm/fwd.hpp>
#include "battle_game/core/obstacle.h"

namespace battle_game::obstacle {
class Setter : public Obstacle {
 public:
  Setter(GameCore *game_core,
        uint32_t id,
        std::vector<glm::vec2> vertices,
        float rotation = 0.0f,
        glm::vec2 scale = glm::vec2{1.0f, 1.0f});

  std::pair<glm::vec2, glm::vec2> GetSurfaceNormal(glm::vec2 origin, glm::vec2 terminus) override;

 private:
  [[nodiscard]] bool IsBlocked(glm::vec2 p) const override;
  glm::vec2 GetIntersection(glm::vec2 origin1, glm::vec2 terminus1, glm::vec2 origin2, glm::vec2 terminus2) const; // returns {0, 0} if no intersection
  void Render() override;
  glm::vec2 scale_{1.0f};
  glm::vec2 texture_{0.0f};
  glm::vec4 color_{0.0f, 0.0f, 0.0f, 1.0f};
  std::vector<glm::vec2> vertices_;
  std::vector<bool> inner_side_; // true for anti-clockwise, false for clockwise
  uint32_t setter_model_index = 0xffffffffu;
};
}  // namespace battle_game::obstacle
