#include <iostream>
#include <cmath>
#include <functional>

// STRUCT CONTAINING THE INPUT DATA

using point = std::vector<double>;

struct {
    // function
    const std::function<double(point)> f = 
        [](point x){return x[0]*x[1] + 4*std::pow(x[0], 4) + pow(x[1], 2) + 3 * x[0];};

    // gradient function
    const std::function<point(point)> gradf =
    [](point x){
        point val = {
            x[1] + 16*std::pow(x[0], 3) + 3,
            x[0] + 2*x[1]
            };
        return val;
    };

    // initial guess
    point x0 = {2.,2.};
    
    // tolerances for
    // - the step lenght
    const double eps_s = 1e-6;
    // - the residual (of the gradient)
    const double eps_r = 1e-6;

    // initial step
    double alpha0 = 1;

    // maximum number of iterarion
    const unsigned int max_iter = 1e4;

    // factor hyperparameter
    const double sigma = 0.5;
} data;


double norm_pow2(point x){
    double val_norm = 0;
    for(auto x_i : x){
        val_norm += x_i * x_i;
    }
    return val_norm;
}

// THE GRADIENT DESCENT FUNCTION
point gradient_method(
    const std::function<double(point)> f,
    const std::function<point(point)> gradf,
    point x0,
    const double eps_s,
    const double eps_r,
    double alpha0,
    const unsigned int max_iter,
    const double sigma
    )
{

    //initialization parameters
    point x_current= x0;
    double dist_x = 1e10;
    double grad_norm = 1e10;

    for(unsigned int iter = 0; iter <= max_iter && dist_x > eps_s && grad_norm > eps_r; iter++){
        double alpha_current(alpha0);
        point grad_current = gradf(x_current);
        grad_norm = norm_pow2(grad_current);

        //update alpha_k
        for(unsigned int iter2 = 0; iter <= max_iter; iter2++){
            point x_temp = {x_current[0] - alpha_current*grad_current[0], x_current[1] - alpha_current*grad_current[1]};
            double lhs_armijo = f(x_current) -f(x_temp);
            double rhs_armijo = sigma * alpha_current * grad_norm;

            if(lhs_armijo >= rhs_armijo){
                break;
                
            }
            else{
                alpha_current = alpha_current/2;
            }
        }

        //update minima
        x_current[0] = x_current[0] - alpha_current * grad_current[0];
        x_current[1] = x_current[1] - alpha_current * grad_current[1];

    }

    return x_current;
}



int main(int argc, char** argv)
{
    point result_armijo = gradient_method(
        data.f,
        data.gradf,
        data.x0,
        data.eps_s,
        data.eps_r,
        data.alpha0,
        data.max_iter,
        data.sigma
    );
    
    std::cout << "Initial guess: x1="  << data.x0[0] << ", x2=" << data.x0[1] << std::endl;

    std::cout << "Result Armijo-GD: x1=" << result_armijo[0] << ", x2=" << result_armijo[1] << std::endl;
    
    return 0;
}