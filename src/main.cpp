#include <iostream>
#include <memory>
#include <vector>
#include "utils/out/CTable.h"
#include <SFML/System.hpp>
#include <algorithm>
#include "methods/approximation/impl/Exponential.h"
#include "graph/draw/IShape.h"
#include "graph/draw/Axis.h"
#include "graph/draw/AxisNumbers.h"
#include "graph/draw/Grid.h"
#include "graph/draw/FuncGraph.h"
#include "graph/draw/Points.h"
#include "graph/draw/Legend.h"
#include "methods/approximation/ApproximationManager.h"
#include <mutex>
#include <SFML/Window.hpp>
#include <queue>


std::queue<sf::Event> eventQueue;
std::mutex eventMutex;
void workerThread()
{
    std::vector<std::pair<double, double>> points = {{1.2, 7.4},
                                                     {2.9, 9.5},
                                                     {4.1, 11.1},
                                                     {5.5, 12.9},
                                                     {6.7, 14.6},
                                                     {9.2,18.2},
                                                     {10.3, 20.7}
    };
    std::unique_ptr<approx::IApproximation> linear = std::make_unique<approx::Linear>(points);
    linear->fit();
    linear->get_info();
    linear->get_extras_info();
    // Push an event to the queue
    sf::Event event;
    event.type = sf::Event::EventType::Closed;
    {
        std::lock_guard<std::mutex> lock(eventMutex);
        eventQueue.push(event);
    }
}

int main() {
//    std::vector<std::pair<double, double>> points;
//    std::cout << "Enter number of points: ";
//    int n;
//    std::cin >> n;
//    for (int i = 0; i < n; i++) {
//        std::cout << "Enter x" << i + 1 << " and y" << i + 1 << " : ";
//        double x, y;
//        std::cin >> x >> y;
//        points.emplace_back(x, y);
//    }
//    std::vector<std::vector<double>> A = {{-10, -7, 0}, {-3, 2, 6}, {5, -1, 5}};
//    std::vector<double> b = {7, 4, 6};
//    std::vector<double> res = GaussianElimination::solve(A, b);
//    for (int i = 0; i < res.size(); i++) {
//        std::cout << "x" << i + 1 << " = " << res[i] << std::endl;
//    }
//    std::vector<int> row1{1, 2, 3266};
//    std::vector<int> row2{4, 5, 6};
//    std::vector<int> row3{7, 8, 9};
//    std::vector<std::string> header = {"x1", "x2", "x3"};
//    size_t t = 3;
//    auto mat = Matrix<int,3,3>::make_matrix({row1, row2, row3});
//    std::pair<Matrix<int,3,3>, std::vector<std::string>> p = std::make_pair(mat.T(), header);
//    ctable << p;

//    std::vector<std::pair<double, double>> points = {{1.2, 7.4},
//                                                     {2.9, 9.5},
//                                                     {4.1, 11.1},
//                                                     {5.5, 12.9},
//                                                     {6.7, 14.6},
//                                                     {9.2,18.2},
//                                                     {10.3, 20.7}
//                                                     };
    std::vector<std::pair<double, double>> points = {{1.1, 2.73},
                                                     {2.3, 5.12},
                                                     {3.7, 7.74},
                                                     {4.5, 8.91},
                                                     {5.4, 10.59},
                                                     {6.8,12.75},
                                                     {7.5, 13.43}
    };
    std::vector<std::unique_ptr<IShape>> shapes;
    shapes.push_back(std::make_unique<Axis>(3));
    shapes.push_back(std::make_unique<AxisNumbers>());
    shapes.push_back(std::make_unique<Grid>(1));
    ApproximationManager manager(points);
    Legend legend;
    legend.addEntry("Points", sf::Color::Red);
    std::vector<sf::Color> colors = {sf::Color(156, 89, 0), sf::Color(5, 156, 0), sf::Color::Blue, sf::Color(225,167,18), sf::Color::Magenta, sf::Color::Cyan};
    for (auto &approx : manager.getApproximations()) {

        std::string str = approximation_str[approx.first];
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        std::cout <<std::string(10, '-') << str << std::string(10, '-')<< std::endl;
        approx.second->fit();
        approx.second->get_info();
        approx.second->get_extras_info();
        shapes.push_back(std::make_unique<FuncGraph>(approx.second->getF(), approx.second->getParams(), 10, colors[approx.first]));
        legend.addEntry(approximation_str[approx.first], colors[approx.first]);
    }
    shapes.push_back(std::make_unique<Legend>(legend));
    sf::Color customBlue(100, 150, 255);
    shapes.push_back(std::make_unique<Points>(points, sf::Color::Red));
    MainWindow::getInstance().setShapes(shapes);



//    sf::Thread thread(&workerThread);
//    thread.launch();
    MainWindow::getInstance().display();

    return 0;

}
