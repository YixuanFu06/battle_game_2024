#include "volleytank.h"
#include <GLFW/glfw3.h>

#include "battle_game/core/bullets/bullets.h"
#include "battle_game/core/game_core.h"
#include "battle_game/graphics/graphics.h"

namespace battle_game::unit {

namespace {
uint32_t volley_body_model_index = 0xffffffffu;
uint32_t serve_turret_model_index = 0xffffffffu;
uint32_t spike_turret_model_index = 0xffffffffu;
uint32_t circle_cover_icon = 0xffffffffu;
uint32_t CD_bar_segment_index = 0xffffffffu;
}  // namespace

VolleyTank::VolleyTank(GameCore *game_core, uint32_t id, uint32_t player_id)
    : Unit(game_core, id, player_id) {
  lifebar_offset_ = {0.0f, 1.2f};
  if (!~spike_turret_model_index) {
    auto mgr = AssetsManager::GetInstance();
    {
      /* Spike Turret */
      std::vector<ObjectVertex> turret_vertices;
      std::vector<uint32_t> turret_indices;
      const int precision = 60;
      const float inv_precision = spike_range_ / (float(precision) - 1);
      turret_vertices.push_back({{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.2f, 0.7f, 0.8f}});
      for (int i = 1; i <= precision; i++) {
        auto start_rotation = - spike_range_ / 2;
        auto theta = float(i - 1) * inv_precision + start_rotation;
        theta = theta * glm::pi<float>() * 2.0f + turret_rotation_;
        auto sin_theta = std::sin(theta);
        auto cos_theta = std::cos(theta);
        if (i <= 2 || i >= precision - 1) {
          turret_vertices.push_back({{sin_theta * 2.0f, cos_theta * 2.0f},
                                    {0.0f, 0.0f},
                                    {0.0f, 0.2f, 0.7f, 0.8f}});
        } else {
          turret_vertices.push_back({{sin_theta * 2.0f, cos_theta * 2.0f},
                                    {0.0f, 0.0f},
                                    {0.0f, 0.2f, 0.7f, 0.1f}});
        }
        if (i != precision) {
          turret_indices.push_back(0);
          turret_indices.push_back(i);
          turret_indices.push_back(i + 1);
        }
      }
      spike_turret_model_index =
          mgr->RegisterModel(turret_vertices, turret_indices);
    }
  }

  if (!~serve_turret_model_index) {
    auto mgr = AssetsManager::GetInstance();
    {
      /* Serve Turret */
      serve_turret_model_index = mgr->RegisterModel(
        {
          {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.3f}},
          {{-0.79f, 0.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.3f}},
          {{0.79f, 0.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.3f}},
          {{-0.79f, 25.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.1f}},
          {{0.79f, 25.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.1f}},

          {{-0.8f, 0.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.7f}},
          {{0.8f, 0.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.7f}},
          {{-0.8f, 25.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.1f}},
          {{0.8f, 25.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.1f}},

          {{-0.85f, 0.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.7f}},
          {{0.85f, 0.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.7f}},
          {{-0.85f, 25.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.1f}},
          {{0.85f, 25.0f}, {0.0f, 0.0f}, {0.7f, 0.0f, 0.2f, 0.1f}},
        },
        {0, 1, 3, 0, 2, 4, 0, 3, 4, 1, 3, 5, 2, 4, 6, 3, 5, 7, 4, 6, 8, 5, 7, 9, 6, 8, 10, 7, 9, 11, 8, 10, 12});
    }
  }

  if (!~circle_cover_icon) {
    auto mgr = AssetsManager::GetInstance();
    {
      std::vector<ObjectVertex> turret_vertices;
      std::vector<uint32_t> turret_indices;
      const int precision = 300;
      const float inv_precision = 1.0f / float(precision);
      turret_vertices.push_back({{0, 0},
                                {0.0f, 0.0f},
                                {0.0f, 0.0f, 0.0f, 0.3f}});
      for (int i = 1; i <= precision; i++) {
        auto theta = float(i - 1) * inv_precision;
        theta = theta * glm::pi<float>() * 2.0f;
        auto sin_theta = std::sin(theta);
        auto cos_theta = std::cos(theta);
        turret_vertices.push_back({{sin_theta * 0.9f, cos_theta * 0.9f},
                                  {0.0f, 0.0f},
                                  {0.0f, 0.0f, 0.0f, 0.5f}});
        if (i != precision) {
          turret_indices.push_back(0);
          turret_indices.push_back(i);
          turret_indices.push_back(i + 1);
        }
      }
      turret_indices.push_back(0);
      turret_indices.push_back(precision);
      turret_indices.push_back(1);
      circle_cover_icon =
          mgr->RegisterModel(turret_vertices, turret_indices);
    }
  }

