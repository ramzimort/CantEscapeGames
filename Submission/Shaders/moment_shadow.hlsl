

float det3(float3 a, float3 b, float3 c)
{
    return a.x * (b.y * c.z - b.z * c.y) + a.y 
        * (b.z * c.x - b.x * c.z) + a.z * (b.x * c.y - b.y * c.x);
}


float CalculateMSM(float4 moment, float fragmentZ)
{
    const float alpha_value = 0.0001f;

    float4 b_prime = moment.xyzw * (1.f - alpha_value) 
        + alpha_value * float4(0.5f, 0.5f, 0.5f, 0.5f);


    float3 a_vector = float3(1.f, b_prime.x, b_prime.y);
    float3 b_vector = float3(b_prime.xyz);
    float3 c_vector = float3(b_prime.y, b_prime.z, b_prime.w);

    float d = det3(a_vector,  b_vector, c_vector);


    float fragmentZPow2 = pow(fragmentZ, 2.f);

    float3 z_vector = float3(1.f, fragmentZ, fragmentZPow2);

    float c1 = det3(z_vector, b_vector, c_vector) / d;
    float c2 = det3(a_vector, z_vector, c_vector) / d;
    float c3 = det3(a_vector, b_vector, z_vector) / d;
   

    float a = c3;
    float b = c2;
    float c = c1;

    float discriminant = sqrt((b * b) - (4.f * a * c));
    float c1_2 = 2.f * a;

    float positive_z = (-b + discriminant) / c1_2;
    float minus_z = (-b - discriminant) / c1_2;
     
    float min_z = min(positive_z, minus_z);
    float max_z = max(positive_z, minus_z);

    float shadow_factor = 0.f;

    if(fragmentZ <= min_z )
    {
        return 0.f;
    }
    else if(fragmentZ <= max_z)
    {
        float numerator = (fragmentZ * max_z - b_prime.x * (fragmentZ + max_z) + b_prime.y);
        float denominator = (max_z - min_z) * (fragmentZ - min_z);
        shadow_factor = numerator / denominator;
    }
    else
    {
        float numerator = min_z * max_z - b_prime.x * (min_z + max_z) + b_prime.y;
        float denominator = (fragmentZ - min_z) * (fragmentZ - max_z);
        shadow_factor = 1.f - (numerator / denominator);
    }

    return saturate(shadow_factor);
}