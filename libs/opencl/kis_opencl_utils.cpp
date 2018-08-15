/*
 *  Copyright (c) 2018 Andrew Savonichev <andrew.savonichev@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "kis_opencl_utils.h"

#include <kis_debug.h>
#include <QVector>

namespace ocl {

const char* errorStr(cl_int error) {
    switch (error) {
    case 0:   return "CL_SUCCESS";
    case -1:  return "CL_DEVICE_NOT_FOUND";
    case -2:  return "CL_DEVICE_NOT_AVAILABLE";
    case -3:  return "CL_COMPILER_NOT_AVAILABLE";
    case -4:  return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5:  return "CL_OUT_OF_RESOURCES";
    case -6:  return "CL_OUT_OF_HOST_MEMORY";
    case -7:  return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8:  return "CL_MEM_COPY_OVERLAP";
    case -9:  return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";
    case -69: return "CL_INVALID_PIPE_SIZE";
    case -70: return "CL_INVALID_DEVICE_QUEUE";
    }

    return "CL_UNKNOWN_ERROR";
}

void printKernelNames(cl_program program)
{
    cl_int error = CL_SUCCESS;

    QVector<cl_kernel> kernels;
    cl_uint numKernels = 0;
    error = clCreateKernelsInProgram(program, 0, NULL, &numKernels);
    if (error != CL_SUCCESS) {
        dbgOpenCL << "clCreateKernelsInProgram failed:" << errorStr(error);
        return;
    }

    kernels.resize(numKernels);
    error = clCreateKernelsInProgram(program, numKernels, &kernels[0], NULL);
    if (error != CL_SUCCESS) {
        dbgOpenCL << "clCreateKernelsInProgram failed:" << errorStr(error);
        return;
    }

    for (cl_kernel kernel : kernels) {
        QVector<char> kernelName;
        size_t kernelNameSize = 0;
        error = clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME, 0, NULL, &kernelNameSize);
        if (error != CL_SUCCESS) {
            dbgOpenCL << "clGetKernelInfo failed:" << errorStr(error);
            return;
        }

        kernelName.resize(kernelNameSize);

        error = clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME, kernelNameSize, &kernelName[0], NULL);
        if (error != CL_SUCCESS) {
            dbgOpenCL << "clGetKernelInfo failed:" << errorStr(error);
            return;
        }

        dbgOpenCL << " " << &kernelName[0];
    }
}

} // namespace ocl
