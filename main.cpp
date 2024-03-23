#include <iostream>
#include <cmath>
#include <functional>

using point = std::vector<double>;


// STRUCT CONTAINING THE INPUT DATA
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

    

    // maximum number of iterarion
    const unsigned int max_iter = 1e4;

    // hyperparameters for armejo method
    double A_alpha0 = 1;
    const double A_sigma = 0.5;

    // hyperparameters for heavy ball method
    double HB_alpha = 1;
    const double HB_eta = 0.9;


} data;


// L2 norm function
double norm_L2(point x){
    double val_norm = 0;
    for(auto x_i : x){
        val_norm += x_i * x_i;
    }
    return std::sqrt(val_norm);
}

// THE GRADIENT DESCENT FUNCTIONS

// Armijo version
point gradient_method_Armijo(
    const std::function<double(point)> f,
    const std::function<point(point)> gradf,
    point x0,
    const double eps_s,
    const double eps_r,
    const unsigned int max_iter,
    double alpha0,
    const double sigma
    )
{
    //initialization parameters
    point x_current= x0;
    
    double dist_x = eps_s + 1.;
    double grad_norm = eps_r + 1.;


    for(unsigned int iter = 0; iter <= max_iter && dist_x > eps_s && grad_norm > eps_r; iter++){
        double alpha_current(alpha0);
        point grad_current = gradf(x_current);
        grad_norm = norm_L2(grad_current);

        //update alpha_k
        for(unsigned int iter2 = 0; iter2 <= max_iter; iter2++){
            point x_temp = {x_current[0] - alpha_current*grad_current[0], x_current[1] - alpha_current*grad_current[1]};
            double lhs_armijo = f(x_current) -f(x_temp);
            double rhs_armijo = sigma * alpha_current * grad_norm*grad_norm;

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

// Heavy-Ball version
// version with alpha_k = alpha_0 for every k
point gradient_method_HeavyBall(
    const std::function<double(point)> f,
    const std::function<point(point)> gradf,
    point x0,
    const double eps_s,
    const double eps_r,
    const unsigned int max_iter,
    double alpha,
    const double eta
    )
{
    //initialization parameters
    point x_current= x0;
    double dist_x = eps_s + 1.;
    double grad_norm = eps_r + 1.;

    point d_current = {-alpha * gradf(x0)[0], - alpha * gradf(x0)[1]};

    for(unsigned int iter = 0; iter <= max_iter && dist_x > eps_s && grad_norm > eps_r; iter++){
        //update minima
        x_current[0] = x_current[0] + d_current[0];
        x_current[1] = x_current[1] + d_current[1];

        //update grad and grad_norm
        point grad_current = gradf(x_current);
        grad_norm = norm_L2(grad_current);

        //updating d
        d_current[0] = eta * d_current[0] - alpha * grad_current[0];
        d_current[1] = eta * d_current[1] - alpha * grad_current[1];
    }

    return x_current;
}






int main(int argc, char** argv)
{
    point result_armijo = gradient_method_Armijo(
        data.f,
        data.gradf,
        data.x0,
        data.eps_s,
        data.eps_r,
        data.max_iter,
        data.A_alpha0,
        data.A_sigma
    );

    point result_hb = gradient_method_Armijo(
        data.f,
        data.gradf,
        data.x0,
        data.eps_s,
        data.eps_r,
        data.max_iter,
        data.HB_alpha,
        data.HB_eta
    );
    
    std::cout << "Initial guess: x1="  << data.x0[0] << ", x2=" << data.x0[1] << std::endl;

    std::cout << "Result Armijo-GD: x1=" << result_armijo[0] << ", x2=" << result_armijo[1] << std::endl;

    std::cout << "Result heavy-ball-GD: x1=" << result_hb[0] << ", x2=" << result_hb[1] << std::endl;
    
    return 0;
}