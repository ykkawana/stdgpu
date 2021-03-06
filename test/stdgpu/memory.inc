/*
 *  Copyright 2019 Patrick Stotko
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef STDGPU_MEMORY_TEST_CLASS
    #error "Class name for unit test not specified!"
#endif

#include <gtest/gtest.h>

#include <cmath>
#include <thrust/equal.h>
#include <thrust/execution_policy.h>
#include <thrust/logical.h>

#include <test_utils.h>
#include <stdgpu/iterator.h>
#include <stdgpu/memory.h>
#include <stdgpu/platform.h>


class STDGPU_MEMORY_TEST_CLASS : public ::testing::Test
{
    protected:
        // Called before each test
        virtual void SetUp()
        {

        }

        // Called after each test
        virtual void TearDown()
        {

        }

};


struct equal_to_number
{
    int number;

    equal_to_number(const int number_int)
        : number(number_int)
    {

    }

    STDGPU_HOST_DEVICE bool
    operator()(const int value)
    {
        return (value == number);
    }
};


TEST_F(STDGPU_MEMORY_TEST_CLASS, dynamic_memory_type_device)
{
    int* array_device = createDeviceArray<int>(42);

    EXPECT_EQ(stdgpu::get_dynamic_memory_type(array_device), stdgpu::dynamic_memory_type::device);

    destroyDeviceArray<int>(array_device);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, dynamic_memory_type_host)
{
    int* array_host = createHostArray<int>(42);

    EXPECT_EQ(stdgpu::get_dynamic_memory_type(array_host), stdgpu::dynamic_memory_type::host);

    destroyHostArray<int>(array_host);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, dynamic_memory_type_managed_on_device)
{
    int* array_managed = createManagedArray<int>(42, 0, Initialization::DEVICE);

    EXPECT_EQ(stdgpu::get_dynamic_memory_type(array_managed), stdgpu::dynamic_memory_type::managed);

    destroyManagedArray<int>(array_managed);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, dynamic_memory_type_managed_on_host)
{
    int* array_managed = createManagedArray<int>(42, 0, Initialization::HOST);

    EXPECT_EQ(stdgpu::get_dynamic_memory_type(array_managed), stdgpu::dynamic_memory_type::managed);

    destroyManagedArray<int>(array_managed);
}



TEST_F(STDGPU_MEMORY_TEST_CLASS, dynamic_memory_type_invalid_pointer)
{
    int* array_invalid  = reinterpret_cast<int*>(42);

    EXPECT_EQ(stdgpu::get_dynamic_memory_type(array_invalid), stdgpu::dynamic_memory_type::invalid);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, dynamic_memory_type_variable_pointer)
{
    int non_dynamic_array = 42;

    EXPECT_EQ(stdgpu::get_dynamic_memory_type(&non_dynamic_array), stdgpu::dynamic_memory_type::invalid);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, dynamic_memory_type_nullptr)
{
    EXPECT_EQ(stdgpu::get_dynamic_memory_type<int*>(nullptr), stdgpu::dynamic_memory_type::invalid);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, size_bytes_device)
{
    int* array_device = createDeviceArray<int>(42);

    EXPECT_EQ(stdgpu::size_bytes(array_device), 42 * sizeof(int));

    destroyDeviceArray<int>(array_device);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, size_bytes_host)
{
    int* array_host = createHostArray<int>(42);

    EXPECT_EQ(stdgpu::size_bytes(array_host), 42 * sizeof(int));

    destroyHostArray<int>(array_host);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, size_bytes_managed_device)
{
    int* array_managed = createManagedArray<int>(42, 0, Initialization::DEVICE);

    EXPECT_EQ(stdgpu::size_bytes(array_managed), 42 * sizeof(int));

    destroyManagedArray<int>(array_managed);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, size_bytes_manged_host)
{
    int* array_managed = createManagedArray<int>(42, 0, Initialization::HOST);

    EXPECT_EQ(stdgpu::size_bytes(array_managed), 42 * sizeof(int));

    destroyManagedArray<int>(array_managed);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, size_bytes_nullptr)
{
    EXPECT_EQ(stdgpu::size_bytes<int*>(nullptr), static_cast<stdgpu::index64_t>(0));
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, size_bytes_device_shifted)
{
    int* array_device = createDeviceArray<int>(42);

    EXPECT_EQ(stdgpu::size_bytes(array_device + 24), static_cast<stdgpu::index64_t>(0));

    destroyDeviceArray<int>(array_device);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, size_bytes_host_shifted)
{
    int* array_host = createHostArray<int>(42);

    EXPECT_EQ(stdgpu::size_bytes(array_host + 24), static_cast<stdgpu::index64_t>(0));

    destroyHostArray<int>(array_host);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, size_bytes_managed_device_shifted)
{
    int* array_managed = createManagedArray<int>(42, 0, Initialization::DEVICE);

    EXPECT_EQ(stdgpu::size_bytes(array_managed + 24), static_cast<stdgpu::index64_t>(0));

    destroyManagedArray<int>(array_managed);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, size_bytes_managed_host_shifted)
{
    int* array_managed = createManagedArray<int>(42, 0, Initialization::HOST);

    EXPECT_EQ(stdgpu::size_bytes(array_managed + 24), static_cast<stdgpu::index64_t>(0));

    destroyManagedArray<int>(array_managed);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, createDestroyDeviceArray_empty)
{
    int* array_device = createDeviceArray<int>(0, 0);

    EXPECT_EQ(array_device, nullptr);

    destroyDeviceArray<int>(array_device);

    EXPECT_EQ(array_device, nullptr);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, createDestroyHostArray_empty)
{
    int* array_host = createHostArray<int>(0, 0);

    EXPECT_EQ(array_host, nullptr);

    destroyHostArray<int>(array_host);

    EXPECT_EQ(array_host, nullptr);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, createDestroyManagedArray_empty)
{
    int* array_managed_device = createManagedArray<int>(0, 0, Initialization::DEVICE);
    int* array_managed_host = createManagedArray<int>(0, 0, Initialization::HOST);

    EXPECT_EQ(array_managed_device, nullptr);
    EXPECT_EQ(array_managed_host, nullptr);

    destroyManagedArray<int>(array_managed_device);
    destroyManagedArray<int>(array_managed_host);

    EXPECT_EQ(array_managed_device, nullptr);
    EXPECT_EQ(array_managed_host, nullptr);
}


namespace
{
    void createAndDestroyDeviceFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array_device = createDeviceArray<int>(size, default_value);

            #if STDGPU_DEVICE_COMPILER == STDGPU_DEVICE_COMPILER_NVCC
                EXPECT_TRUE( thrust::all_of(stdgpu::device_cbegin(array_device), stdgpu::device_cend(array_device),
                                            equal_to_number(default_value)) );
            #endif

            destroyDeviceArray<int>(array_device);

            EXPECT_EQ(array_device, nullptr);
        }
    }

    void createAndDestroyHostFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array_host = createHostArray<int>(size, default_value);

            EXPECT_TRUE( thrust::all_of(stdgpu::host_cbegin(array_host), stdgpu::host_cend(array_host),
                                        equal_to_number(default_value)) );


            destroyHostArray<int>(array_host);

            EXPECT_EQ(array_host, nullptr);
        }
    }

    void createAndDestroyManagedFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array_managed_device = createManagedArray<int>(size, default_value, Initialization::DEVICE);
            int* array_managed_host = createManagedArray<int>(size, default_value, Initialization::HOST);

            #if STDGPU_DEVICE_COMPILER == STDGPU_DEVICE_COMPILER_NVCC
                EXPECT_TRUE( thrust::all_of(stdgpu::device_cbegin(array_managed_device), stdgpu::device_cend(array_managed_device),
                                            equal_to_number(default_value)) );
            #endif

            EXPECT_TRUE( thrust::all_of(stdgpu::host_cbegin(array_managed_host), stdgpu::host_cend(array_managed_host),
                                        equal_to_number(default_value)) );

            destroyManagedArray<int>(array_managed_device);
            destroyManagedArray<int>(array_managed_host);

            EXPECT_EQ(array_managed_device, nullptr);
            EXPECT_EQ(array_managed_host, nullptr);
        }
    }
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, createDestroyDeviceArray)
{
    createAndDestroyDeviceFunction(1);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, createDestroyDeviceArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&createAndDestroyDeviceFunction,
                                           iterations_per_thread);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, createDestroyHostArray)
{
    createAndDestroyHostFunction(1);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, createDestroyHostArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&createAndDestroyHostFunction,
                                           iterations_per_thread);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, createDestroyManagedArray)
{
    createAndDestroyManagedFunction(1);
}


/*
TEST_F(STDGPU_MEMORY_TEST_CLASS, createDestroyManagedArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&createAndDestroyManagedFunction,
                                           iterations_per_thread);
}
*/


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateHost2HostArray_empty)
{
    int* array_host = createHostArray<int>(0, 0);

    EXPECT_EQ(array_host, nullptr);

    int* array_host_copy = copyCreateHost2HostArray<int>(array_host, 0);

    EXPECT_EQ(array_host_copy, nullptr);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateDevice2HostArray_empty)
{
    int* array_device = createDeviceArray<int>(0, 0);

    EXPECT_EQ(array_device, nullptr);

    int* array_host_copy = copyCreateDevice2HostArray<int>(array_device, 0);

    EXPECT_EQ(array_host_copy, nullptr);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateHost2DeviceArray_empty)
{
    int* array_host = createHostArray<int>(  0, 0);

    EXPECT_EQ(array_host, nullptr);

    int* array_device_copy = copyCreateHost2DeviceArray<int>(array_host, 0);

    EXPECT_EQ(array_device_copy, nullptr);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateDevice2DeviceArray_empty)
{
    int* array_device = createDeviceArray<int>(0, 0);

    EXPECT_EQ(array_device, nullptr);

    int* array_device_copy = copyCreateDevice2DeviceArray<int>(array_device, 0);

    EXPECT_EQ(array_device_copy, nullptr);
}


