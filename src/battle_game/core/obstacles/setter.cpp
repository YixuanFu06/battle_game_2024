#include "battle_game/core/obstacles/setter.h"
#include <glm/fwd.hpp>

namespace battle_game::obstacle {

Setter::Setter(GameCore *game_core,
             uint32_t id,
             std::vector<glm::vec2> vertices,
             float rotation,
             glm::vec2 scale)
    : Obstacle(game_core, id, vertices[0], rotation) {
    if (!~setter_model_index) {
        auto mgr = AssetsManager::GetInstance();
        std::vector<ObjectVertex> setter_vertices;
        for (auto &v : vertices) {
            vertices_.push_back(v - position_);
            setter_vertices.push_back({v - position_, texture_, color_});
        }
        std::vector<uint32_t> setter_indices;
        for (int i = 0; i < vertices_.size(); ++i) {
            if (i != 0 && i != vertices_.size() - 1) {
                setter_indices.push_back(0);
                setter_indices.push_back(i);
                setter_indices.push_back(i + 1);
            }

            auto v1 = vertices_[i] - vertices_[(i - 1) % vertices_.size()];
            auto v2 = vertices_[(i + 1) % vertices_.size()] - vertices_[i];
            auto cross = v1.x * v2.y - v1.y * v2.x;
            if (cross > 0) {
                inner_side_.push_back(true);
            } else {
                inner_side_.push_back(false);
            }
        }
        setter_model_index = mgr->RegisterModel(setter_vertices, setter_indices);
    }
}

bool Setter::IsBlocked(glm::vec2 p) const {
  p = WorldToLocal(p);
  for (int i = 0; i < inner_side_.size(); ++i) {
    auto v1 = vertices_[(i + 1) % inner_side_.size()] - vertices_[i];
    auto v2 = p - vertices_[i];
    auto cross = v1.x * v2.y - v1.y * v2.x;
    if (cross < 0 && inner_side_[(i + 1) % inner_side_.size()]) {
      return false;
    }
    if (cross > 0 && !inner_side_[(i + 1) % inner_side_.size()]) {
      return false;
    }
  }
  return true;
}

std::pair<glm::vec2, glm::vec2> Setter::GetSurfaceNormal(glm::vec2 origin,
                                                        glm::vec2 terminus) {
    auto local_origin = WorldToLocal(origin);
    auto local_terminus = WorldToLocal(terminus);
    glm::vec2 local_position = {0.0f, 0.0f};
    glm::vec2 local_normal = {0.0f, 0.0f};
    float min_distance = 1e9;

    for (int i = 0; i < vertices_.size(); ++i) {
        auto intersection = GetIntersection(local_origin, local_terminus, vertices_[i], vertices_[(i + 1) % vertices_.size()]);
        if (intersection == glm::vec2{0, 0}) {
            continue;
        }
        auto distance = glm::length(intersection - local_origin);
        if (distance < min_distance) {
            min_distance = distance;
            local_position = intersection;
            local_normal = glm::normalize(vertices_[(i + 1) % vertices_.size()] - vertices_[i]);
            // if inner_side_[(i + 1) % inner_side_.size()] = true, rotate 90 degree clockwise; else rotate 90 degree anti-clockwise
            local_normal = glm::vec2{local_normal.y, -local_normal.x};
            if (!inner_side_[(i + 1) % inner_side_.size()]) {
                local_normal = -local_normal;
            }
        }
    }

    if (min_distance == 1e9) {
        return {glm::vec2{0, 0}, glm::vec2{0, 0}};
    } else {
        std::pair<glm::vec2, glm::vec2> result;
        result.first = LocalToWorld(local_position);
        result.second = LocalToWorld(local_normal) - LocalToWorld({0, 0});
        return result;
    }
}

glm::vec2 Setter::GetIntersection(glm::vec2 origin1, glm::vec2 terminus1, glm::vec2 origin2, glm::vec2 terminus2) const {
    auto v1 = terminus1 - origin1;
    auto v2 = terminus2 - origin2;
    auto cross = v1.x * v2.y - v1.y * v2.x;
    if (cross == 0) {
        return {0, 0};
    }
    auto t = ((origin2.x - origin1.x) * v2.y - (origin2.y - origin1.y) * v2.x) / cross;
    auto u = ((origin2.x - origin1.x) * v1.y - (origin2.y - origin1.y) * v1.x) / cross;
    if (t < 0 || t > 1 || u < 0 || u > 1) {
        return {0, 0};
    }
    return origin1 + t * v1;
}

void Setter::Render() {
  battle_game::SetColor(color_);
  battle_game::SetTexture(0);
  battle_game::SetTransformation(position_, rotation_, scale_);
  battle_game::DrawModel(setter_model_index);
}
}  // namespace battle_game::obstacle
