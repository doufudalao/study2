#include <iostream>
#include <stdexcept>
#include <string>
#include <yaml-cpp/yaml.h>

class Config {
public:
    int a;
    int b;

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
        const Config config = Config::load("config.yaml");
        std::cout << "a+b = " << config.a + config.b << '\n';
    } catch (const std::exception& error) {
        std::cerr << "配置错误：" << error.what() << '\n';
        return 1;
    }

    return 0;
}