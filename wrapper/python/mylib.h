#include <Eigen/Dense>
#include <cmath>

#include <iostream>

using Eigen::Matrix, Eigen::Dynamic;
typedef Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> myMatrix;

class MyClass {

    int N;
    double a;
    double b;

public:

    Eigen::VectorXd v_data;
    Eigen::VectorXd v_gamma;

    MyClass(){}
    MyClass( double a_in, double b_in, int N_in) 
    {
        N = N_in;
        a = a_in;
        b = b_in;
    }

    void run() 
    { 
        v_data = Eigen::VectorXd::LinSpaced(N, a, b); 

        auto gammafunc = [](double it) { return std::tgamma(it); };
        v_gamma = v_data.unaryExpr(gammafunc);
    }

    void printInfo() const {
        std::cout << "hello" << std::endl;
    }

    const std::string & to_string() const {
        return m_name;
    }

    const std::string &getName() const { return m_name; }

    std::string m_name = "hello";
};

class Pet {
    std::string name;
    std::string m_name = "hello";
  public:
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }
//    const std::string &getName() const { return m_name; }

};
