#include "Print.h"

#include <stdio.h>

namespace Stella
{
namespace Debug
{

void print_mat4(const glm::mat4 & mat)
{
    printf("[\n");
    printf("  [ %f, %f, %f, %f ]\n", mat[0].x, mat[0].y, mat[0].z, mat[0].w);
    printf("  [ %f, %f, %f, %f ]\n", mat[1].x, mat[1].y, mat[1].z, mat[1].w);
    printf("  [ %f, %f, %f, %f ]\n", mat[2].x, mat[2].y, mat[2].z, mat[2].w);
    printf("  [ %f, %f, %f, %f ]\n", mat[3].x, mat[3].y, mat[3].z, mat[3].w);
    printf("]\n");
}
    
} // namespace Debug    
} // namespace Stella
