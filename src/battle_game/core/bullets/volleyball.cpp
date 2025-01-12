#include "battle_game/core/bullets/volleyball.h"

#include "battle_game/core/game_core.h"
#include "battle_game/core/particles/particles.h"

namespace battle_game::bullet {
Volleyball::Volleyball(GameCore *core,
                       uint32_t id,
                       uint32_t unit_id,
                       uint32_t player_id,
                       glm::vec2 position,
                       float rotation,
                       float damage_scale,
                       glm::vec2 velocity)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
      velocity_(velocity), angular_velocity_(0), reflect_count_(0) {
}

void Volleyball::Render() {
  SetTransformation(position_, rotation_, glm::vec2{radius_});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/volleyball.png");
  DrawModel(0);
}

void Volleyball::Update() {
  position_ += velocity_ * kSecondPerTick;
  rotation_ += angular_velocity_ * kSecondPerTick;
  bool should_die = false;
  if (game_core_->IsBlockedByObstacles(position_)) {
    auto obstacle = game_core_->GetBlockedObstacle(position_);
    if (obstacle == nullptr || reflect_count_ >= max_reflect_count_) {
      should_die = true;
    } else {
      auto normal = obstacle->GetSurfaceNormal(position_ - velocity_, position_ + velocity_);
      if (normal == std::pair{glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 0.0f}}) {
        should_die = true;
      } else {
        auto general_velocity = reflect(std::pair(velocity_, angular_velocity_), normal.second);
        velocity_ = general_velocity.first;
        angular_velocity_ = general_velocity.second;
        position_ = normal.first;
        damage_scale_ *= float(max_reflect_count_ - reflect_count_) / float(max_reflect_count_ - reflect_count_ + 1);
        reflect_count_++;
      }
    }
  }

  auto &units = game_core_->GetUnits();
  for (auto &unit : units) {
    if (unit.first == unit_id_) {
      continue;
    }
    if (unit.second->IsHit(position_)) {
      game_core_->PushEventDealDamage(unit.first, id_, damage_scale_ * 10.0f);
      should_die = true;
    }
  }

  if (should_die) {
    game_core_->PushEventRemoveBullet(id_);
  }
}

std::pair<glm::vec2, float> Volleyball::reflect(std::pair<glm::vec2, float> input_velocity, glm::vec2 normal) {
  auto velocity = input_velocity.first;
  auto angular_velocity = input_velocity.second;
  auto dot = glm::dot(velocity, normal);
  auto v_normal = dot * normal;
  auto v_tangent = velocity - v_normal;
  auto tangent = glm::normalize(v_tangent);
  if (enable_rotation_) {
    auto reflect_velocity = -v_normal + glm::vec2{3.0f / 7.0f * v_tangent.x, 3.0f / 7.0f * v_tangent.y}
                          + glm::vec2{4.0f / 7.0f * angular_velocity * radius_ * tangent.x, 4.0f / 7.0f * angular_velocity * radius_ * tangent.y};
    auto reflected_angular_velocity = 10.0f / 7.0f * v_tangent.length() / radius_ - 3.0f / 7.0f * angular_velocity;
    return {reflect_velocity, reflected_angular_velocity};
  } else {
    auto reflect_velocity = -v_normal + v_tangent;
    auto reflect_angular_velocity = angular_velocity;
    return {reflect_velocity, reflect_angular_velocity};
  }
}

Volleyball::~Volleyball() {
  for (int i = 0; i < 5; i++) {
    game_core_->PushEventGenerateParticle<particle::Smoke>(
        position_, rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
  }
}
}  // namespace battle_game::bullet