namespace
{
    void copyCreateDevice2DeviceFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array = createDeviceArray<int>(size, default_value);
            int* array_copy = copyCreateDevice2DeviceArray<int>(array, size);

            #if STDGPU_DEVICE_COMPILER == STDGPU_DEVICE_COMPILER_NVCC
                EXPECT_TRUE( thrust::equal(stdgpu::device_begin(array), stdgpu::device_end(array),
                                           stdgpu::device_begin(array_copy),
                                           thrust::equal_to<int>()) );
            #endif

            destroyDeviceArray<int>(array);
            destroyDeviceArray<int>(array_copy);
        }
    }
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateDevice2DeviceArray)
{
    copyCreateDevice2DeviceFunction(1);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateDevice2DeviceArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&copyCreateDevice2DeviceFunction,
                                           iterations_per_thread);
}


namespace
{
    void copyCreateHost2DeviceFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array = createDeviceArray<int>(size, default_value);
            int* array_host = createHostArray<int>(size, default_value);
            int* array_copy = copyCreateHost2DeviceArray<int>(array_host, size);

            #if STDGPU_DEVICE_COMPILER == STDGPU_DEVICE_COMPILER_NVCC
                EXPECT_TRUE( thrust::equal(stdgpu::device_cbegin(array), stdgpu::device_cend(array),
                                           stdgpu::device_cbegin(array_copy),
                                           thrust::equal_to<int>()) );
            #endif

