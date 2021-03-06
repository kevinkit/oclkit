## oclkit, plain and stupid OpenCL helper

_oclkit_ is a small set of C functions, to avoid writing the same OpenCL boiler
plate over and over again, yet keeping full control over executing OpenCL.


### API

* `ocl_new`: Create a new OpenCL platform instance with all devices initialized.
  Each device is assigned its own queue. The returned pointer is opaque and
  should only be modified with the following functions.
* `ocl_free`: Releases all resources except for programs created with
  `ocl_get_program`.
* `ocl_get_context`: Return the `cl_context` pointer
* `ocl_get_num_devices`: Return the number of detected devices.
* `ocl_get_devices`: Return array of devices.
* `ocl_get_cmd_queues`: Return array of command queues.
* `ocl_get_program`: Create program from source file. Release with
  `clReleaseProgram`.
* `ocl_strerr`: Return static C string of an OpenCL error code.
* `OCL_CHECK_ERROR`: Check error code and print a message on `stderr` if error
  code is not `CL_SUCCESS`.


### Running the samples

Run `make` in the top-level directory and change into `build/examples`.


### License

The code is licensed under GPL v3.
