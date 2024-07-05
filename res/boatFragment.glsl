#version 410

out vec4 out_color;
in vec2 uv;
in vec3 vNormal_vs;   // Normale du sommet transformée dans l'espace de vue
uniform sampler2D my_texture;
uniform vec3 light_direction; // Direction de la lumière (dans l'espace monde)

void main()
{
    // Normaliser la normale
    vec3 normal = normalize(vNormal_vs);

    // Calculer la direction de la lumière dans l'espace de vue
    vec3 light_dir_vs = normalize(mat3(inverse(mat3(1.0))) * light_direction);
    
    // Calculer l'éclairage diffus
    float diffuse = max(dot(-light_dir_vs, normal), 1);
    
    // Couleur de base (albedo)
    vec3 albedo = vec3(1.0, 1.0, 1.0);
    
    // Calculer la couleur finale
    vec3 color = albedo * diffuse;

    vec4 texture_color = texture(my_texture, uv);
    out_color = texture_color * vec4(color, 1);
}