            destroyDeviceArray<int>(array);
            destroyHostArray<int>(array_host);
            destroyDeviceArray<int>(array_copy);
        }
    }
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateHost2DeviceArray)
{
    copyCreateHost2DeviceFunction(1);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateHost2DeviceArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&copyCreateHost2DeviceFunction,
                                           iterations_per_thread);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateHost2DeviceArray_no_check)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array = createDeviceArray<int>(size, default_value);
    int* array_host = new int[size];
    for (stdgpu::index64_t i = 0; i < size; ++i)
    {
        array_host[i] = default_value;
    }
    int* array_copy = copyCreateHost2DeviceArray<int>(array_host, size, MemoryCopy::NO_CHECK);

    #if STDGPU_DEVICE_COMPILER == STDGPU_DEVICE_COMPILER_NVCC
        EXPECT_TRUE( thrust::equal(stdgpu::device_cbegin(array), stdgpu::device_cend(array),
                                   stdgpu::device_cbegin(array_copy),
                                   thrust::equal_to<int>()) );
    #endif

    destroyDeviceArray<int>(array);
    delete[] array_host;
    destroyDeviceArray<int>(array_copy);
}


namespace
{
    void copyCreateDevice2HostFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array = createDeviceArray<int>(size, default_value);
            int* array_host = createHostArray<int>(size, default_value);
            int* array_copy = copyCreateDevice2HostArray<int>(array, size);

