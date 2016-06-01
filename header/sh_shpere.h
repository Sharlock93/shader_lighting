#define GLEW_STATIC
#include <GL\glew.h>
#include <shar.h>

vec4 unit( const vec4& p );

class sh_shpere
{
public:
    sh_shpere();
    ~sh_shpere();
    void render();
    int _index;
    int _size;

    vec4 *_points;
    GLuint _vbo;
};
