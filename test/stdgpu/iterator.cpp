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

#include <gtest/gtest.h>

#include <vector>
#include <thrust/copy.h>
#include <thrust/sequence.h>
#include <thrust/sort.h>

#include <stdgpu/iterator.h>
#include <stdgpu/memory.h>



class stdgpu_iterator : public ::testing::Test
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


TEST_F(stdgpu_iterator, size_device_void)
{
    int* array = createDeviceArray<int>(42);

    EXPECT_EQ(stdgpu::size((void*)array), 42 * sizeof(int));

    destroyDeviceArray<int>(array);
}


TEST_F(stdgpu_iterator, size_host_void)
{
    int* array_result = createHostArray<int>(42);

    EXPECT_EQ(stdgpu::size((void*)array_result), 42 * sizeof(int));

    destroyHostArray<int>(array_result);
}


TEST_F(stdgpu_iterator, size_nullptr_void)
{
    EXPECT_EQ(stdgpu::size((void*)nullptr), static_cast<size_t>(0));
}


TEST_F(stdgpu_iterator, size_device)
{
    int* array = createDeviceArray<int>(42);

    EXPECT_EQ(stdgpu::size(array), static_cast<size_t>(42));

    destroyDeviceArray<int>(array);
}


TEST_F(stdgpu_iterator, size_host)
{
    int* array_result = createHostArray<int>(42);

    EXPECT_EQ(stdgpu::size(array_result), static_cast<size_t>(42));

    destroyHostArray<int>(array_result);
}


TEST_F(stdgpu_iterator, size_nullptr)
{
    EXPECT_EQ(stdgpu::size((int*)nullptr), static_cast<size_t>(0));
}


TEST_F(stdgpu_iterator, size_device_shifted)
{
    int* array = createDeviceArray<int>(42);

    EXPECT_EQ(stdgpu::size(array + 24), static_cast<size_t>(0));

    destroyDeviceArray<int>(array);
}


TEST_F(stdgpu_iterator, size_host_shifted)
{
    int* array_result = createHostArray<int>(42);

    EXPECT_EQ(stdgpu::size(array_result + 24), static_cast<size_t>(0));

    destroyHostArray<int>(array_result);
}


TEST_F(stdgpu_iterator, size_device_wrong_alignment)
{
    int* array = createDeviceArray<int>(1);

    EXPECT_EQ(stdgpu::size(reinterpret_cast<size_t*>(array)), static_cast<size_t>(0));

    destroyDeviceArray<int>(array);
}


TEST_F(stdgpu_iterator, size_host_wrong_alignment)
{
    int* array_result = createHostArray<int>(1);

    EXPECT_EQ(stdgpu::size(reinterpret_cast<size_t*>(array_result)), static_cast<size_t>(0));

    destroyHostArray<int>(array_result);
}


TEST_F(stdgpu_iterator, device_begin_end)
{
    int* array = createDeviceArray<int>(42);

    int* array_begin   = stdgpu::device_begin(array).get();
    int* array_end     = stdgpu::device_end(  array).get();

    EXPECT_EQ(array_begin, array);
    EXPECT_EQ(array_end,   array + 42);

    destroyDeviceArray<int>(array);
}


TEST_F(stdgpu_iterator, host_begin_end)
{
    int* array_result = createHostArray<int>(42);

    int* array_result_begin   = stdgpu::host_begin(array_result).get();
    int* array_result_end     = stdgpu::host_end(  array_result).get();

    EXPECT_EQ(array_result_begin, array_result);
    EXPECT_EQ(array_result_end,   array_result + 42);

    destroyHostArray<int>(array_result);
}


TEST_F(stdgpu_iterator, device_begin_end_const)
{
    int* array = createDeviceArray<int>(42);

    const int* array_begin   = stdgpu::device_begin(reinterpret_cast<const int*>(array)).get();
    const int* array_end     = stdgpu::device_end(  reinterpret_cast<const int*>(array)).get();

    EXPECT_EQ(array_begin, array);
    EXPECT_EQ(array_end,   array + 42);

    destroyDeviceArray<int>(array);
}


TEST_F(stdgpu_iterator, host_begin_end_const)
{
    int* array_result = createHostArray<int>(42);

    const int* array_result_begin   = stdgpu::host_begin(reinterpret_cast<const int*>(array_result)).get();
    const int* array_result_end     = stdgpu::host_end(  reinterpret_cast<const int*>(array_result)).get();

    EXPECT_EQ(array_result_begin, array_result);
    EXPECT_EQ(array_result_end,   array_result + 42);

    destroyHostArray<int>(array_result);
}


