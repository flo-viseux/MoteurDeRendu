#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE); // On peut configurer l'équation qui mélange deux couleurs, comme pour faire différents blend mode dans Photoshop. Cette équation-ci donne le blending "normal" entre pixels transparents.

    auto camera = gl::Camera{};
    gl::set_events_callbacks({camera.events_callbacks()});
    
    auto const rectangle_mesh = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position3D{0}},
        .data   = {
            -1.f, -1.f, -1.f, // 0
            +1.f, -1.f, -1.f,// 1
            +1.f, +1.f, -1.f,// 2
            -1.f, +1.f, -1.f, // 3

            -1.f, -1.f, +1.f, // 4
            +1.f, -1.f, +1.f,// 5
            +1.f, +1.f, +1.f,// 6
            -1.f, +1.f, +1.f  // 7
        },
    }},
    .index_buffer   = {
        // bottom
        0, 1, 2,
        0, 2, 3,

        // top
        4, 5, 6,
        4, 6, 7,

        // front
        0, 1, 5,
        0, 5, 4,

        // back
        3, 2, 6,
        3, 6, 7,

        // left
        1, 2, 6,
        1, 6, 5,

        // right
        0, 3, 7,
        0, 7, 2
    },
    }};

    auto const cameraShader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/cameraVertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/cameraFragment.glsl"},
    }};

    while (gl::window_is_open())
    {
        // Rendu à chaque frame
        glClearColor(0.5f, 0.5f, 0.7f, 1.f); // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        glClear(GL_COLOR_BUFFER_BIT); // Exécute concrètement l'action d'appliquer sur tout l'écran la couleur choisie au-dessus

        rectangle_mesh.draw();

        glm::mat4 const view_matrix = camera.view_matrix();

        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);

        cameraShader.bind();

        cameraShader.set_uniform("view_projection_matrix", projection_matrix * view_matrix);
    }
}