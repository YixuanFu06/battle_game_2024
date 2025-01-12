#pragma once
#include "battle_game/core/unit.h"

namespace battle_game::unit {
class VolleyTank : public Unit {
 public:
  VolleyTank(GameCore *game_core, uint32_t id, uint32_t player_id);
  void Render() override;
  void Update() override;
  void RenderHelper() override;
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

/************************ These are defined for CD bar of Spike ************************/

 public:
  void SetSpikeCDBarLength(float new_length);
  void SetSpikeCDBarOffset(glm::vec2 new_offset);
  void SetSpikeCDBarFrontColor(glm::vec4 new_color);
  void SetSpikeCDBarBackgroundColor(glm::vec4 new_color);
  [[nodiscard]] float GetSpikeCDBarLength();
  [[nodiscard]] glm::vec2 GetSpikeCDBarOffset();
  [[nodiscard]] glm::vec4 GetSpikeCDBarFrontColor();
  [[nodiscard]] glm::vec4 GetSpikeCDBarBackgroundColor();
  [[nodiscard]] float BasicMaxSpikeCD() const;
  [[nodiscard]] float GetSpikeCDScale() const;
  [[nodiscard]] float GetMaxSpikeCD() const {
    return std::max(GetSpikeCDScale() * BasicMaxSpikeCD(), 1.0f);
  }

  /*
   * SpikeCD value is in range [0, 1], represents the remaining spikeCD in ratio
   * form. GetSpikeCD() * GetMaxSpikeCD() represent true remaining spikeCD of the
   * unit.
   * */
  [[nodiscard]] float GetSpikeCD() const {
    return spikeCD_;
  }

  /*
   * The value of new_spikeCD will be clamped to [0, 1]
   * */
  void SetSpikeCD(float new_spikeCD) {
    spikeCD_ = std::clamp(new_spikeCD, 0.0f, 1.0f);
  }

  void ShowSpikeCDBar();
  void HideSpikeCDBar();
  void RenderSpikeCDBar();

 protected:
  float spikeCD_{1.0f};
  bool spikeCDbar_display_{true};
  glm::vec2 spikeCDbar_offset_{};
  float spikeCDbar_length_{1.0f};
  glm::vec4 front_spikeCDbar_color_{};
  glm::vec4 background_spikeCDbar_color_{};

/****************************************************************************************/

/************************ These are defined for CD bar of Serve ************************/

 public:
  void SetServeCDBarLength(float new_length);
  void SetServeCDBarOffset(glm::vec2 new_offset);
  void SetServeCDBarFrontColor(glm::vec4 new_color);
  void SetServeCDBarBackgroundColor(glm::vec4 new_color);
  [[nodiscard]] float GetServeCDBarLength();
  [[nodiscard]] glm::vec2 GetServeCDBarOffset();
  [[nodiscard]] glm::vec4 GetServeCDBarFrontColor();
  [[nodiscard]] glm::vec4 GetServeCDBarBackgroundColor();
  [[nodiscard]] float BasicMaxServeCD() const;
  [[nodiscard]] float GetServeCDScale() const;
  [[nodiscard]] float GetMaxServeCD() const {
    return std::max(GetServeCDScale() * BasicMaxServeCD(), 1.0f);
  }

  /*
   * ServeCD value is in range [0, 1], represents the remaining serveCD in ratio
   * form. GetServeCD() * GetMaxServeCD() represent true remaining serveCD of the
   * unit.
   * */
  [[nodiscard]] float GetServeCD() const {
    return serveCD_;
  }

  /*
   * The value of new_serveCD will be clamped to [0, 1]
   * */
  void SetServeCD(float new_serveCD) {
    serveCD_ = std::clamp(new_serveCD, 0.0f, 1.0f);
  }

  void ShowServeCDBar();
  void HideServeCDBar();
  void RenderServeCDBar();

 protected:
  float serveCD_{1.0f};
  bool serveCDbar_display_{true};
  glm::vec2 serveCDbar_offset_{};
  float serveCDbar_length_{1.0f};
  glm::vec4 front_serveCDbar_color_{};
  glm::vec4 background_serveCDbar_color_{};

/****************************************************************************************/

};
}  // namespace battle_game::unit
