#include <iostream>
#include <ctime> // time
#include <cstdlib> //ramd
#include <random>
#include <cstdio>
#include <vector> 


using namespace std;


//  ===================================================
//  Quantum DEFINE 

float QUANTUM = 2;  // set on 2, but it can be changed
int N = 0;          // GV number of total processes  



//  ===================================================
//  Process struct definition

struct Process{

    float priority = 0.0,
          burst_time = 0.0,
          waiting_time = 0.0,
          turnaroud_time = 0.0,
          remaining_time = 0.0,
          arrival_time = 0.0;

    int proces_ID = 0;

};


//  ===================================================

// QUEUES Definition


// Queue 1:  System Process having High                   - Priority Round Robin
// Queue 2:  Interactive Process havin Medium Priority 1  - Priority non Preemptive
// Queue 3:  Interactive Process havin Medium Priority 2  - Shortest Job First non Preemprtive
// Queue 4:  Batch Process having Lowest Priority         - First Come First Served (FCFS)


vector<Process> Ready_Queue, // Global variable Ready Queue
                Queue_1,
                Queue_2,
                Queue_3,
                Queue_4,
                Gantt_Queue; 



//  ===================================================
// Defining our funcions and processes

void initProcess();
void printQueueProps(vector<Process> P);
void printQueueJobs(vector<Process> P);
void RR();
void Priority();
void SJF();
void FCFS();
void calcWaitingTime();
void calcTurnAroundTime();




//  ===================================================
//  Functions and Process

void initProcess(){
    cout << "Choose the amount of process :"; 
    cin >> N; // total of processes

    

    for(int i = 0; i  < N; i++){
         
        float random_arrival = 0.0 + (static_cast <float>(rand())/ (static_cast <float>(RAND_MAX))) + i, // sets random arrival time
              random_burst_time = (static_cast <float>(rand())/ (static_cast <float>(RAND_MAX/10)));     // sets random burst time 

        // Random library implementation 
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(0,4), distr2(1,N);

        int   random_priority_queue = distr(gen), // Defines if which Queue belongs
              random_priority_scheduling = distr2(gen); // Define Priority for Priority Algorithms 

        Ready_Queue.push_back(Process()); // Push bacj new Process created with default constructor

        Ready_Queue[i].arrival_time = random_arrival;
        Ready_Queue[i].burst_time   = random_burst_time;   
        if(random_priority_queue == 2)
            Ready_Queue[i].priority = random_priority_scheduling; 
        Ready_Queue[i].proces_ID = i+1;

        // cout <<"\n" << random_arrival
            //  <<"\n" << random_burst_time
            //  <<"\n" << random_priority_queue
            //  <<"\n" << random_priority_scheduling;
    }
}

void printQueueProps(vector<Process> P){

    for(int i = 0 ; i < N; i++){

        cout    << "Job " << P[i].proces_ID << ": " << endl
                << "| Arrival Time: "    <<   P[i].arrival_time << endl
                << "| Burst Time: "      <<   P[i].burst_time << endl
                << "| Priority: "        <<   P[i].priority << endl
                << "| Turnaround Time: " <<   P[i].turnaroud_time << endl
                << "| Waiting Time: "    <<   P[i].waiting_time << endl
                << "| Remaining Time: "  <<   P[i].remaining_time << endl; 
    }
}

void printQueueJobs(vector<Process> P){

    for(int i = P.size() - 1 ; i >= 0; i--){
        cout << "| Job " << P[i].proces_ID << " ";
    }
    cout << "|" << endl;
}



//  ===================================================
//  Main 

int main(){
    

    srand(static_cast <unsigned> (time(0)));


    int new_quantum;
    cout << "Set Quantum (Default = 2, input = 0 means no change):";
    cin >> new_quantum;

    if(new_quantum != 0 )
        QUANTUM = new_quantum;

    cout << QUANTUM << endl;
    
    initProcess();

    cout << "Processes:" << endl << endl;
    printQueueProps(Ready_Queue);

    cout << "Jobs:" << endl << endl;
    printQueueJobs(Ready_Queue);
    
    return 0;
}