  spikeCDbar_offset_ = {15.75f, -8.5f};
  background_spikeCDbar_color_ = {0.0f, 0.0f, 0.0f, 0.3f};
  front_spikeCDbar_color_ = {0.0f, 0.2f, 0.7f, 0.8f};

  serveCDbar_offset_ = {13.0f, -8.5f};
  background_serveCDbar_color_ = {0.0f, 0.0f, 0.0f, 0.3f};
  front_serveCDbar_color_ = {0.7f, 0.0f, 0.2f, 0.8f};

  if (!~CD_bar_segment_index) {
    auto mgr = AssetsManager::GetInstance();
    {
      /* A segment of ServeCD Bar */
      const int precision = 300; // You should also change the precision in RenderServeCDBar() and RenderSpikeCDBar().
      std::vector<ObjectVertex> serveCD_bar_vertices;
      std::vector<uint32_t> serveCD_bar_indices;
      auto theta = glm::pi<float>() * 2.0f / float(precision);
      auto sin_theta = std::sin(-theta);
      auto cos_theta = std::cos(-theta);

      CD_bar_segment_index = mgr->RegisterModel(
        {
          {{0.0f, 0.9f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
          {{0.0f, 1.1f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
          {{sin_theta * 0.9f, cos_theta * 0.9f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
          {{sin_theta * 1.1f, cos_theta * 1.1f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
        },
        {0, 1, 2, 1, 2, 3});
    }
  }
}

void VolleyTank::Render() {
  battle_game::SetTransformation(position_, rotation_);
  battle_game::SetColor(game_core_->GetPlayerColor(player_id_));

  auto player = game_core_->GetPlayer(player_id_);
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/volleytank.png");
  battle_game::DrawModel(0);
  // battle_game::DrawModel(tank_body_model_index);
}

void VolleyTank::Update() {
  TankMove(3.0f, glm::radians(180.0f));
  TurretRotate();
  Fire();
  Serve();
  Spike();
}

void VolleyTank::TankMove(float move_speed, float rotate_angular_speed) {
  auto player = game_core_->GetPlayer(player_id_);
  if (player) {
    auto &input_data = player->GetInputData();
    glm::vec2 offset{0.0f};
    if (input_data.key_down[GLFW_KEY_W]) {
      offset.y += 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_S]) {
      offset.y -= 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_A]) {
      offset.x -= 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_D]) {
      offset.x += 1.0f;
    }
    float speed = move_speed * GetSpeedScale();
    offset *= kSecondPerTick * speed;
    auto new_position = position_ + offset;
    /*
    auto new_position = position_ + offset;
        position_ + glm::vec2{glm::rotate(glm::mat4{1.0f}, rotation_,
                                          glm::vec3{0.0f, 0.0f, 1.0f}) *
                              glm::vec4{offset, 0.0f, 0.0f}};
    */
    if (!game_core_->IsBlockedByObstacles(new_position)) {
      game_core_->PushEventMoveUnit(id_, new_position);
    }
    float rotation_offset = 0.0f;
    /*
    if (input_data.key_down[GLFW_KEY_A]) {
      rotation_offset += 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_D]) {
      rotation_offset -= 1.0f;
    }
    */
    rotation_offset *= kSecondPerTick * rotate_angular_speed * GetSpeedScale();
    game_core_->PushEventRotateUnit(id_, rotation_ + rotation_offset);
  }
}

void VolleyTank::TurretRotate() {
  auto player = game_core_->GetPlayer(player_id_);
  if (player) {
    auto &input_data = player->GetInputData();
    auto diff = input_data.mouse_cursor_position - position_;
    if (glm::length(diff) < 1e-4) {
      turret_rotation_ = rotation_;
    } else {
      turret_rotation_ = std::atan2(diff.y, diff.x) - glm::radians(90.0f);
    }
  }
}

void VolleyTank::Fire() {
  if (fire_count_down_ == 0) {
    auto player = game_core_->GetPlayer(player_id_);
    if (player) {
      auto &input_data = player->GetInputData();
      if (input_data.mouse_button_down[GLFW_MOUSE_BUTTON_LEFT] && !input_data.key_down[GLFW_KEY_SPACE]) {
        auto velocity = Rotate(glm::vec2{0.0f, 20.0f}, turret_rotation_);
        GenerateBullet<bullet::Volleyball>(
            position_ + Rotate({0.0f, 1.2f}, turret_rotation_),
            turret_rotation_, GetDamageScale(), velocity);
        fire_count_down_ = kTickPerSecond;  // Fire interval 1 second.
      }
    }
  }
  if (fire_count_down_) {
    fire_count_down_--;
  }
}

void VolleyTank::Serve() {
  if (serve_count_down_ == 0) {
    auto player = game_core_->GetPlayer(player_id_);
    if (player) {
      auto &input_data = player->GetInputData();
      if (input_data.mouse_button_down[GLFW_MOUSE_BUTTON_RIGHT] && input_data.key_down[GLFW_KEY_SPACE]) {
        auto velocity = Rotate(glm::vec2{0.0f, 20.0f}, turret_rotation_);
        GenerateBullet<bullet::ServeVolleyball>(
            position_ + Rotate({0.0f, 1.2f}, turret_rotation_),
            turret_rotation_, GetDamageScale(), velocity);
        serve_count_down_ = serve_CD_sec_ * kTickPerSecond;
      }
    }
  }
  if (serve_count_down_) {
    serve_count_down_--;
  }
  SetServeCD(1 - serve_count_down_ / (serve_CD_sec_ * kTickPerSecond));
}

void VolleyTank::Spike() {
  if (spike_count_down_ == 0) {
    auto player = game_core_->GetPlayer(player_id_);
    if (player) {
      auto &input_data = player->GetInputData();
      if (input_data.mouse_button_down[GLFW_MOUSE_BUTTON_LEFT] && input_data.key_down[GLFW_KEY_SPACE]) {
        auto start_rotation = - spike_range_ / 2;
        for (int i = 0; i < spike_num_; ++i) {
          auto theta = spike_range_ / (spike_num_ - 1) * float(i) + start_rotation;
          theta *= glm::pi<float>() * 2.0f;
          auto velocity = Rotate(glm::vec2{0.0f, 20.0f}, turret_rotation_ + theta);
          GenerateBullet<bullet::Volleyball>(
              position_ + Rotate({0.0f, 1.2f}, turret_rotation_ + theta),
              turret_rotation_ + theta, GetDamageScale(), velocity);
          spike_count_down_ = spike_CD_sec_ * kTickPerSecond;
        }
      }
    }
  }
  if (spike_count_down_) {
    spike_count_down_--;
  }
  SetSpikeCD(1 - spike_count_down_ / (spike_CD_sec_ * kTickPerSecond));
}

bool VolleyTank::IsHit(glm::vec2 position) const {
  position = WorldToLocal(position);
  return sqrt(position.x * position.x + position.y * position.y) < 1.0f;
  /*
  return position.x > -0.8f && position.x < 0.8f && position.y > -1.0f &&
         position.y < 1.0f && position.x + position.y < 1.6f &&
         position.y - position.x < 1.6f;
  */
}

/************************ These are defined for CD bar of Spike ************************/

void VolleyTank::SetSpikeCDBarLength(float new_length) {
  spikeCDbar_length_ = std::min(new_length, 0.0f);
}
void VolleyTank::SetSpikeCDBarOffset(glm::vec2 new_offset) {
  spikeCDbar_offset_ = new_offset;
}
void VolleyTank::SetSpikeCDBarFrontColor(glm::vec4 new_color) {
  front_spikeCDbar_color_ = new_color;
}
void VolleyTank::SetSpikeCDBarBackgroundColor(glm::vec4 new_color) {
  background_spikeCDbar_color_ = new_color;
}
float VolleyTank::GetSpikeCDBarLength() {
  return spikeCDbar_length_;
}
glm::vec2 VolleyTank::GetSpikeCDBarOffset() {
  return spikeCDbar_offset_;
}
glm::vec4 VolleyTank::GetSpikeCDBarFrontColor() {
  return front_spikeCDbar_color_;
}
glm::vec4 VolleyTank::GetSpikeCDBarBackgroundColor() {
  return background_spikeCDbar_color_;
}

void VolleyTank::ShowSpikeCDBar() {
  spikeCDbar_display_ = true;
}
void VolleyTank::HideSpikeCDBar() {
  spikeCDbar_display_ = false;
}

void VolleyTank::RenderSpikeCDBar() {
  if (spikeCDbar_display_) {
    auto parent_unit = game_core_->GetUnit(id_);
    auto pos = parent_unit->GetPosition() + spikeCDbar_offset_;
    auto spikeCD = GetSpikeCD();
    const int precision = 300;
    
    battle_game::SetColor(game_core_->GetPlayerColor(player_id_));
    SetTexture(BATTLE_GAME_ASSETS_DIR "textures/volleytank.png");
    SetTransformation(pos - glm::vec2{0.0f, 0.2f}, rotation_, {0.4f, 0.4f});
    battle_game::DrawModel(0);

    const float decoration_radius = 0.65f;
    const float decoration_offset = 0.15f;
    const float angle_inteval = glm::pi<float>() / 6;

    for (int i = -2 ; i <= 2; ++i) {
      auto theta = angle_inteval * float(i);
      SetTexture(BATTLE_GAME_ASSETS_DIR "textures/volleyball.png");
      SetTransformation(pos + glm::vec2{decoration_radius * std::sin(theta), decoration_radius * std::cos(theta) - decoration_offset},
                       theta, glm::vec2{0.1f});
      battle_game::DrawModel(0);
    }

    if (spikeCD != 1) {
      SetTransformation(pos, 0.0f);
      SetTexture(0);
      DrawModel(circle_cover_icon);
    }
    for (int i = 0; i < precision; ++i) {
      auto theta = - float(i) / float(precision);
      theta *= glm::pi<float>() * 2.0f;
      SetTransformation(pos, theta);
      if (i <= spikeCD * precision) {
        SetColor(front_spikeCDbar_color_);
      } else {
        SetColor(background_spikeCDbar_color_);
      }
      SetTexture(0);
      DrawModel(CD_bar_segment_index);
    }
  }
}

/****************************************************************************************/

/************************ These are defined for CD bar of Serve ************************/

void VolleyTank::SetServeCDBarLength(float new_length) {
  serveCDbar_length_ = std::min(new_length, 0.0f);
}
void VolleyTank::SetServeCDBarOffset(glm::vec2 new_offset) {
  serveCDbar_offset_ = new_offset;
}
void VolleyTank::SetServeCDBarFrontColor(glm::vec4 new_color) {
  front_serveCDbar_color_ = new_color;
}
void VolleyTank::SetServeCDBarBackgroundColor(glm::vec4 new_color) {
  background_serveCDbar_color_ = new_color;
}
float VolleyTank::GetServeCDBarLength() {
  return serveCDbar_length_;
}
glm::vec2 VolleyTank::GetServeCDBarOffset() {
  return serveCDbar_offset_;
}
glm::vec4 VolleyTank::GetServeCDBarFrontColor() {
  return front_serveCDbar_color_;
}
glm::vec4 VolleyTank::GetServeCDBarBackgroundColor() {
  return background_serveCDbar_color_;
}

void VolleyTank::ShowServeCDBar() {
  serveCDbar_display_ = true;
}
void VolleyTank::HideServeCDBar() {
  serveCDbar_display_ = false;
}

void VolleyTank::RenderServeCDBar() {
  if (serveCDbar_display_) {
    auto parent_unit = game_core_->GetUnit(id_);
    auto pos = parent_unit->GetPosition() + serveCDbar_offset_;
    auto serveCD = GetServeCD();
    const int precision = 300;
    
    battle_game::SetColor(game_core_->GetPlayerColor(player_id_));
    SetTexture(BATTLE_GAME_ASSETS_DIR "textures/serve_volleyball.png");
    SetTransformation(pos, 0.0f, {0.6f, 0.8f});
    battle_game::DrawModel(0);

    if (serveCD != 1) {
      SetTransformation(pos, 0.0f);
      SetTexture(0);
      DrawModel(circle_cover_icon);
    }
    for (int i = 0; i < precision; ++i) {
      auto theta = - float(i) / float(precision);
      theta *= glm::pi<float>() * 2.0f;
      SetTransformation(pos, theta);
      if (i <= serveCD * precision) {
        SetColor(front_serveCDbar_color_);
      } else {
        SetColor(background_serveCDbar_color_);
      }
      SetTexture(0);
      DrawModel(CD_bar_segment_index);
    }
  }
}

/****************************************************************************************/

void VolleyTank::RenderHelper() {
  battle_game::SetTransformation(position_, rotation_);
  battle_game::SetColor(game_core_->GetPlayerColor(player_id_));

  battle_game::SetRotation(turret_rotation_);
  auto player = game_core_->GetPlayer(player_id_);
  if (player) {
    auto &input_data = player->GetInputData();
    if (input_data.key_down[GLFW_KEY_SPACE] && serve_count_down_ == 0) {
      SetTexture(0);
      battle_game::DrawModel(serve_turret_model_index);
    }
  }

  battle_game::SetRotation(rotation_);
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/volleytank.png");
  battle_game::DrawModel(0);

  battle_game::SetRotation(turret_rotation_);
  if (player) {
    auto &input_data = player->GetInputData();
    if (input_data.key_down[GLFW_KEY_SPACE] && spike_count_down_ == 0) {
      SetTexture(0);
      battle_game::DrawModel(spike_turret_model_index);
    }
  }

  RenderSpikeCDBar();
  RenderServeCDBar();
}

const char *VolleyTank::UnitName() const {
  return "VolleyTank";
}

const char *VolleyTank::Author() const {
  return "Gingko";
}
}  // namespace battle_game::unit
