__kernel void matrixAdd(__global const int* A,
                            __global const int* B,
                            __global int* C,
                            const int rows,
                            const int cols)
    {
        int gidX = get_global_id(0);
        int gidY = get_global_id(1);
        int index = gidY * cols + gidX;
        C[index] = A[index] + B[index];
    }
