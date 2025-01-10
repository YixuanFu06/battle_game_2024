#pragma once
#include "battle_game/core/unit.h"

namespace battle_game::unit {
class VolleyTank : public Unit {
 public:
  VolleyTank(GameCore *game_core, uint32_t id, uint32_t player_id);
  void Render() override;
  void Update() override;
  [[nodiscard]] bool IsHit(glm::vec2 position) const override;

 protected:
  void TankMove(float move_speed, float rotate_angular_speed);
  void TurretRotate();
  void Fire();
  void Serve();
  void Spike();
  [[nodiscard]] const char *UnitName() const override;
  [[nodiscard]] const char *Author() const override;

  float turret_rotation_{0.0f};
  uint32_t fire_count_down_{0};
  uint32_t serve_count_down_{0};
  uint32_t spike_count_down_{0};
  uint32_t mine_count_down_{0};

  const double serve_CD_sec_ = 5; // 5 seconds' CD
  const double spike_CD_sec_ = 5; // 5 seconds' CD
  const float spike_num_{13};
  const float spike_range_degree_{120.0f};
  const float spike_range_{spike_range_degree_ / 360};
};
}  // namespace battle_game::unit
