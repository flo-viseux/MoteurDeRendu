#version 330

in vec3 vPosition_vs; // Position du sommet transformée dans l'espace de vue
in vec3 vNormal_vs;   // Normale du sommet transformée dans l'espace de vue
in vec2 vTexCoords;   // Coordonnées de texture du sommet

out vec4 fFragColor;

uniform vec3 light_direction; // Direction de la lumière (dans l'espace monde)

void main() {
    // Normaliser la normale
    vec3 normal = normalize(vNormal_vs);
    
    // Calculer la direction de la lumière dans l'espace de vue
    vec3 light_dir_vs = normalize(mat3(inverse(mat3(1.0))) * light_direction);
    
    // Calculer l'éclairage diffus
    float diffuse = max(dot(-light_dir_vs, normal), 0.0);
    
    // Couleur de base (albedo)
    vec3 albedo = vec3(1.0, 1.0, 1.0);
    
    // Calculer la couleur finale
    vec3 color = albedo * diffuse;
    
    // Afficher les normales comme couleurs (décommentez pour tester)
    // color = normal * 0.5 + 0.5;
    
    fFragColor = vec4(color, 1.0);
}