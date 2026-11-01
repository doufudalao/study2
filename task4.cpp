#include <array>
#include <chrono>
#include <condition_variable>
//实现条件变量功能 事件驱动的思想
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <mutex>
//实现互斥锁安全功能
#include <queue>
//实现队列功能
#include <random>
#include <thread>
//实现多线程功能
#include <vector>
//动态的数组 容器

struct SensorPacket 
// 传感器数据包
{
    std::uint64_t id;
    std::chrono::steady_clock::time_point send_time;
    std::vector<int> values;
};

int main() 
{
    constexpr std::uint64_t packet_count = 20;

    constexpr std::size_t values_per_packet = 5;

    std::queue<SensorPacket> packets;
    //共享数据
    std::mutex queue_mutex;
    std::condition_variable queue_changed;
    bool collection_finished = false;

    std::cout << "小洁的传感器监测程序已启动\n";

    // 采集线程：每隔 100 ms 生成并发送一组数据。

    std::thread collector([&]() 
    {
        std::mt19937 generator(std::random_device{}());
        //随机数生成器
        std::uniform_int_distribution<int> distribution(0, 100);

        for (std::uint64_t id = 0; id < packet_count; ++id) {
            SensorPacket packet;
            packet.id = id;
            packet.send_time = std::chrono::steady_clock::now();

            for (std::size_t i = 0; i < values_per_packet; ++i)
            {
                packet.values.push_back(distribution(generator));
            }
            //  括号可以缩减锁的范围，避免不必要的锁定时间，提高效率
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                //用move将数据包移动到队列中，避免不必要的拷贝
                packets.push(std::move(packet));
            }
            // 通知处理线程有新数据可用。
            queue_changed.notify_one();


            // 采集线程每隔 100 ms 生成一组数据
            //+1是因为最后一次采集后不需要再等待
            if (id + 1 < packet_count) 
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        // 采集线程完成数据采集，设置标志并通知处理线程。
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            collection_finished = true;
        }
        queue_changed.notify_one();
    });

    // 处理线程：按队列顺序取出并处理数据。
    std::thread processor([&]() 
    {
        while (true) 
        {
            SensorPacket packet;

            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                // 等待条件变量，直到队列不为空或采集完成。
                queue_changed.wait(lock, [&]() 
                {
                    return !packets.empty() || collection_finished;
                });

                // 采集已结束且队列为空，处理线程可以退出。
                if (packets.empty() && collection_finished) 
                {
                    break;
                }

                //先取出再删除
                packet = std::move(packets.front());
                packets.pop();
            }


            //min_it和max_it分别是最小值和最大值的迭代器，他类似指针所以获取的时候要*min_it和*max_it
            const auto [min_it, max_it] =
                std::minmax_element(packet.values.begin(), packet.values.end());

            int sum = 0;
            for (int value : packet.values)
            {
                sum += value;
            }

            const double average =
                static_cast<double>(sum) / packet.values.size();

            const auto finish_time = std::chrono::steady_clock::now();

            //这里是一个类模板 和 重载函数很像 通过这个又了解到函数模板
            const double elapsed_ms =
                std::chrono::duration<double, std::milli>
                (
                    finish_time - packet.send_time
                ).count();

            std::cout << "[结果] id=" << packet.id<< ", 数据=[";

            for (std::size_t i = 0; i < packet.values.size(); ++i) 
            {
                if (i > 0) {
                    std::cout << ", ";
                }
                std::cout << packet.values[i];
            }

            std::cout << "], 最小值=" << *min_it
                      << ", 最大值=" << *max_it
                      << ", 平均值=" << std::fixed << std::setprecision(2)
                      << average
                      << ", 耗时=" << std::setprecision(2)
                      << elapsed_ms << " ms\n";
        }
    });
    //主线程会等待采集线程和处理线程完成。还有一个detach（）的线程，主线程不会等待它完成，它会在后台运行，直到程序退出。
    collector.join();
    processor.join();

    std::cout << "采集与处理完成，共处理 " << packet_count << " 组数据，程序退出\n";

    return 0;
}
//这次的程序实现了一个简单的传感器数据采集与处理系统，使用了多线程、互斥锁和条件变量来保证线程安全和数据同步。
//这次的题目一点都不简单sos，边学边写，边看代码示例，边调试，最后终于写完了
//甚至结束后我还是觉得我一个人单独啃下来这类题目会有点难
//通过这个程序，我对C++的多线程编程有了更深入的理解，也掌握了条件变量和互斥锁的使用方法。