/**

 Author:       Noam Grebler
 Date:         March 2nd, 2016.
 Description:  This program simulates a line at a bank.
 
 Additional comments:
 +-------------------------------------------------------+
 | Waiting times for different Probabilities and Tellers |
 +-------------------------------------------------------+
        | T = 1 | T = 2 | T = 3 | T = 4 | T = 5 |
 P = 10 |9.62963|1.15625|      0|      0|      0|
 P = 20 |118.616| 7.4717|0.60360|0.03810|0.00901|
 P = 30 |336.158|67.0361|2.39877|0.75817|0.10256|
 P = 40 |683.968|185.964|27.0605|3.18386|   0.26|
 P = 50 |845.779|317.226|138.544|22.9766|1.66784|
 ---------------------------------------------------------
 */

#include <iostream>	// std::cout, std::endl, std::istream
#include <fstream>	// std::ifstream
#include <string>// std::string
#include <ctime>
#include <cstdlib>
#include "queue.h"

using namespace Pic10B;  // A call to queue<T> is a call to Pic10B::queue<T> instead of std::queue<T>
/** Could also specify we only want Pic10B::queue like so:
 using Pic10B::queue;
 */

/**
 Definition of the 'Cliente' class.
 */

using std::cout;
using std::cin;
using std::ostream;
using std::string;
using std::endl;

class Cliente {
public:
    inline Cliente( std::string n = "Default_client" , int a = 0 , int s = 0 )
    : name(n) , arrivalTime(a) , serviceTime(s) { }
    
    
    Cliente(int a , int b, int i);
    Cliente (bool x);
    
    // Accessors
    inline std::string get_name() const { return name; }
    inline int get_arrival_time() const { return arrivalTime; }
    inline int get_service_time() const { return serviceTime; }
    
    void counter_set(int x) {counter_num = x; }
    int get_counter (){return counter_num;}
    void set_leaving_time (int x) { leaving_time = x;}
    int get_leaving_time(){return leaving_time;}
    bool get_empty () {return empty;}
    
    friend std::istream& operator>>( std::istream& in , Cliente& rhs ) ;
    
    
private:
    std::string name;
    int arrivalTime;
    int serviceTime;
    int counter_num;
    int leaving_time;
    bool empty = false;
};



Cliente::Cliente(int a , int b, int i){
    arrivalTime = a;
    serviceTime = b;
    name = "Random_customer_" + std::to_string(i);
    empty  = false;
}

Cliente::Cliente(bool x){
    empty = x;
}

/**
 Non-member functions
 */
std::istream& operator>>( std::istream& in , Cliente& rhs ) {
    in >> rhs.name >> rhs.arrivalTime >> rhs.serviceTime;
    return in;
}



/*
 Prints the time on the screen. It assumes we are given
 the number of minutes after 9:00am.
 */
void print_time( int minsAfterNine ) {
    const int MIN_PER_HOUR = 60;
    
    int hour = 9 + minsAfterNine / MIN_PER_HOUR;
    int min = minsAfterNine % MIN_PER_HOUR;
    
    if (hour <= 12) std::cout << hour << ":";
    else std::cout << hour - 12 << ":";
    
    if (min > 9) std::cout << min;
    else std::cout << "0" << min;
    
    if (hour <= 12) std::cout << "am";
    else std::cout << "pm";
    
    return;
}

void fill_data(queue<Cliente>& customerList, Cliente& newCustomer, int& i)
{
    string file;
    char c;
   
    
    cout<< "Would you like to upload the data from a file or generate random data? f for file or r for random: ";
    cin >>c;
    cout << endl;
    
    
   
    
    if (c=='f' || c== 'F')
    {
        cout << "What file would you like to upload the data from? ";
        cin >> file;
        cout<< endl;
        
        std::ifstream fin;
        fin.open("clientes.txt");
        
        while ( fin >> newCustomer ){
            customerList.enter(newCustomer);
            i++;
        }
        fin.close();
        
        if ( customerList.is_empty() )
        {
            std::cout << "ERROR: Could not read file 'clientes.txt'." << std::endl;
         
        }
        
    }
    
    else if (c=='r' || c== 'R')
    {
        srand( (int)time(0) );
        int probs;
        int service_time;
        int a=0;
        
        
        cout << "What is the probability that a customer enters each minute? ";
        cin >> probs;
        
       
        
      while(a<=540)
      {
           service_time = rand() % 14 + 2;
          
          if((rand() % 100 + 1)<=probs)
          {//create a client give the minutes a and the waiting time service_time;
              Cliente s(a,service_time ,i);
              customerList.enter(s);
              a++;
              i++;
          }
          else
          {
               a++;
          }
      }
    }
    
    
    else
    {
        fill_data( customerList,newCustomer,i);
    }
}

