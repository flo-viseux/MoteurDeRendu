#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"
#include "tiny_obj_loader.h"
#include <iostream>

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut
    glEnable(GL_DEPTH_TEST);

    auto camera = gl::Camera{};

    gl::set_events_callbacks({
    camera.events_callbacks(),
    {
        .on_mouse_pressed = [&](gl::MousePressedEvent const& e) 
            {
            //std::cout << "Mouse pressed at " << e.position.x << " " << e.position.y << '\n';
            },
    },
    });

    auto const fourareenTexture = gl::Texture
    {
            gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
                .path           = "res/meshes/fourareen2K_albedo.jpg",
                .flip_y         = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
                .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
            },
            gl::TextureOptions{
                .minification_filter  = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
                .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
                .wrap_x               = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
                .wrap_y               = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
            }
    };

    std::filesystem::path inputfile = gl::make_absolute_path("res/meshes/fourareen.obj");
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile.string(), reader_config)) {
    if (!reader.Error().empty()) {
        std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
    }

    if (!reader.Warning().empty()) {
    std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<float> buffer;

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) 
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) 
            {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                buffer.push_back(vx);
                buffer.push_back(vy);
                buffer.push_back(vz);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                    
                    buffer.push_back(nx);
                    buffer.push_back(ny);
                    buffer.push_back(nz);
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];

                    buffer.push_back(tx);
                    buffer.push_back(ty);
                }
            }

            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }

    auto const boat_Mesh = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::Position3D{1} ,gl::VertexAttribute::Position2D{2}},
        .data   =  { 
            buffer 
            },
        }}
    }};

    auto const cameraShader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/cameraVertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/cameraFragment.glsl"},
    }};

    auto const lightShader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/lightVertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/lightFragment.glsl"},
    }};

    auto const boatShader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/boatVertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/boatFragment.glsl"},
    }};

    while (gl::window_is_open())
    {
        glClearColor(.5f, .5f, .5f, 1.f); // Dessine du rouge, non pas à l'écran, mais sur notre render target
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 const view_matrix = camera.view_matrix();

        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);


        cameraShader.bind();
        cameraShader.set_uniform("view_projection_matrix", projection_matrix * view_matrix);


        lightShader.bind();
        // Mettre à jour les uniforms du shader ici
        glm::vec3 lightDir = glm::normalize(glm::vec3(0.2f, 0.3f, -1.0f));
        lightShader.set_uniform("light_direction", lightDir);


        boatShader.bind();
        boatShader.set_uniform("view_projection_matrix", projection_matrix * view_matrix);
        boatShader.set_uniform("my_texture", fourareenTexture);

        boat_Mesh.draw();
    }
}