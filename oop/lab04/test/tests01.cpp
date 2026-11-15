#include <gtest/gtest.h>
#include "../include/figure.h"
#include "../include/custom_shapes.h"
#include "../include/array.h"
#include <sstream>

// ==================== Point Tests ====================

TEST(PointTest, DefaultConstructor) {
    Point<double> p;
    ASSERT_EQ(p.get_x(), 0.0);
    ASSERT_EQ(p.get_y(), 0.0);
}

TEST(PointTest, ParameterizedConstructor) {
    Point<int> p(5, 10);
    ASSERT_EQ(p.get_x(), 5);
    ASSERT_EQ(p.get_y(), 10);
}

TEST(PointTest, InitializerListConstructor) {
    Point<double> p({3.5, 7.2});
    ASSERT_NEAR(p.get_x(), 3.5, 1e-6);
    ASSERT_NEAR(p.get_y(), 7.2, 1e-6);
}

TEST(PointTest, InitializerListPartial) {
    Point<double> p({5.0});
    ASSERT_NEAR(p.get_x(), 5.0, 1e-6);
    ASSERT_NEAR(p.get_y(), 0.0, 1e-6);
}

TEST(PointTest, CopyConstructor) {
    Point<double> p1(3.0, 4.0);
    Point<double> p2(p1);
    ASSERT_EQ(p1.get_x(), p2.get_x());
    ASSERT_EQ(p1.get_y(), p2.get_y());
}

TEST(PointTest, Equality) {
    Point<int> p1(0, 0);
    Point<int> p2(0, 0);
    Point<int> p3(1, 1);
    ASSERT_TRUE(p1 == p2);
    ASSERT_FALSE(p1 == p3);
}

TEST(PointTest, SettersAndGetters) {
    Point<double> p;
    p.set_x(10.5);
    p.set_y(20.3);
    ASSERT_NEAR(p.get_x(), 10.5, 1e-6);
    ASSERT_NEAR(p.get_y(), 20.3, 1e-6);
}

// ==================== Rectangle Tests ====================

TEST(RectangleTest, ConstructorValid) {
    std::initializer_list<Point<double>> pts{
        Point<double>(0, 0), 
        Point<double>(4, 0), 
        Point<double>(4, 3), 
        Point<double>(0, 3)
    };
    Rectangle<double> rect(pts);
    ASSERT_NEAR(static_cast<double>(rect), 12.0, 1e-6);
}

TEST(RectangleTest, ConstructorInvalidVertexCount) {
    std::initializer_list<Point<double>> bad{
        Point<double>(0, 0), 
        Point<double>(1, 0), 
        Point<double>(1, 1)
    };
    ASSERT_THROW(Rectangle<double> badRect(bad), std::logic_error);
}

TEST(RectangleTest, AreaCalculation) {
    Rectangle<double> rect({
        Point<double>(0, 0),
        Point<double>(5, 0),
        Point<double>(5, 2),
        Point<double>(0, 2)
    });
    ASSERT_NEAR(static_cast<double>(rect), 10.0, 1e-6);
}

TEST(RectangleTest, AreaWithNegativeCoords) {
    Rectangle<double> rect({
        Point<double>(-2, -1),
        Point<double>(2, -1),
        Point<double>(2, 1),
        Point<double>(-2, 1)
    });
    ASSERT_NEAR(static_cast<double>(rect), 8.0, 1e-6);
}

TEST(RectangleTest, CenterCount) {
    Rectangle<double> rect({
        Point<double>(0, 0),
        Point<double>(4, 0),
        Point<double>(4, 2),
        Point<double>(0, 2)
    });
    Point<double> center = rect.center_count();
    ASSERT_NEAR(center.get_x(), 2.0, 1e-6);
    ASSERT_NEAR(center.get_y(), 1.0, 1e-6);
}

// ==================== Trapezoid Tests ====================

TEST(TrapezoidTest, ConstructorValidAndInvalid) {
    std::initializer_list<Point<double>> pts{
        Point<double>(0, 0), 
        Point<double>(1, 0), 
        Point<double>(1, 1), 
        Point<double>(0, 1)
    };
    Trapezoid<double> trapezoid(pts);
    ASSERT_NEAR(static_cast<double>(trapezoid), 1.0, 1e-6);
    
    std::initializer_list<Point<double>> bad{
        Point<double>(0, 0), 
        Point<double>(1, 0), 
        Point<double>(1, 1)
    };
    ASSERT_THROW(Trapezoid<double> badTrap(bad), std::logic_error);
}



