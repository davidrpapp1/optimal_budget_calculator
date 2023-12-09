/*
Using a Monte Carlo algorithm the same objective as in "optimal_stock.cpp" can be achieved. The key differences
are three fold.

1. Stock quantity guesses are allowed to happen in bulk, dictated by a tunable sigma value

2. Stock quantities are adjusted randomly, meaning only either 1, 2, 3, etc stock quantities will be
   increased at a given timestep.

3. The implementation of a seperate algorithm that displays the nearest total estimate that can be achieved,
   given the provided total's condition can not be met.
*/

// Required libraries
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <bits/stdc++.h>
#include <unistd.h>

// input_init -------------------------------------------------------------
std::vector <double> stock_init;
int stock_init_size;
int stock_size_check;
double stock_container;
bool input_init_flag = false;
int number;
double budget;
double tolerance;
int iterations;

// Initializes all stock options via user input
std::vector <double> input_init(){

    // User message to input number of stocks
    std::cout << "Number of stock types: ";

    // Assign value to stock_init_size
    std::cin >> stock_init_size;

    // User message to input stock prices
    std::cout << "Input stock prices, seperated by enters." << std::endl;

    // Fill stock price vector
    stock_size_check = 0;
    while (stock_size_check < stock_init_size){

        // Request to fill stock_container until the size condition has been met
        std::cin >> stock_container;
        stock_init.push_back(stock_container);

        // Grow size stock size flag;
        stock_size_check++;

    }

    // User message to input total number of stocks purchasable
    std::cout << "Please enter maximum number of purchasable stocks: ";

    // Assign user input number
    std::cin >> number;

    // User message to input budget for purchases
    std::cout << "Please enter purchasing budget: ";

    // Assign user input budget
    std::cin >> budget;

    // User message to input budget tolerance (as a %)
    std::cout << "Please input budget tolerance, as a percentage: ";

    // Assign user input to tolerance
    std::cin >> tolerance;

    // User message to input number of iterations required
    std::cout << "Number of iterations required: ";

    std::cin >> iterations;

    // Display successfull initialization message
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "Initialization success with " << stock_init.size() << " stock option(s)." << std::endl;
    std::cout << "Total number of purchasable stocks: " << number << std::endl;
    std::cout << "Total budget: " << budget << std::endl;
    std::cout << "Budget tolerance %: " << tolerance << std::endl;
    std::cout << "Number of iterations required: " << iterations << std::endl;

    // Return success flag
    input_init_flag = true;

    // Return filled stock vector
    return stock_init;
}

// End of input_init ------------------------------------------------------


// sigma_calculation ------------------------------------------------------
std::vector <double> sigma_values;
double stock_sum;
double stock_average;
bool sigma_calculation_flag = false;

// Calculates the average of provided stock options
std::vector <double> sigma_calculation(std::vector <double> stock){

    // Calculate assumed average to adjust sigma values
    stock_average = budget / number;

    // Calculate all sigma values and place into sigma_values vector
    sigma_values.resize(stock.size());
    for(int i=0; i<stock.size(); i++){
        sigma_values[i] = stock_average / stock[i];
    }

    // Display success message
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << "Successfully calculated sigma value(s)." << std::endl;

    // Return success flag
    sigma_calculation_flag = true;

    // Return sigma_values vector
    return sigma_values;

}

// End of sigma_calculation -----------------------------------------------


// main -------------------------------------------------------------------
std::vector <double> stock;
std::vector <double> sigma;
std::vector <int> stock_quantity_unfiltered;
std::vector <int> stock_quantity;
std::vector <int> stock_quantity_valid;
std::vector <double> stock_quantity_adjust;
bool exit_flag = false;
double total_cost;
std::vector <double> total_cost_options;
std::vector <double> total_cost_unfiltered;
int count;
int iteration_count = 0;
std::vector <int> super_index;
bool options_found = false;
double found;
int incomplete_count = 0;
int cost_index;
int options_found_count = 0;
int stock_quantity_validator = 0;
std::vector <int> stock_quantity_temp;
std::vector<int>::iterator scan;
double stock_quantity_cost = 0.0;
int solution_count = 0;

// Random number declarations
std::random_device rd;
std::mt19937 generator(rd());
std::uniform_real_distribution<double> distribution(0.0, 1.0);
std::normal_distribution<double> gaussian(0.5, 0.1666667);
double p;