            EXPECT_TRUE( thrust::equal(stdgpu::host_cbegin(array_host), stdgpu::host_cend(array_host),
                                       stdgpu::host_cbegin(array_copy),
                                       thrust::equal_to<int>()) );

            destroyDeviceArray<int>(array);
            destroyHostArray<int>(array_host);
            destroyHostArray<int>(array_copy);
        }
    }
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateDevice2HostArray)
{
    copyCreateDevice2HostFunction(1);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateDevice2HostArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&copyCreateDevice2HostFunction,
                                           iterations_per_thread);
}


namespace
{
    void copyCreateHost2HostFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array_host = createHostArray<int>(size, default_value);
            int* array_copy = copyCreateHost2HostArray<int>(array_host, size);

            EXPECT_TRUE( thrust::equal(stdgpu::host_cbegin(array_host), stdgpu::host_cend(array_host),
                                       stdgpu::host_cbegin(array_copy),
                                       thrust::equal_to<int>()) );

            destroyHostArray<int>(array_host);
            destroyHostArray<int>(array_copy);
        }
    }
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateHost2HostArray)
{
    copyCreateHost2HostFunction(1);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateHost2HostArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&copyCreateHost2HostFunction,
                                           iterations_per_thread);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyCreateHost2HostArray_no_check)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array_host = new int[size];
    for (stdgpu::index64_t i = 0; i < size; ++i)
    {
        array_host[i] = default_value;
    }
    int* array_copy = copyCreateHost2HostArray<int>(array_host, size, MemoryCopy::NO_CHECK);

    EXPECT_TRUE( thrust::equal(thrust::host,
                               array_host, array_host + size,
                               stdgpu::host_cbegin(array_copy),
                               thrust::equal_to<int>()) );

    delete[] array_host;
    destroyHostArray<int>(array_copy);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyHost2HostArray_empty)
{
    int* array_host = createHostArray<int>(1, 0);

    int* array_host_copy = reinterpret_cast<int*>(42);

    copyHost2HostArray<int>(array_host, 0, array_host_copy);
    EXPECT_EQ(array_host_copy, reinterpret_cast<int*>(42));

    destroyHostArray<int>(array_host);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyDevice2HostArray_empty)
{
    int* array_device = createDeviceArray<int>(1, 0);

    int* array_host_copy = reinterpret_cast<int*>(42);

    copyDevice2HostArray<int>(array_device, 0, array_host_copy);
    EXPECT_EQ(array_host_copy, reinterpret_cast<int*>(42));

    destroyDeviceArray<int>(array_device);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyHost2DeviceArray_empty)
{
    int* array_host = createHostArray<int>(1, 0);

    int* array_device_copy = reinterpret_cast<int*>(42);

    copyHost2DeviceArray<int>(array_host, 0, array_device_copy);
    EXPECT_EQ(array_device_copy, reinterpret_cast<int*>(42));
    destroyHostArray<int>(array_host);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyDevice2DeviceArray_empty)
{
    int* array_device = createDeviceArray<int>(1, 0);

    int* array_device_copy = reinterpret_cast<int*>(42);

    copyDevice2DeviceArray<int>(array_device, 0, array_device_copy);
    EXPECT_EQ(array_device_copy, reinterpret_cast<int*>(42));

    destroyDeviceArray<int>(array_device);
}


namespace
{
    void copyDevice2DeviceFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array = createDeviceArray<int>(size, default_value);
            int* array_copy = createDeviceArray<int>(size, 0);
            copyDevice2DeviceArray<int>(array, size, array_copy);

            #if STDGPU_DEVICE_COMPILER == STDGPU_DEVICE_COMPILER_NVCC
                EXPECT_TRUE( thrust::equal(stdgpu::device_cbegin(array), stdgpu::device_cend(array),
                                           stdgpu::device_cbegin(array_copy),
                                           thrust::equal_to<int>()) );
            #endif