TEST(TrapezoidTest, AreaWithNegativeCoords) {
    Trapezoid<double> trap({
        Point<double>(-1, -1),
        Point<double>(1, -1),
        Point<double>(1, 1),
        Point<double>(-1, 1)
    });
    double area = static_cast<double>(trap);
    ASSERT_NEAR(area, 4.0, 1e-6);
}

TEST(TrapezoidTest, CenterCount) {
    Trapezoid<double> trap({
        Point<double>(0, 0),
        Point<double>(4, 0),
        Point<double>(4, 2),
        Point<double>(0, 2)
    });
    Point<double> center = trap.center_count();
    ASSERT_NEAR(center.get_x(), 2.0, 1e-6);
    ASSERT_NEAR(center.get_y(), 1.0, 1e-6);
}

// ==================== Rhombus Tests ====================

TEST(RhombusTest, ConstructorValid) {
    Rhombus<double> rhombus({
        Point<double>(2, 0),
        Point<double>(4, 2),
        Point<double>(2, 4),
        Point<double>(0, 2)
    });
    double area = static_cast<double>(rhombus);
    ASSERT_GT(area, 0);
}

TEST(RhombusTest, ConstructorInvalid) {
    std::initializer_list<Point<double>> bad{
        Point<double>(0, 0),
        Point<double>(1, 1),
        Point<double>(2, 0)
    };
    ASSERT_THROW(Rhombus<double> badRhomb(bad), std::logic_error);
}

TEST(RhombusTest, AreaCalculation) {
    Rhombus<double> rhombus({
        Point<double>(0, 0),
        Point<double>(1, 1),
        Point<double>(2, 0),
        Point<double>(1, -1)
    });
    double expected_area = 2.0; // диагонали = 2 и 2 => площадь = 2*2/2 = 2
    ASSERT_NEAR(static_cast<double>(rhombus), expected_area, 1e-6);
}

TEST(RhombusTest, AreaWithRotatedRhombus) {
    Rhombus<double> rh({
        Point<double>(0, 0),
        Point<double>(1, 1),
        Point<double>(2, 0),
        Point<double>(1, -1)
    });
    double area = static_cast<double>(rh);
    ASSERT_NEAR(area, 2.0, 1e-6);
}

TEST(RhombusTest, CenterCount) {
    Rhombus<double> rhombus({
        Point<double>(2, 0),
        Point<double>(4, 2),
        Point<double>(2, 4),
        Point<double>(0, 2)
    });
    Point<double> center = rhombus.center_count();
    ASSERT_NEAR(center.get_x(), 2.0, 1e-6);
    ASSERT_NEAR(center.get_y(), 2.0, 1e-6);
}

// ==================== Figure Tests ====================

TEST(FigureTest, Equality) {
    Trapezoid<double> t1({
        Point<double>(0, 0),
        Point<double>(1, 0),
        Point<double>(1, 1),
        Point<double>(0, 1)
    });
    Trapezoid<double> t2({
        Point<double>(0, 0),
        Point<double>(1, 0),
        Point<double>(1, 1),
        Point<double>(0, 1)
    });
    Trapezoid<double> t3({
        Point<double>(0, 0),
        Point<double>(2, 0),
        Point<double>(2, 2),
        Point<double>(0, 2)
    });
    
    ASSERT_TRUE(t1 == t2);
    ASSERT_FALSE(t1 == t3);
}

TEST(FigureTest, CopyConstructor) {
    Trapezoid<double> t1({
        Point<double>(0, 0),
        Point<double>(1, 0),
        Point<double>(1, 1),
        Point<double>(0, 1)
    });
    Trapezoid<double> t2(t1);
    ASSERT_TRUE(t2 == t1);
}

TEST(FigureTest, MoveConstructor) {
    Trapezoid<double> t1({
        Point<double>(0, 0),
        Point<double>(1, 0),
        Point<double>(1, 1),
        Point<double>(0, 1)
    });
    Trapezoid<double> t2(std::move(t1));
    ASSERT_NEAR(t2.center_count().get_x(), 0.5, 1e-6);
}

TEST(FigureTest, CopyAssignment) {
    Rectangle<double> r1({
        Point<double>(0, 0),
        Point<double>(2, 0),
        Point<double>(2, 2),
        Point<double>(0, 2)
    });
    Rectangle<double> r2({
        Point<double>(1, 1),
        Point<double>(3, 1),
        Point<double>(3, 3),
        Point<double>(1, 3)
    });
    
    r2 = r1;
    ASSERT_TRUE(r1 == r2);
}

