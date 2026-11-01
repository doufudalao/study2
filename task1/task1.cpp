#include <iostream>
#include <stdexcept>
#include <string>
#include <yaml-cpp/yaml.h>

// 读取 YAML 配置文件并解析参数 a 和 b
class Config {
public:
    int a;
    int b;
    
// 读取 YAML 文件并返回 Config 对象
    static Config load(const std::string& filename) 
    {
        try 
        {
            const YAML::Node root = YAML::LoadFile(filename);

            if (!root["a"]) 
            {
                throw std::runtime_error("缺少参数 a");
            }
            if (!root["b"]) 
            {
                throw std::runtime_error("缺少参数 b");
            }

            Config config;
            // 使用 as<int>() 方法将 YAML 节点转换为整数类型
            config.a = root["a"].as<int>();
            config.b = root["b"].as<int>();
            return config;
        
        } catch (const YAML::Exception& error) 
        {
            throw std::runtime_error(
                std::string("读取或解析 YAML 失败：") + error.what()
            );
        }
    }
};

int main() 
{
    try {
        // 加载配置文件
        const Config config = Config::load("config.yaml");

        // 输出配置参数
        std::cout << "配置参数 a: " << config.a << '\n';
        std::cout << "配置参数 b: " << config.b << '\n';

        // 计算并输出 a + b 的结果
        std::cout << "a+b = " << config.a + config.b << '\n';
    } catch (const std::exception& error) 
    {
        // 捕获异常并输出错误信息
        std::cerr << "配置错误：" << error.what() << '\n';
        return 1;
    }

    return 0;
}