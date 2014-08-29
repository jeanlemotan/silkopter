#pragma once

extern void render_arrow(math::vec3f const& start,
                        math::vec3f const& end,
                        q::video::Material const& material,
                        uint32_t color,
                        q::draw::Painter& painter);

extern void render_axes(q::draw::Painter& painter, float length);