// Main function containing calculation algorithm
int main(){

    // Perform input initialization routine 
    if(input_init_flag==false){
        stock = input_init();
        stock_quantity.resize(stock.size(), 0);
        stock_quantity_adjust.resize(stock_quantity.size(), 0.5);
        stock_quantity_temp.resize(stock_quantity.size(), 0);
    }

    // Perform sigma calculation routine 
    if(sigma_calculation_flag==false){
        sigma = sigma_calculation(stock);
        std::cout << "Computing..." << std::endl;
    }

    // Remain in loop until exit_flag is turned to true
    do{

        // Determine which stock options to increase
        for (int i=0; i<stock_quantity.size(); i++){

            // Check to increase stock option at all
            p = distribution(generator);
            
            if(p>=stock_quantity_adjust[i]){

                // Add one from stock quantity if probability is satisfied, take one away otherwise
                stock_quantity[i]++;

                // Decrease adaptive probability comparator if stock quantity increases
                stock_quantity_adjust[i] = stock_quantity_adjust[i] + (stock_quantity_adjust[i]/(stock[i]*iteration_count));
         
            } else{

                stock_quantity[i]--;

                stock_quantity_adjust[i] = stock_quantity_adjust[i] - (stock_quantity_adjust[i]/(stock[i]*iteration_count));

                // Ensure boundary at quantity = 0
                if(stock_quantity[i]<0){
                    stock_quantity[i]=0;
                }

            }

        }

        // Calculation of total_cost
        total_cost = 0.0;
        for(int i=0; i<stock.size(); i++){
            total_cost += stock[i] * stock_quantity[i];
        }

        // Store total_cost and stock_quantity in vector for future use
        count = 0;
        for(int i=0; i<stock_quantity.size(); i++){
            count += stock_quantity[i];
        }

        // If count condition is satisfied
        if(count == number){

            total_cost_unfiltered.push_back(total_cost);

            for(int i=0; i<stock_quantity.size(); i++){
                stock_quantity_unfiltered.push_back(stock_quantity[i]);
            }

            // Reset stock quantities after storing
            std::fill(stock_quantity.begin(), stock_quantity.end(), 0);
            std::fill(stock_quantity_adjust.begin(), stock_quantity_adjust.end(), 0.5);

            // Increase number of iterations completed
            iteration_count++;
            
        } else if(count > number){

            // Reset stock quantities if number purchased overruns
            std::fill(stock_quantity.begin(), stock_quantity.end(), 0);
            std::fill(stock_quantity_adjust.begin(), stock_quantity_adjust.end(), 0.5);

        } else{

            // Add one to incomplete counter to prevent recursive looping
            incomplete_count++;

        }

        if(incomplete_count==10000){

            // Reset stock quantities if recursive loop detected
            std::fill(stock_quantity.begin(), stock_quantity.end(), 0);
            std::fill(stock_quantity_adjust.begin(), stock_quantity_adjust.end(), 0.5);

        }

        // Exit once number of iterations have been met
        if(iteration_count == iterations){
            exit_flag = true;
        }

    } while(exit_flag==false);

    // Post calculation analysis
    for(int i=0; i<total_cost_unfiltered.size(); i++){

        // Analysis of satisfactory total_cost options
        if((total_cost_unfiltered[i]<=budget+((budget/100)*tolerance)) && (total_cost_unfiltered[i]>=budget-((budget/100)*tolerance))){

            // Mark viable options in super_index
            super_index.push_back(i);

            // Turn options_found flag on
            options_found = true;

            // Increase options flag
            options_found_count++;

        } 

    }

    if(options_found==true){

        // Resize to avoid segmentation fault
        super_index.resize(options_found_count);

    }

    // Storage of relevant stock information based on super_index
    for(int i=0; i<super_index.size(); i++){

        for(int n=super_index[i]*stock_quantity.size(), c=0; n<(super_index[i]*stock_quantity.size())+stock_quantity.size(); n++, c++){

            // Store local stock_quantity values in container
            stock_quantity_temp[c] = stock_quantity_unfiltered[n];

            stock_quantity_validator++;

            // Identify if duplicate and skip if so
            if(stock_quantity_validator==stock_quantity.size()){

                // Scan validated vector for duplicate entries
                scan = std::search(stock_quantity_valid.begin(), stock_quantity_valid.end(), stock_quantity_temp.begin(), stock_quantity_temp.end());

                if(scan!=stock_quantity_valid.end()){

                    // Previously found combination so do nothing

                } else{

                    for(int c=0; c<stock_quantity_temp.size(); c++){

                        // Store stock quantity in validated vector
                        stock_quantity_valid.push_back(stock_quantity_temp[c]);

                    }

                }

                stock_quantity_validator = 0;

            }
        
        }

    }

    // If no suitable total_cost matches are found, print out nearest number to budget
    if(options_found==false){

        // Subtract all costs obtained from budget, and sort vector to obtain the smallest difference
        for(int i=0; i<total_cost_unfiltered.size(); i++){
            total_cost_unfiltered[i] = abs(total_cost_unfiltered[i]-budget);
        }

        std::sort(total_cost_unfiltered.begin(), total_cost_unfiltered.end());

        found = total_cost_unfiltered[0];
    
    }

    // Output gathered statistics to user
    if(options_found==true){
        std::cout << "Calculations completed with " << iteration_count << " iteration(s)." << std::endl;

        // Compute number of solutions found
        for(int i=0; i<stock_quantity_valid.size(); i=i+stock_quantity.size()){

            solution_count++;

        }

        std::cout << "Option(s) found: " << solution_count << std::endl;

        for(int i=0, k=1; i<stock_quantity_valid.size(); i=i+stock_quantity.size(), k++){
            
            // Formatting
            if(i==0){
            
                std::cout << "-----------------------------" << std::endl;

            }

            // Option index
            std::cout << "Option: " << k << std::endl;

            // Output all valid stock count options
            for(int n=i, c=0; n<i+stock_quantity.size(); n++, c++){

                // Output valid stock options
                std::cout << "Stock " << c+1 << ": " << stock_quantity_valid[n] << std::endl;

                // Calculate stock_quantity_cost
                stock_quantity_cost += stock_quantity_valid[n] * stock[c];
                
            }

            // Output stock_quantity_cost and reset for next iteration
            std::cout << "Total cost: " << stock_quantity_cost << std::endl;
            stock_quantity_cost = 0.0;

            // Formatting
            std::cout << "-----------------------------" << std::endl;

        }
    
    // Output closest budget estimate
    } else{

        std::cout << "No suitable result found within tolerance." << std::endl;
        std::cout << "Nearest purchase estimate calculated: " << found << std::endl;

    }

}

// End of main ------------------------------------------------------------