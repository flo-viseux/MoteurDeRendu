#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE); // On peut configurer l'équation qui mélange deux couleurs, comme pour faire différents blend mode dans Photoshop. Cette équation-ci donne le blending "normal" entre pixels transparents.

    auto camera = gl::Camera{};
    gl::set_events_callbacks({camera.events_callbacks()});
    
    auto const rectangle_mesh = gl::Mesh{{
    .vertex_buffers = {{
        .layout = {gl::VertexAttribute::Position2D{0}, gl::VertexAttribute::UV{1}},
        .data   = {
            -2.f, -2.f, 0.f, 0.f,// -1.f, // 0
            +2.f, -2.f, 1.f, 0.f,// -1.f,// 1
            +2.f, +2.f, 1.f, 1.f,// -1.f,// 2
            -2.f, +2.f, 0.f, 1.f,// -1.f, // 3

            //-1.f, -1.f, 1.f, // 0
            //+1.f, -1.f, 1.f,// 1
            //+1.f, +1.f, 1.f,// 2
            //-1.f, +1.f, 1.f, // 3
        },
    }},
    .index_buffer   = {
        // bottom
        0, 1, 2,
        0, 2, 3,

        // top
        //4, 5, 6,
        //4, 6, 7,

        // front
        //0, 1, 5,
        //0, 5, 4,

        // back
        //3, 2, 6,
        //3, 6, 7,

        // left
        //1, 2, 6,
        //1, 6, 5,

        // right
        //0, 4, 7,
        //0, 7, 3,
    },
    }};

    auto const texture = gl::Texture{
    gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
        .path           = "res/texture.png",
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

    auto const cameraShader = gl::Shader{{
    .vertex   = gl::ShaderSource::File{"res/cameraVertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/cameraFragment.glsl"},
    }};

    while (gl::window_is_open())
    {
        // Rendu à chaque frame
        glClearColor(0.5f, 0.5f, 0.7f, 1.f); // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        glClear(GL_COLOR_BUFFER_BIT); // Exécute concrètement l'action d'appliquer sur tout l'écran la couleur choisie au-dessus
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Vient remplacer glClear(GL_COLOR_BUFFER_BIT);

        rectangle_mesh.draw();

        glm::mat4 const view_matrix = camera.view_matrix();

        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);

        cameraShader.bind();

        cameraShader.set_uniform("view_projection_matrix", projection_matrix * view_matrix);
        cameraShader.set_uniform("my_texture", texture);
    }
}