TEST_F(stdgpu_iterator, device_cbegin_cend)
{
    int* array = createDeviceArray<int>(42);

    const int* array_begin   = stdgpu::device_cbegin(array).get();
    const int* array_end     = stdgpu::device_cend(  array).get();

    EXPECT_EQ(array_begin, array);
    EXPECT_EQ(array_end,   array + 42);

    destroyDeviceArray<int>(array);
}


TEST_F(stdgpu_iterator, host_cbegin_cend)
{
    int* array_result = createHostArray<int>(42);

    const int* array_result_begin   = stdgpu::host_cbegin(array_result).get();
    const int* array_result_end     = stdgpu::host_cend(  array_result).get();

    EXPECT_EQ(array_result_begin, array_result);
    EXPECT_EQ(array_result_end,   array_result + 42);

    destroyHostArray<int>(array_result);
}


struct back_insert_interface
{
    using value_type = std::vector<int>::value_type;

    back_insert_interface(std::vector<int>& vector)
        : vector(vector)
    {

    }

    void
    push_back(const int x)
    {
        vector.push_back(x);
    }

    std::vector<int>& vector;
};


struct front_insert_interface
{
    using value_type = std::vector<int>::value_type;

    front_insert_interface(std::vector<int>& vector)
        : vector(vector)
    {

    }

    void
    push_front(const int x)
    {
        vector.push_back(x);
    }

    std::vector<int>& vector;
};


struct insert_interface
{
    using value_type = std::vector<int>::value_type;

    insert_interface(std::vector<int>& vector)
        : vector(vector)
    {

    }

    void
    insert(const int x)
    {
        vector.push_back(x);
    }

    std::vector<int>& vector;
};


TEST_F(stdgpu_iterator, back_inserter)
{
    const stdgpu::index_t N = 100000;

    int* array = createHostArray<int>(N);
    std::vector<int> numbers;

    thrust::sequence(stdgpu::host_begin(array), stdgpu::host_end(array),
                     1);

    back_insert_interface ci(numbers);
    thrust::copy(stdgpu::host_cbegin(array), stdgpu::host_cend(array),
                 stdgpu::back_inserter(ci));

    int* array_result = copyCreateHost2HostArray<int>(numbers.data(), N, MemoryCopy::NO_CHECK);

    thrust::sort(stdgpu::host_begin(array_result), stdgpu::host_end(array_result));

    for (stdgpu::index_t i = 0; i < N; ++i)
    {
        EXPECT_EQ(array_result[i], i + 1);
    }

    destroyHostArray<int>(array_result);
    destroyHostArray<int>(array);
}


TEST_F(stdgpu_iterator, front_inserter)
{
    const stdgpu::index_t N = 100000;

    int* array = createHostArray<int>(N);
    std::vector<int> numbers;

    thrust::sequence(stdgpu::host_begin(array), stdgpu::host_end(array),
                     1);

    front_insert_interface ci(numbers);
    thrust::copy(stdgpu::host_cbegin(array), stdgpu::host_cend(array),
                 stdgpu::front_inserter(ci));

    int* array_result = copyCreateHost2HostArray<int>(numbers.data(), N, MemoryCopy::NO_CHECK);

    thrust::sort(stdgpu::host_begin(array_result), stdgpu::host_end(array_result));

    for (stdgpu::index_t i = 0; i < N; ++i)
    {
        EXPECT_EQ(array_result[i], i + 1);
    }

    destroyHostArray<int>(array_result);
    destroyHostArray<int>(array);
}


TEST_F(stdgpu_iterator, inserter)
{
    const stdgpu::index_t N = 100000;

    int* array = createHostArray<int>(N);
    std::vector<int> numbers;

    thrust::sequence(stdgpu::host_begin(array), stdgpu::host_end(array),
                     1);

    insert_interface ci(numbers);
    thrust::copy(stdgpu::host_cbegin(array), stdgpu::host_cend(array),
                 stdgpu::inserter(ci));

    int* array_result = copyCreateHost2HostArray<int>(numbers.data(), N, MemoryCopy::NO_CHECK);

    thrust::sort(stdgpu::host_begin(array_result), stdgpu::host_end(array_result));

    for (stdgpu::index_t i = 0; i < N; ++i)
    {
        EXPECT_EQ(array_result[i], i + 1);
    }

    destroyHostArray<int>(array_result);
    destroyHostArray<int>(array);
}