            destroyDeviceArray<int>(array);
            destroyDeviceArray<int>(array_copy);
        }
    }
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyDevice2DeviceArray)
{
    copyDevice2DeviceFunction(1);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyDevice2DeviceArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&copyDevice2DeviceFunction,
                                           iterations_per_thread);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyDevice2DeviceArray_self)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array = createDeviceArray<int>(size, default_value);
    int* array_copy = array;
    copyDevice2DeviceArray<int>(array, size, array_copy);

    #if STDGPU_DEVICE_COMPILER == STDGPU_DEVICE_COMPILER_NVCC
        EXPECT_TRUE( thrust::equal(stdgpu::device_cbegin(array), stdgpu::device_cend(array),
                                   stdgpu::device_cbegin(array_copy),
                                   thrust::equal_to<int>()) );
    #endif

    destroyDeviceArray<int>(array);
}


namespace
{
    void copyHost2DeviceFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array = createDeviceArray<int>(size, default_value);
            int* array_host = createHostArray<int>(size, default_value);
            int* array_copy = createDeviceArray<int>(size, 0);
            copyHost2DeviceArray<int>(array_host, size, array_copy);

            #if STDGPU_DEVICE_COMPILER == STDGPU_DEVICE_COMPILER_NVCC
                EXPECT_TRUE( thrust::equal(stdgpu::device_cbegin(array), stdgpu::device_cend(array),
                                           stdgpu::device_cbegin(array_copy),
                                           thrust::equal_to<int>()) );
            #endif

            destroyDeviceArray<int>(array);
            destroyHostArray<int>(array_host);
            destroyDeviceArray<int>(array_copy);
        }
    }
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyHost2DeviceArray)
{
    copyHost2DeviceFunction(1);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyHost2DeviceArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&copyHost2DeviceFunction,
                                           iterations_per_thread);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyHost2DeviceArray_no_check)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array = createDeviceArray<int>(size, default_value);
    int* array_host = new int[size];
    for (stdgpu::index64_t i = 0; i < size; ++i)
    {
        array_host[i] = default_value;
    }
    int* array_copy = createDeviceArray<int>(size, 0);
    copyHost2DeviceArray<int>(array_host, size, array_copy, MemoryCopy::NO_CHECK);

    #if STDGPU_DEVICE_COMPILER == STDGPU_DEVICE_COMPILER_NVCC
        EXPECT_TRUE( thrust::equal(stdgpu::device_cbegin(array), stdgpu::device_cend(array),
                                   stdgpu::device_cbegin(array_copy),
                                   thrust::equal_to<int>()) );
    #endif

    destroyDeviceArray<int>(array);
    delete[] array_host;
    destroyDeviceArray<int>(array_copy);
}


namespace
{
    void copyDevice2HostFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array = createDeviceArray<int>(size, default_value);
            int* array_host = createHostArray<int>(size, default_value);
            int* array_copy = createHostArray<int>(size, 0);
            copyDevice2HostArray<int>(array, size, array_copy);

            EXPECT_TRUE( thrust::equal(stdgpu::host_cbegin(array_host), stdgpu::host_cend(array_host),
                                       stdgpu::host_cbegin(array_copy),
                                       thrust::equal_to<int>()) );

            destroyDeviceArray<int>(array);
            destroyHostArray<int>(array_host);
            destroyHostArray<int>(array_copy);
        }
    }
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyDevice2HostArray)
{
    copyDevice2HostFunction(1);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyDevice2HostArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&copyDevice2HostFunction,
                                           iterations_per_thread);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyDevice2HostArray_no_check)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array = createDeviceArray<int>(size, default_value);
    int* array_host = createHostArray<int>(size, default_value);
    int* array_copy = new int[size];
    copyDevice2HostArray<int>(array, size, array_copy, MemoryCopy::NO_CHECK);

    EXPECT_TRUE( thrust::equal(thrust::host,
                               stdgpu::host_cbegin(array_host), stdgpu::host_cend(array_host),
                               array_copy,
                               thrust::equal_to<int>()) );

    destroyDeviceArray<int>(array);
    destroyHostArray<int>(array_host);
    delete[] array_copy;
}


