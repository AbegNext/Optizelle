#include "peopt/peopt.h"
#include "peopt/vspaces.h"
#include "peopt/json.h"
#include <iostream>
#include <iomanip>

// Optimize a simple optimization problem with an optimal solution of (1/3,1/3)

// Squares its input
template <typename Real>
Real sq(Real x){
    return x*x; 
}

// Define a simple objective where 
// 
// f(x,y)=(x+1)^2+(y+1)^2
//
struct MyObj : public peopt::ScalarValuedFunction <double,peopt::Rm> {
    typedef peopt::Rm <double> X;
    typedef double Real;

    // Evaluation 
    double operator () (const X::Vector& x) const {
        return sq(x[0]+Real(1.))+sq(x[1]+Real(1.));
    }

    // Gradient
    void grad(
        const X::Vector& x,
        X::Vector& g
    ) const {
        g[0]=2*x[0]+2;
        g[1]=2*x[1]+2;
    }

    // Hessian-vector product
    void hessvec(
        const X::Vector& x,
        const X::Vector& dx,
        X::Vector& H_dx
    ) const {
        H_dx[0]= Real(2.)*dx[0]; 
        H_dx[1]= Real(2.)*dx[1]; 
    }
};

// Define simple inequalities 
//
// g(x,y)= [ x + 2y >= 1 ] 
//         [ 2x + y >= 1 ] 
//
struct MyIneq
    : public peopt::VectorValuedFunction <double,peopt::Rm,peopt::Rm>
{
    typedef peopt::Rm <double> X;
    typedef peopt::Rm <double> Y;
    typedef double Real;

    // y=f(x) 
    void operator () (
        const X::Vector& x,
        Y::Vector& y
    ) const {
        y[0]=x[0]+Real(2.)*x[1]-Real(1.);
        y[1]=Real(2.)*x[0]+x[1]-Real(1.);
    }

    // y=f'(x)dx
    void p(
        const X::Vector& x,
        const X::Vector& dx,
        Y::Vector& y
    ) const {
        y[0]= dx[0]+Real(2.)*dx[1];
        y[1]= Real(2.)*dx[0]+dx[1];
    }

    // z=f'(x)*dy
    void ps(
        const X::Vector& x,
        const Y::Vector& dy,
        X::Vector& z
    ) const {
        z[0]= dy[0]+Real(2.)*dy[1];
        z[1]= Real(2.)*dy[0]+dy[1];
    }

    // z=(f''(x)dx)*dy
    void pps(
        const X::Vector& x,
        const X::Vector& dx,
        const Y::Vector& dy,
        X::Vector& z
    ) const {
        X::zero(z);
    }
};

int main(){
    // Create a type shortcut
    using peopt::Rm;

    // Generate an initial guess for the primal
    std::vector <double> x(2);
    x[0]=2.1; x[1]=1.1;

    // Generate an initial guess for the dual
    std::vector <double> z(2);
    z[0]=1.; z[1]=1.;

    // Create an optimization state
    peopt::InequalityConstrained <double,Rm,Rm>::State::t state(x,z);

    // Read the parameters from file
    peopt::json::InequalityConstrained <double,peopt::Rm,peopt::Rm>::read(
        peopt::Messaging(),"simple_inequality.peopt",state);
    
    // Create a bundle of functions
    peopt::InequalityConstrained <double,Rm,Rm>::Functions::t fns;
    fns.f.reset(new MyObj);
    fns.h.reset(new MyIneq);

    // Solve the optimization problem
    peopt::InequalityConstrained <double,Rm,Rm>::Algorithms
        ::getMin(peopt::Messaging(),fns,state);

    // Print out the reason for convergence
    std::cout << "The algorithm converged due to: " <<
        peopt::StoppingCondition::to_string(state.opt_stop) << std::endl;

    // Print out the final answer
    const std::vector <double>& opt_x=*(state.x.begin());
    std::cout << std::scientific << std::setprecision(16)
        << "The optimal point is: (" << opt_x[0] << ','
	<< opt_x[1] << ')' << std::endl;
}