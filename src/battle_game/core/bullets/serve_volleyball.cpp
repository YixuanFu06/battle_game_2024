#include "battle_game/core/bullets/volleyball.h"

#include "battle_game/core/game_core.h"
#include "battle_game/core/particles/particles.h"

namespace battle_game::bullet {
ServeVolleyball::ServeVolleyball(GameCore *core,
                       uint32_t id,
                       uint32_t unit_id,
                       uint32_t player_id,
                       glm::vec2 position,
                       float rotation,
                       float damage_scale,
                       glm::vec2 velocity)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
      velocity_(velocity) {
  radius_ = 0.85f;
  center_distance_ = 0.9f;
}

void ServeVolleyball::Render() {
  SetTransformation(position_, rotation_, glm::vec2{1.5f, 2.0f});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/serve_volleyball.png");
  DrawModel(0);
}

void ServeVolleyball::Update() {
  position_ += velocity_ * kSecondPerTick;
  bool should_die = false;

  bool IsBlockedByObstacles = false;
  const int precision = 60;
  for (int i = 0; i < precision; ++i) {
    auto theta = 1 / float(precision) * float(i);
    theta *= glm::pi<float>() * 2.0f;
    auto sin_theta = std::sin(theta);
    auto cos_theta = std::cos(theta);
    auto boundary_position = position_ + glm::vec2{-center_distance_ * std::sin(rotation_) + sin_theta * radius_,
                                                  center_distance_ * std::cos(rotation_) + cos_theta * radius_};
    if (game_core_->IsBlockedByObstacles(boundary_position)) {
      should_die = true;
      break;
    }
  }

  auto &units = game_core_->GetUnits();
  for (auto &unit : units) {
    if (unit.first == unit_id_) {
      continue;
    }
    bool AlreadyHit = false;
    for (auto &hit : HadHit_) {
      if (hit == unit.first) {
        AlreadyHit = true;
        break;
      }
    }
    if (!AlreadyHit) {
      bool IsHit = false;
      for (int i = 0; i < precision; ++i) {
        auto theta = 1 / float(precision) * float(i);
        theta *= glm::pi<float>() * 2.0f;
        auto sin_theta = std::sin(theta);
        auto cos_theta = std::cos(theta);
        auto boundary_position = position_ + glm::vec2{-center_distance_ * std::sin(rotation_) + sin_theta * radius_,
                                                      center_distance_ * std::cos(rotation_) + cos_theta * radius_};
        if (unit.second->IsHit(boundary_position)) {
          IsHit = 1;
          break;
        }
      }
      if (IsHit) {
        game_core_->PushEventDealDamage(unit.first, id_, damage_scale_ * 33.4f);
        // should_die = true; // The bullet will not die after hitting a unit
        HadHit_.push_back(unit.first);
      }
    }
  }

  if (should_die) {
    game_core_->PushEventRemoveBullet(id_);
  }
}

ServeVolleyball::~ServeVolleyball() {
  for (int i = 0; i < 5; i++) {
    game_core_->PushEventGenerateParticle<particle::Smoke>(
        position_ + glm::vec2{-center_distance_ * std::sin(rotation_), center_distance_ * std::cos(rotation_)},
        rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
  }
}
}  // namespace battle_game::bullet
