#include <stdio.h>
#include <glib.h>
#include "ocl.h"

int
main (void)
{
    OclPlatform *ocl;
    cl_mem mem;
    cl_program program;
    cl_kernel kernel;
    cl_int errcode;
    cl_event event;
    size_t n_elements;
    GTimer *timer;
    cl_command_queue *cmd_queues;

    timer = g_timer_new ();
    ocl = ocl_new_with_queues (0, CL_DEVICE_TYPE_ALL, 0);
    g_timer_stop (timer);
    g_print ("Creating context with devices and queues: %fs\n", g_timer_elapsed (timer, NULL));

    if (ocl == NULL)
        return 1;

    g_timer_start (timer);
    program = ocl_create_program_from_file (ocl, "test.cl", NULL, &errcode);
    OCL_CHECK_ERROR (errcode);
    g_timer_stop (timer);
    g_print ("Build program: %fs\n", g_timer_elapsed (timer, NULL));

    cmd_queues = ocl_get_cmd_queues (ocl);

    g_timer_start (timer);
    kernel = clCreateKernel (program, "fill_ones", &errcode);
    OCL_CHECK_ERROR (errcode);
    g_timer_stop (timer);
    g_print ("Create kernel: %fs\n", g_timer_elapsed (timer, NULL));

    g_timer_start (timer);
    n_elements = 1024 * 1024;
    mem = clCreateBuffer (ocl_get_context (ocl), CL_MEM_READ_WRITE,
                          n_elements * sizeof (float),
                          NULL, &errcode);
    g_timer_stop (timer);
    g_print ("Create buffer: %fs\n", g_timer_elapsed (timer, NULL));

    OCL_CHECK_ERROR (clSetKernelArg (kernel, 0, sizeof (cl_mem), &mem));
    OCL_CHECK_ERROR (clEnqueueNDRangeKernel (cmd_queues[0], kernel,
                                             1, NULL, &n_elements, NULL,
                                             0, NULL, &event));
                                             
    OCL_CHECK_ERROR (clWaitForEvents (1, &event));

    g_timer_start (timer);
    OCL_CHECK_ERROR (clReleaseEvent (event));
    OCL_CHECK_ERROR (clReleaseMemObject (mem));
    OCL_CHECK_ERROR (clReleaseKernel (kernel));
    OCL_CHECK_ERROR (clReleaseProgram (program));
    ocl_free (ocl);
    g_timer_stop (timer);
    g_print ("Cleanup: %fs\n", g_timer_elapsed (timer, NULL));

    return 0;
}
