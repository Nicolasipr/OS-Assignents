#include <iostream>
#include <ctime> // time
#include <cstdlib> //ramd
#include <random>
#include <cstdio>
#include <vector> 


using namespace std;


//  ===================================================
//  GLOBAL VARIABLES 

float QUANTUM = 2;  // set on 2, but it can be changed
int N = 0;          // GV number of total processes  



//  ===================================================
//  Process struct definition

struct Process{

    float priority = 0.0,
          priority_type = 0.0,
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
void queueSortingHandler(vector<Process> P);
void copyJobToNewQueue(vector<Process> P, vector<Process> &New_Q, int i, int j);
void queueActivationHandler();
void RR();
void Priority();
void SJF();
void FCFS();
void calcWaitingTime();
void calcTurnAroundTime();
void sortMerge(vector<Process> &P);
void mergeHalves(vector<Process> &P,vector<Process> L, vector<Process> R);


//  ===================================================
//  Functions and Useful handlers

void initProcess(){
    cout << "Choose the amount of process :"; 
    cin >> N; // total of processes
  

    for(int i = 0; i  < N; i++){
         
        float random_arrival = 0.0 + (static_cast <float>(rand())/ (static_cast <float>(RAND_MAX))) + i, // sets random arrival time
              random_burst_time = (static_cast <float>(rand())/ (static_cast <float>(RAND_MAX/10)));     // sets random burst time 

        // Random library implementation 
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(1,4), distr2(1,N);

        int   random_priority_queue = distr(gen), // Defines if which Queue belongs
              random_priority_scheduling = distr2(gen); // Define Priority for Priority Algorithms 

        Ready_Queue.push_back(Process()); // Push bacj new Process created with default constructor

        Ready_Queue[i].arrival_time = random_arrival;
        Ready_Queue[i].burst_time   = random_burst_time;   
        Ready_Queue[i].priority_type = random_priority_queue;
        if(random_priority_queue == 2) // It sets only prioriry for priority algorithm
            Ready_Queue[i].priority = random_priority_scheduling; 
        Ready_Queue[i].proces_ID = i+1;
    }
}

void printQueueProps(vector<Process> P){

    for(int i = 0 ; i < P.size(); i++){

        cout    << "Job " << P[i].proces_ID << ": " << endl
                << "| Arrival Time: "    <<   P[i].arrival_time << endl
                << "| Burst Time: "      <<   P[i].burst_time << endl
                << "| Priority: "        <<   P[i].priority << endl
                << "| Priority Tupe: "   <<   P[i].priority_type << endl
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

void queueActivationHandler(){



}

void queueSortingHandle(vector<Process> P){
    int h = 0,j = 0,k = 0,l = 0;

    for(int i = 0; i < P.size(); i++){

                 if (P[i].priority_type == 1 ){
                    copyJobToNewQueue(P, Queue_1, i,h);
                    h++;
                 }
                    
                else if (P[i].priority_type == 2 ){
                    copyJobToNewQueue(P, Queue_2, i,j);
                    j++;
                }
                else if (P[i].priority_type == 3){
                    copyJobToNewQueue(P, Queue_3, i,k);
                    k++;
                }
                else if (P[i].priority_type == 4 ){
                    copyJobToNewQueue(P, Queue_4, i,l);
                    l++;
                } 
    }
}

void copyJobToNewQueue(vector<Process> P, vector<Process> &New_Q, int i, int m){

                    New_Q.push_back(Process());
                    New_Q[m].arrival_time    = P[i].arrival_time;
                    New_Q[m].burst_time      = P[i].burst_time;
                    New_Q[m].priority        = P[i].priority;
                    New_Q[m].priority_type   = P[i].priority_type;
                    New_Q[m].proces_ID       = P[i].proces_ID;
                    New_Q[m].remaining_time  = P[i].remaining_time;
                    New_Q[m].turnaroud_time  = P[i].turnaroud_time;
                    New_Q[m].waiting_time    = P[i].waiting_time;

}

//  ===================================================
//  CPU Scheduling Functions and Methods


void RR(){
    

}
void Priority(){

}
void SJF(){

}
void FCFS(){

}
void calcWaitingTime(){

}
void calcTurnAroundTime(){

}

//  ===================================================
//  SORTING 


void sortMerge(vector<Process> &P){ 

    if( 1 < P.size()){

        vector<Process> left_temp_vector(P.begin(), P.begin() + P.size() / 2 );
        sortMerge(left_temp_vector);

        vector<Process> right_temp_vector(P.begin() + P.size() / 2 , P.end());
        sortMerge(right_temp_vector);

        mergeHalves(P, left_temp_vector, right_temp_vector);
    }

}

//  merge Priority 

void mergeHalves(vector<Process> &P, vector<Process> Left, vector<Process> Right){

    P.clear();
    int i, j, index; 

    for(i = 0, j = 0, index = 0; i < Left.size() && j < Right.size(); index++ ){

        if(Left[i].priority <= Right[j].priority) {
                   P.push_back(Left.at(i));
                     i++;
        }
        else if(Left[i].priority > Right[j].priority ){
            P.push_back(Right.at(j));
            j++;
        }
        index++;
    }

    while( i < Left.size()){
        P.push_back(Left.at(i));
        i++;
    }
    while( j < Right.size()){
        P.push_back(Right.at(j));
        j++;
    }

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

    // sortMerge(Ready_Queue);

    // cout << "Sorted Queue: " << endl; 

    // printQueueJobs(Ready_Queue);

    queueSortingHandle(Ready_Queue);


    cout <<  " NEW QUEUES" << endl << endl; 

    // cout << "Processes 1:" << endl << endl;
    // printQueueProps(Queue_1);
    cout << "Round Robin:" << endl;
    cout << "Jobs Order :" << endl << endl;
    printQueueJobs(Queue_1);

    //  cout << "Processes 2:" << endl << endl;
    // printQueueProps(Queue_2);
    cout << "Priority Order" << endl;
    cout << "Jobs Order :" << endl << endl;
    printQueueJobs(Queue_2);

    //  cout << "Processes 3:" << endl << endl;
    // printQueueProps(Queue_3);
    cout << "Shortest Job First:" << endl;
    cout << "Jobs Order:" << endl << endl;
    printQueueJobs(Queue_3);


    //  cout << "Processes 4:" << endl << endl;
    // printQueueProps(Queue_4);
    cout << "First Come First Served:" << endl;
    cout << "Jobs Order <---:" << endl << endl;
    printQueueJobs(Queue_4);

    return 0;
}