bool is_counter_empty(std::vector<Cliente> a)
{
    int count = 0;
    for (int j = 0; j<a.size() ; j ++)
    {
        if(a[j].get_empty()==false)
        {
            count++;
            
        }
    }
    
    
    if(count == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 main routine that simulates a 'cola' (line) at a bank
 */
int main() {
    /**
     Read customer file.
     Assume the format is:
     name arrivalTime serviceTime
     and that the list is sorted based on arrival times.
     */
    
    
    queue<Cliente> customerList;
    Cliente newCustomer;
    int num_customers = 1 ;
    double average_time_spent = 0;
    int moment_pop = 0;
    int max_customers = 1;
    int time_spent = 0;
    double total_time_spent = 0;
    int num_counters = 1;
    std::vector<int> line_time;
    
    
    fill_data(customerList,newCustomer, num_customers );
    
   
    
    
    if ( customerList.is_empty() )
    {
        return 1; // Standard practice is to NOT RETURN 0 if an error occurs.
    }
    
    cout << endl << "How many counters are there? ";
    cin >>num_counters;
    cout << endl;
    
    if(num_counters<=0)
    {
        return 1;
    }
    
    std:: vector<Cliente> counters(num_counters);
    
    /**
     First customer steps up to the counter immediately (i.e., no waiting occurs).
     Clock starts running at this arrival time.
     */
   
    
    Cliente atCounter = customerList.leave();
    int currentTime = atCounter.get_arrival_time();
    int startService = atCounter.get_arrival_time();
    
    
    print_time(currentTime);
    std::cout << "  " << atCounter.get_name() << " enters the bank." << std::endl;
    print_time(currentTime);
    std::cout << "  " << atCounter.get_name() << " steps up to counter 1." << std::endl;
    
    counters[0] = atCounter;
    counters[0].set_leaving_time(counters[0].get_arrival_time() + counters[0].get_service_time());
    
    for(int k = 1; k < counters.size(); k++)
    {
        Cliente a(true);
        counters[k]=a;
    }
    
    // set up our empty 'cola'
    queue<Cliente> line;
    
    // while there are customers in bank... or they are yet to arrive.
    while ( !customerList.is_empty() || !line.is_empty() || !is_counter_empty(counters)  ){
        
        // Check if a 'Cliente' enters the 'cola'.
        if ( !customerList.is_empty() && customerList.peek().get_arrival_time() <= currentTime ){
            print_time(currentTime);
            std::cout  << "  " << customerList.peek().get_name()
            << " enters the bank." << std::endl;
            line.enter( customerList.leave() );
            moment_pop++;
        }
        
        // Check if a 'Cliente' leaves the counter.
        
            
            
            Cliente v(true);
            
            for (int i = 0 ; i  < counters.size() ; i++ )
            {
                if (counters[i].get_leaving_time() <= currentTime && !counters[i].get_empty())
                {
                    print_time(currentTime);
                    std::cout << "  " << counters[i].get_name()
                    << " leaves the counter " << i+1 << std::endl;

                    
                    
                    counters[i] = v;
             
                }
                
            }
        

        
        // Check if counter is empty and someone is waiting.
        if ( !line.is_empty() ){
            startService = currentTime;
            
             for (int i = 0 ; i  < counters.size() ; i++ )
             {
                 if(counters[i].get_empty() && !line.is_empty())
                 {
                     counters[i]=line.leave();
                     counters[i].set_leaving_time(currentTime + counters[i].get_service_time());
                     counters[i].counter_set(i);
                     
                     time_spent = currentTime - counters[i].get_arrival_time();
                     line_time.push_back(time_spent);
                     
                     print_time(currentTime);
                     cout << "  " << counters[i].get_name()
                     << " steps up to the counter " << i+1 << std::endl;
                     moment_pop--;
                                      }
             }
            
        }
        
        ++currentTime;
    
        
        if (moment_pop > max_customers)
        {
            max_customers = moment_pop;
        }
    }
    
    for(int j = 0 ; j<line_time.size() ; j++)
    {
         total_time_spent += line_time[j]; //ading the total time that people have spent in line.
    }
    
    
    average_time_spent =  total_time_spent / (num_customers-1); // Calculating the average time spent
    
     cout << endl << endl << "The total number of customers served is: " << num_customers-1 << endl;
    
     cout << endl << endl << "The average time spent in line is: " << average_time_spent << " minutes." << endl;
    
     cout << endl << endl << "The maximum number of customer in line at the same time is: " << max_customers << endl;
    
    return 0;
}
