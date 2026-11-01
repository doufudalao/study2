#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

//创建一个基类,并且规定这个基类必须有area和perimeter两个函数，这个基类就是一个抽象类,不能被实例化,只能被继承
class Shape 
{
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    virtual ~Shape() = default;
};

//继承基类Shape，创建一个三角形类Triangle
class Triangle : public Shape 
{
private:
    double a_;
    double b_;
    double c_;

public:
    Triangle(double a, double b, double c)
        : a_(a), b_(b), c_(c) {}

    double area() const override    
    {
        const double s = perimeter() / 2.0;
        return std::sqrt(s * (s - a_) * (s - b_) * (s - c_));
    }

    double perimeter() const override 
    {
        return a_ + b_ + c_;
    }
};

//继承基类Shape，创建一个矩形类Rectangle
class Rectangle : public Shape 
{
private:
    double width_;
    double height_;

public:
    Rectangle(double width, double height)
        : width_(width), height_(height) {}

    double area() const override {
        return width_ * height_;
    }

    double perimeter() const override {
        return 2.0 * (width_ + height_);
    }
};

//继承基类Shape，创建一个圆形类Circle
class Circle : public Shape 
{
private:
    double radius_;

public:
    explicit Circle(double radius)
        : radius_(radius) {}

    double area() const override {
        constexpr double pi = 3.14159265358979323846;
        return pi * radius_ * radius_;
    }

    double perimeter() const override {
        constexpr double pi = 3.14159265358979323846;
        return 2.0 * pi * radius_;
    }
};


int main() 
{
    //这里创建了一个独占智能指针
    std::vector<std::unique_ptr<Shape>> shapes;
    //make_unique这个工作函数会创建一个对象 并且返还一个智能指针
    shapes.push_back(std::make_unique<Triangle>(3.0, 4.0, 5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    shapes.push_back(std::make_unique<Circle>(2.0));

    //设置输出格式为固定小数点表示，并保留两位小数

    std::cout << std::fixed << std::setprecision(2);

    for (std::size_t i = 0; i < shapes.size(); ++i) 
    {
        std::cout << "Shape " << i + 1 << '\n';
        
        // 通过多态和指针调用area和perimeter函数
        std::cout << "  Area: " << shapes[i]->area() << '\n';
        std::cout << "  Perimeter: " << shapes[i]->perimeter() << "\n\n";
    }

    return 0;
}