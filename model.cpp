#include "model.hpp"

#include "types/vec3.hpp"

#include "vec4.hpp"

void model_lit_shader(const uint8_t *in, Render3D::VertexOutData *out, const Render3D &r)
{
    auto vertex = reinterpret_cast<const Model::Vertex *>(in);
    // get normal
    // should be / 32767, but we're going to normalise anyway
    FixedVec4 nor(Fixed32<>(vertex->nx) / 32768, Fixed32<>(vertex->ny) / 32768, Fixed32<>(vertex->nz) / 32768, Fixed32<>(0.0f));
    nor = r.get_model_view() * nor;

    // normalise
    float len = std::sqrt(float(nor.x * nor.x + nor.y * nor.y + nor.z * nor.z));
    auto inv_len = Fixed32<>(1.0f / len);

    auto nx = nor.x * inv_len;
    auto ny = nor.y * inv_len;
    auto nz = nor.z * inv_len;

    // lighting
    auto params = (ModelShaderParams *)r.get_shader_params();
    auto &light = params->light_direction;

    // everything here is in -1 - 1
    // trade a bit of precision for avoiding the 64-bit muls
    // kinda ugly with all the too/from raw...
    auto dot = Fixed32<15>::from_raw(Fixed32<15>(light.x).raw() * Fixed32<15>(nx).raw() >> 15)
             + Fixed32<15>::from_raw(Fixed32<15>(light.y).raw() * Fixed32<15>(ny).raw() >> 15)
             + Fixed32<15>::from_raw(Fixed32<15>(light.z).raw() * Fixed32<15>(nz).raw() >> 15);

    if(dot.raw() < 0)
        dot = 0;

    // ambient
    dot += Fixed32<15>(0.3f);

    if(dot.raw() > 1 << 15)
        dot = Fixed32<15>(1);

    out->r = int32_t(dot * vertex->r);
    out->g = int32_t(dot * vertex->g);
    out->b = int32_t(dot * vertex->b);

    out->tex_index = vertex->tex_index;

    if(vertex->tex_index)
    {
        out->u = Fixed16<12>::from_raw(vertex->u >> 2);
        out->v = Fixed16<12>::from_raw(vertex->v >> 2);
    }
}

Model::Model(const uint8_t *asset_data)
{
    // TODO: check magic header

    auto ptr32 = reinterpret_cast<const uint32_t *>(asset_data + 4);

    num_meshes = *ptr32++;
    meshes = new Mesh[num_meshes];

    for(uint32_t i = 0; i < num_meshes; i++)
    {
        meshes[i].num_vertices = *ptr32++;
        meshes[i].vertices = reinterpret_cast<const Vertex *>(ptr32);

        ptr32 += meshes[i].num_vertices * sizeof(Vertex) / sizeof(uint32_t);
    }
}

Model::~Model()
{
    delete[] meshes;
}

void Model::draw(Render3D &r3d)
{
    r3d.set_vertex_stride(sizeof(Vertex));

    for(uint32_t i = 0; i < num_meshes; i++)
        r3d.draw(meshes[i].num_vertices, reinterpret_cast<const uint8_t *>(meshes[i].vertices));
}

void Model::draw_mesh(uint32_t mesh, Render3D &r3d)
{
    if(mesh >= num_meshes)
        return;

    r3d.set_vertex_stride(sizeof(Vertex));
    r3d.draw(meshes[mesh].num_vertices, reinterpret_cast<const uint8_t *>(meshes[mesh].vertices));
}

uint32_t Model::get_num_meshes() const
{
    return num_meshes;
}