TEST(FigureTest, OutputOperator) {
    Rectangle<double> rect({
        Point<double>(0, 0),
        Point<double>(1, 0),
        Point<double>(1, 1),
        Point<double>(0, 1)
    });
    
    std::stringstream ss;
    ss << rect;
    std::string output = ss.str();
    ASSERT_FALSE(output.empty());
}

TEST(FigureTest, EqualityDifferentSizes) {
    Rectangle<double> rect({
        Point<double>(0, 0),
        Point<double>(1, 0),
        Point<double>(1, 1),
        Point<double>(0, 1)
    });
    Trapezoid<double> trap({
        Point<double>(0, 0),
        Point<double>(1, 0),
        Point<double>(1, 1),
        Point<double>(0, 1)
    });
    
    // Разные типы, но одинаковое количество вершин
    ASSERT_TRUE(rect == trap);
}

// ==================== Array Tests ====================

TEST(ArrayTest, DefaultConstructor) {
    Array<Rectangle<double>> arr;
    ASSERT_EQ(arr.size(), 0);
    ASSERT_TRUE(arr.empty());
}

TEST(ArrayTest, ParameterizedConstructor) {
    Array<Rectangle<double>> arr(10);
    ASSERT_EQ(arr.size(), 0);
    ASSERT_EQ(arr.capacity(), 10);
}

TEST(ArrayTest, PushBack) {
    Array<Rectangle<double>> arr;
    auto rect = std::make_shared<Rectangle<double>>(
        std::initializer_list<Point<double>>{
            Point<double>(0, 0),
            Point<double>(1, 0),
            Point<double>(1, 1),
            Point<double>(0, 1)
        }
    );
    
    arr.push_back(rect);
    ASSERT_EQ(arr.size(), 1);
    ASSERT_FALSE(arr.empty());
}

TEST(ArrayTest, PushBackMultiple) {
    Array<Rectangle<double>> arr;
    
    for (int i = 0; i < 5; ++i) {
        auto rect = std::make_shared<Rectangle<double>>(
            std::initializer_list<Point<double>>{
                Point<double>(0, 0),
                Point<double>(i + 1, 0),
                Point<double>(i + 1, i + 1),
                Point<double>(0, i + 1)
            }
        );
        arr.push_back(rect);
    }
    
    ASSERT_EQ(arr.size(), 5);
}

TEST(ArrayTest, Remove) {
    Array<Rectangle<double>> arr;
    
    for (int i = 0; i < 3; ++i) {
        auto rect = std::make_shared<Rectangle<double>>(
            std::initializer_list<Point<double>>{
                Point<double>(0, 0),
                Point<double>(i + 1, 0),
                Point<double>(i + 1, i + 1),
                Point<double>(0, i + 1)
            }
        );
        arr.push_back(rect);
    }
    
    arr.remove(1);
    ASSERT_EQ(arr.size(), 2);
}

TEST(ArrayTest, RemoveInvalidIndex) {
    Array<Rectangle<double>> arr;
    auto rect = std::make_shared<Rectangle<double>>(
        std::initializer_list<Point<double>>{
            Point<double>(0, 0),
            Point<double>(1, 0),
            Point<double>(1, 1),
            Point<double>(0, 1)
        }
    );
    arr.push_back(rect);
    
    ASSERT_THROW(arr.remove(5), std::out_of_range);
}

TEST(ArrayTest, IndexOperator) {
    Array<Rectangle<double>> arr;
    auto rect = std::make_shared<Rectangle<double>>(
        std::initializer_list<Point<double>>{
            Point<double>(0, 0),
            Point<double>(2, 0),
            Point<double>(2, 2),
            Point<double>(0, 2)
        }
    );
    arr.push_back(rect);
    
    ASSERT_NEAR(static_cast<double>(*arr[0]), 4.0, 1e-6);
}

TEST(ArrayTest, IndexOperatorOutOfRange) {
    Array<Rectangle<double>> arr;
    ASSERT_THROW(arr[0], std::out_of_range);
}

TEST(ArrayTest, CopyConstructor) {
    Array<Rectangle<double>> arr1;
    auto rect = std::make_shared<Rectangle<double>>(
        std::initializer_list<Point<double>>{
            Point<double>(0, 0),
            Point<double>(1, 0),
            Point<double>(1, 1),
            Point<double>(0, 1)
        }
    );
    arr1.push_back(rect);
    
    Array<Rectangle<double>> arr2(arr1);
    ASSERT_EQ(arr1.size(), arr2.size());
}

