#pragma once
#include <cstdint>

#include "render-3d.hpp"

struct ModelShaderParams
{
    blit::Vec3 light_direction;
};

void model_lit_shader(const uint8_t *in, Render3D::VertexOutData *out, const Render3D &r);

class Model final
{
public:
    struct Vertex
    {
        uint32_t x, y, z;
        uint8_t r, g, b, tex_index;
        int16_t nx, ny, nz;
        uint16_t u, v;
        uint16_t pad;
    };

    struct Mesh
    {
        uint32_t num_vertices;
        const Vertex *vertices;
    };

    Model(const uint8_t *asset_data);
    ~Model();

    void draw(Render3D &r3d);
    void draw_mesh(uint32_t mesh, Render3D &r3d);

    uint32_t get_num_meshes() const;

private:
    uint32_t num_meshes;
    Mesh *meshes;
};