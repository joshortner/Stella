struct Ray
{
    float3 m_origin;
    float3 m_dir;
    float m_maxDist;
};

float3 project(float3 point, float4 mat[4])
{
    float3 result;

    result.x = (point.x * mat[0].x) + (point.y * mat[0].y) + (point.z * mat[0].z) + (1.0f * mat[0].w);
    result.y = (point.x * mat[1].x) + (point.y * mat[1].y) + (point.z * mat[1].z) + (1.0f * mat[1].w);
    result.z = (point.x * mat[2].x) + (point.y * mat[2].y) + (point.z * mat[2].z) + (1.0f * mat[2].w);
    /* result.w = (point.x * mat[3].x) + (point.y * mat[3].y) + (point.y * mat[3].y) + (point.y * mat[3].y); */

    return result;
}

__kernel void render_kernel(__global float3 * output, int width, int height)
{
    float4 vp_mat[4];
    vp_mat[0] = (float4)(0.075000, 0.000000, 0.000000, 0.000000);
    vp_mat[1] = (float4)(0.000000, 0.100000, 0.000000, 0.000000);
    vp_mat[2] = (float4)(0.000000, 0.000000, -0.000200, 0.000000);
    vp_mat[3] = (float4)(-0.000000, -0.000000, -0.999600, 1.000000);

    const int work_item_id = get_global_id(0); 
    int x = work_item_id % width; 
    int y = work_item_id / width; 
    float fx = (float)x / (float)width;
    float fy = (float)y / (float)height;
    float cs_x = (fx * 2.0f) - 1.0f; /* Clip space [-1, 1] */
    float cs_y = (fy * 2.0f) - 1.0f; /* Clip space [-1, 1] */
    cs_y *= -1.0f;

    struct Ray ray;
    ray.m_origin = (float3)(0.0f, 0.0f, 0.0f);
    ray.m_dir = (float3)(0.5f, 0.5f, 0.0f);
    ray.m_maxDist = 0.05f;

    float3 point = (float3)(cs_x, cs_y, 0);
    point = project(point, vp_mat);

    float d = dot(ray.m_dir, normalize(point));

    output[work_item_id] = (float3)(0, 0, 0); 

    if (distance(point, ray.m_origin) <= ray.m_maxDist && d > 0.5) {
        output[work_item_id] = (float3)(fx, fy, 0);    
    }

    /*output[work_item_id] = (float3)(fx, fy, 0);*/
}