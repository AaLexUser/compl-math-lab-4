//
// Created by Alexey Lapin on 4/13/23.
//

#include "AbstractApproximation.h"

approx::AbstractApproximation::AbstractApproximation(const std::function<double(double, std::vector<double>)> &f,
                                             const std::vector<std::pair<double, double>> &points) : _f(f),
                                                                                                     _points(points) {}


void approx::AbstractApproximation::set_s() {
    double S = 0;
    if(_params.empty()){
        return;
    }
    for(auto point : _points) {
        double test = _f(point.first, _params);
        S+=pow(_f(point.first, _params) - point.second, 2);
    }
    _extras["Minimization criterion"] = S;
}

void approx::AbstractApproximation::set_standard_deviation() {
    if(_extras.find("Minimization criterion") == _extras.end()){
        set_s();
    }
    _extras["Standard Deviation"] = sqrt(_extras["Minimization criterion"] / (_points.size()));
}

std::pair<std::vector<std::vector<std::string>>, std::vector<std::string>>
 approx::AbstractApproximation::get_info() {
    std::vector<std::vector<std::string>> res(_points.size());
    for(int i = 0; i < _points.size(); ++i) {
        res[i].push_back(std::to_string(static_cast<int>(i + 1)));
        res[i].push_back(std::to_string(_points[i].first));
        res[i].push_back(std::to_string(_points[i].second));
        res[i].push_back(std::to_string(_f(_points[i].first, _params)));
        res[i].push_back(std::to_string(_f(_points[i].first, _params) - _points[i].second));
    }
    return std::make_pair<std::vector<std::vector<std::string>> &, std::vector<std::string>>(res,  {"N p.p.", "xi", "yi", "fi", "ei"});
}

double approx::AbstractApproximation::get_extra_info(std::string key) {
    return _extras[key];
}

std::pair<std::vector<std::vector<std::string>>, std::vector<std::string>>
    approx::AbstractApproximation::get_extras_info() {
    std::vector<std::vector<std::string>> res(_extras.size());
    int i = 0;
    for(auto extra : _extras) {
        res[i].push_back(extra.first);
        res[i].push_back(std::to_string(extra.second));
        ++i;
    }
    for(int i = 0; i < _params.size(); ++i) {
        res.push_back({"a" + std::to_string(i), std::to_string(_params[i])});
    }
    return std::make_pair<std::vector<std::vector<std::string>> &, std::vector<std::string>>(res,  {"Key", "Value"});
}

const std::function<double(double, std::vector<double>)> & approx::AbstractApproximation::getF() const {
    return _f;
}

const std::vector<double> &approx::AbstractApproximation::getParams() const {
    return _params;
}

double approx::AbstractApproximation::pair_sum(const std::vector<std::pair<double, double>> array, int powNum, bool choice) const {
    double sum = 0;
    for(auto& el : array) {
        if(choice) {
            sum += pow(el.first, powNum);
        } else {
            sum += pow(el.second, powNum);
        }
    }
    return sum;
}

double approx::AbstractApproximation::two_pair_sum(const std::vector<std::pair<double, double>> array, int powNum1,
                                                   int powNum2) const {
    double sum = 0;
    for(auto& el : array) {
        sum += pow(el.first, powNum1) * pow(el.second, powNum2);
    }
    return sum;
}