TEST(ArrayTest, MoveConstructor) {
    Array<Rectangle<double>> arr1;
    auto rect = std::make_shared<Rectangle<double>>(
        std::initializer_list<Point<double>>{
            Point<double>(0, 0),
            Point<double>(1, 0),
            Point<double>(1, 1),
            Point<double>(0, 1)
        }
    );
    arr1.push_back(rect);
    
    Array<Rectangle<double>> arr2(std::move(arr1));
    ASSERT_EQ(arr2.size(), 1);
    ASSERT_EQ(arr1.size(), 0);
}

TEST(ArrayTest, AutoResize) {
    Array<Rectangle<double>> arr(2);
    
    for (int i = 0; i < 5; ++i) {
        auto rect = std::make_shared<Rectangle<double>>(
            std::initializer_list<Point<double>>{
                Point<double>(0, 0),
                Point<double>(1, 0),
                Point<double>(1, 1),
                Point<double>(0, 1)
            }
        );
        arr.push_back(rect);
    }
    
    ASSERT_EQ(arr.size(), 5);
    ASSERT_GE(arr.capacity(), 5);
}

// ==================== Integration Tests ====================

TEST(IntegrationTest, ArrayOfDifferentFigures) {
    Array<Figure<double>> figures;
    
    auto rect = std::make_shared<Rectangle<double>>(
        std::initializer_list<Point<double>>{
            Point<double>(0, 0),
            Point<double>(4, 0),
            Point<double>(4, 3),
            Point<double>(0, 3)
        }
    );
    figures.push_back(rect);
    
    auto trap = std::make_shared<Trapezoid<double>>(
        std::initializer_list<Point<double>>{
            Point<double>(0, 0),
            Point<double>(5, 0),
            Point<double>(4, 2),
            Point<double>(1, 2)
        }
    );
    figures.push_back(trap);
    
    auto rhomb = std::make_shared<Rhombus<double>>(
        std::initializer_list<Point<double>>{
            Point<double>(2, 0),
            Point<double>(4, 2),
            Point<double>(2, 4),
            Point<double>(0, 2)
        }
    );
    figures.push_back(rhomb);
    
    ASSERT_EQ(figures.size(), 3);
    
    double total_area = 0.0;
    for (size_t i = 0; i < figures.size(); ++i) {
        total_area += static_cast<double>(*figures[i]);
    }
    
    ASSERT_GT(total_area, 0);
}

TEST(IntegrationTest, TotalAreaCalculation) {
    Array<Figure<double>> figures;
    
    // Прямоугольник 4x3 = 12
    auto rect = std::make_shared<Rectangle<double>>(
        std::initializer_list<Point<double>>{
            Point<double>(0, 0),
            Point<double>(4, 0),
            Point<double>(4, 3),
            Point<double>(0, 3)
        }
    );
    figures.push_back(rect);
    
    // Квадрат 2x2 = 4
    auto square = std::make_shared<Rectangle<double>>(
        std::initializer_list<Point<double>>{
            Point<double>(0, 0),
            Point<double>(2, 0),
            Point<double>(2, 2),
            Point<double>(0, 2)
        }
    );
    figures.push_back(square);
    
    double total = 0.0;
    for (size_t i = 0; i < figures.size(); ++i) {
        total += static_cast<double>(*figures[i]);
    }
    
    ASSERT_NEAR(total, 16.0, 1e-6);
}

TEST(IntegrationTest, IntegerTypeCoordinates) {
    Array<Rectangle<int>> rectangles;
    
    auto rect1 = std::make_shared<Rectangle<int>>(
        std::initializer_list<Point<int>>{
            Point<int>(0, 0),
            Point<int>(3, 0),
            Point<int>(3, 2),
            Point<int>(0, 2)
        }
    );
    rectangles.push_back(rect1);
    
    auto rect2 = std::make_shared<Rectangle<int>>(
        std::initializer_list<Point<int>>{
            Point<int>(1, 1),
            Point<int>(5, 1),
            Point<int>(5, 4),
            Point<int>(1, 4)
        }
    );
    rectangles.push_back(rect2);
    
    ASSERT_EQ(rectangles.size(), 2);
    
    double total_area = 0.0;
    for (size_t i = 0; i < rectangles.size(); ++i) {
        total_area += static_cast<double>(*rectangles[i]);
    }
    
    ASSERT_NEAR(total_area, 18.0, 1e-6); // 6 + 12 = 18
}

// ==================== Main ====================

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}