namespace
{
    void copyHost2HostFunction(const stdgpu::index_t iterations)
    {
        for (stdgpu::index_t i = 0; i < iterations; ++i)
        {
            int default_value = 10;
            stdgpu::index64_t size = 42;

            int* array_host = createHostArray<int>(size, default_value);
            int* array_copy = createHostArray<int>(size, 0);
            copyHost2HostArray<int>(array_host, size, array_copy);

            EXPECT_TRUE( thrust::equal(stdgpu::host_cbegin(array_host), stdgpu::host_cend(array_host),
                                       stdgpu::host_cbegin(array_copy),
                                       thrust::equal_to<int>()) );

            destroyHostArray<int>(array_host);
            destroyHostArray<int>(array_copy);
        }
    }
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyHost2HostArray)
{
    copyHost2HostFunction(1);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyHost2HostArray_parallel)
{
    stdgpu::index_t iterations_per_thread = static_cast<stdgpu::index_t>(pow(2, 7));

    test_utils::for_each_concurrent_thread(&copyHost2HostFunction,
                                           iterations_per_thread);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyHost2HostArray_no_check)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array_host = new int[size];
    for (stdgpu::index64_t i = 0; i < size; ++i)
    {
        array_host[i] = default_value;
    }
    int* array_copy = new int[size];
    copyHost2HostArray<int>(array_host, size, array_copy, MemoryCopy::NO_CHECK);

    EXPECT_TRUE( thrust::equal(thrust::host,
                               array_host, array_host + size,
                               array_copy,
                               thrust::equal_to<int>()) );

    delete[] array_host;
    delete[] array_copy;
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyHost2HostArray_self)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array_host = createHostArray<int>(size, default_value);
    int* array_copy = array_host;
    copyHost2HostArray<int>(array_host, size, array_copy);

    EXPECT_TRUE( thrust::equal(stdgpu::host_cbegin(array_host), stdgpu::host_cend(array_host),
                               stdgpu::host_cbegin(array_copy),
                               thrust::equal_to<int>()) );

    destroyHostArray<int>(array_host);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, copyHost2HostArray_self_no_check)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array_host = new int[size];
    for (stdgpu::index64_t i = 0; i < size; ++i)
    {
        array_host[i] = default_value;
    }
    int* array_copy = array_host;
    copyHost2HostArray<int>(array_host, size, array_copy, MemoryCopy::NO_CHECK);

    EXPECT_TRUE( thrust::equal(thrust::host,
                               array_host, array_host + size,
                               array_copy,
                               thrust::equal_to<int>()) );

    delete[] array_host;
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, destroyDeviceArray_double_free)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array_device   = createDeviceArray<int>(default_value, size);
    int* array_device_2 = array_device;

    destroyDeviceArray<int>(array_device);
    destroyDeviceArray<int>(array_device_2);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, destroyHostArray_double_free)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array_host     = createHostArray<int>(size, default_value);
    int* array_host_2   = array_host;

    destroyHostArray<int>(array_host);
    destroyHostArray<int>(array_host_2);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, destroyManangedArray_double_free)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array_managed_device = createManagedArray<int>(size, default_value, Initialization::DEVICE);
    int* array_managed_host = createManagedArray<int>(size, default_value, Initialization::HOST);
    int* array_managed_device_2 = array_managed_device;
    int* array_managed_host_2 = array_managed_host;

    destroyManagedArray<int>(array_managed_device);
    destroyManagedArray<int>(array_managed_device_2);
    destroyManagedArray<int>(array_managed_host);
    destroyManagedArray<int>(array_managed_host_2);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, destroyDeviceArray_double_free_shifted)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array_device = createDeviceArray<int>(size, default_value);
    int* array_device_2 = array_device + 24;

    destroyDeviceArray<int>(array_device);
    destroyDeviceArray<int>(array_device_2);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, destroyHostArray_double_free_shifted)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array_host = createHostArray<int>(size, default_value);
    int* array_host_2 = array_host + 24;

    destroyHostArray<int>(array_host);
    destroyHostArray<int>(array_host_2);
}


TEST_F(STDGPU_MEMORY_TEST_CLASS, destroyManangedArray_double_free_shifted)
{
    int default_value = 10;
    stdgpu::index64_t size = 42;

    int* array_managed_device = createManagedArray<int>(size, default_value, Initialization::DEVICE);
    int* array_managed_host = createManagedArray<int>(size, default_value, Initialization::HOST);
    int* array_managed_device_2 = array_managed_device + 24;
    int* array_managed_host_2 = array_managed_host + 24;

    destroyManagedArray<int>(array_managed_device);
    destroyManagedArray<int>(array_managed_device_2);
    destroyManagedArray<int>(array_managed_host);
    destroyManagedArray<int>(array_managed_host_2);
}


