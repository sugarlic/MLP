__kernel void SumVectors(__global double* A,
                            __global const double* B)
{
        int i = get_global_id(0);
        A[i] = A[i] + B[i];
}

