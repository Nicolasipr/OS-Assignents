#include <iostream>
#include <ctime> // time
#include <cstdlib> //ramd
#include <random>
#include <cstdio>
#include <vector> 


using namespace std;


//  ===================================================
//  GLOBAL VARIABLES 

float QUANTUM = 2,  // set on 2, but it can be changed
      current_Time = 0.0; 

int N = 0;          // GV number of total processes  



//  ===================================================
//  Process struct definition

struct Process{

    float burst_time = 0.0,
          waiting_time = 0.0,
          turnaroud_time = 0.0,
          remaining_time = 0.0,
          arrival_time = 0.0;

    int priority = 0,
        priority_type = 0,
    proces_ID = 0;

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
void RR(vector<Process> &P, int i);
void Priority(vector<Process> P, int i);
void SJF(vector<Process> P, int i);
void FCFS(vector<Process> P, int i);
void calcWaitingTime(vector<Process> P, int i);
void calcTurnAroundTime(vector<Process> P, int i);
void sortMerge(vector<Process> &P);
void mergeHalvesJobs(vector<Process> &P,vector<Process> L, vector<Process> R);
void mergeHalvesPriority(vector<Process> &P,vector<Process> L, vector<Process> R);

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

        Ready_Queue.push_back(Process()); // Push back new Process created with default constructor

        Ready_Queue[i].arrival_time = random_arrival;
        Ready_Queue[i].burst_time   = random_burst_time;   
        Ready_Queue[i].priority_type = random_priority_queue;
        Ready_Queue[i].remaining_time = Ready_Queue[i].burst_time;
        if(random_priority_queue == 2) // It sets only prioriry for priority algorithm
            Ready_Queue[i].priority = random_priority_scheduling; 
        Ready_Queue[i].proces_ID = i+1;
    }
}

void printQueueProps(vector<Process> P){

    if(P.size()){

            for(int i = 0 ; i < P.size(); i++){
        cout    << "Job " << P[i].proces_ID << ": " << endl
                << "| Arrival Time: "    <<   P[i].arrival_time << endl
                << "| Burst Time: "      <<   P[i].burst_time << endl
                << "| Priority: "        <<   P[i].priority << endl
                << "| Priority Type: "   <<   P[i].priority_type << endl
                << "| Turnaround Time: " <<   P[i].turnaroud_time << endl
                << "| Waiting Time: "    <<   P[i].waiting_time << endl
                << "| Remaining Time: "  <<   P[i].remaining_time << endl; 
            }


    }


}

void printQueueJobs(vector<Process> P){

    if(P.size()){
        
        for(int i = P.size() - 1 ; i >= 0; i--){
        cout << "| Job " << P[i].proces_ID << " ";
        }

        cout << "|" << endl;

    }

    cout << endl << endl;
}

void queueActivationHandler(){

    int h = 0, // queue 1
        j = 0, // queue 2
        k = 0, // queue 3
        l = 0; // queue 4
    bool RR_interrupt = false;

    if(Ready_Queue[0].priority_type == 1){
        cout <<  "IF" << endl; 
        
        current_Time += Ready_Queue[0].arrival_time + Ready_Queue[0].burst_time;
        RR(Ready_Queue,0);
        while(Queue_1.size() && RR_interrupt == false){

            for(int i = 0; i < Queue_1.size(); i++){
                RR(Queue_1,i); 
                if(Queue_1[i + 1].proces_ID != 0 || current_Time <= Queue_1[i + 1].arrival_time ){

                    RR_interrupt = true;
                    break;
                    }
                    
                }

            }
            copyJobToNewQueue(Ready_Queue, Gantt_Queue, 0,0);
            // Queue_1.erase(Queue_1.begin());
            printQueueJobs(Gantt_Queue);
        }
            
    else{
        cout << "Else" << endl;
        if(Queue_1.size() && Ready_Queue[0].priority_type == 1){

            if( Ready_Queue[0].arrival_time >= Queue_1[0].arrival_time // if process arrives in the middle of the executiom, 
            && Queue_1[0].arrival_time <= Ready_Queue[0].burst_time ){ // pauses it and proceeds to activate the important one     

            cout << "Else IF 1 " << endl;
            current_Time += Queue_1[0].arrival_time - Ready_Queue[0].arrival_time;
            Ready_Queue[0].remaining_time -= Queue_1[0].arrival_time;    
            }

        }else if(Queue_2.size() && Ready_Queue[0].priority_type == 2){

            if( Ready_Queue[0].arrival_time >= Queue_2[0].arrival_time // if process arrives in the middle of the executiom, 
            && Queue_2[0].arrival_time <= Ready_Queue[0].burst_time ){ // pauses it and proceeds to activate the important one     

            cout << "Else IF 2 " << endl;
            current_Time += Queue_2[0].arrival_time - Ready_Queue[0].arrival_time;
            Ready_Queue[0].remaining_time -= Queue_2[0].arrival_time;    
            }

        }else
        if(Queue_3.size() && Ready_Queue[0].priority_type == 3){

            if( Ready_Queue[0].arrival_time >= Queue_3[0].arrival_time // if process arrives in the middle of the executiom, 
            && Queue_3[0].arrival_time <= Ready_Queue[0].burst_time ){ // pauses it and proceeds to activate the important one     
            cout << "Else IF 3 " << endl;
            current_Time += Queue_3[0].arrival_time - Ready_Queue[0].arrival_time;
            Ready_Queue[0].remaining_time -= Queue_3[0].arrival_time;    
            }

        }else
        if(Queue_4.size() && Ready_Queue[0].priority_type == 4){

            if( Ready_Queue[0].arrival_time >= Queue_4[0].arrival_time // if process arrives in the middle of the executiom, 
            && Queue_4[0].arrival_time <= Ready_Queue[0].burst_time ){ // pauses it and proceeds to activate the important one     
            cout << "Else IF 4 " << endl;
            
            current_Time += Queue_4[0].arrival_time - Ready_Queue[0].arrival_time;
            Ready_Queue[0].remaining_time -= Queue_4[0].arrival_time;    
            }

        }else 
        
        // current_Time += Ready_Queue[0].arrival_time + Ready_Queue[0].burst_time ;
        copyJobToNewQueue(Ready_Queue, Gantt_Queue, 0,0);

        switch ( Ready_Queue[0].priority_type)
        {
            case 2:
                cout << "Case 2" << endl;
                if(Queue_2[0].remaining_time <= 0)
                Queue_2.erase(Queue_2.begin());
                break;
            case 3:
                cout << "Case 3" << endl;
                if(Queue_3[0].remaining_time <= 0)
                Queue_3.erase(Queue_3.begin());
                break;
            case 4:
                cout << "Case 4" << endl;
                if(Queue_4[0].remaining_time <= 0)
                Queue_4.erase(Queue_4.begin());
                break;
        
            default:
                break;
        }

        Ready_Queue.erase(Ready_Queue.begin());
        printQueueJobs(Gantt_Queue);

    }
    cout << "While" << endl;

    while(Ready_Queue.size()){
        printQueueJobs(Gantt_Queue);


        if( Ready_Queue[0].priority_type == 1 && Queue_1.size() && current_Time >= Queue_1[h].arrival_time){
            cout <<  "IF1" << endl; 
            RR(Queue_1,h);

        }
        else if(Ready_Queue[0].priority_type == 2 && Queue_2.size() && current_Time >= Queue_2[j].arrival_time){
            cout <<  "IF 2" << endl;
            Priority(Queue_2,j);
            
        }
        else if(Ready_Queue[0].priority_type == 3 && Queue_3.size() && current_Time >= Queue_1[k].arrival_time){
            cout <<  "IF 3" << endl;
            SJF(Queue_3, k);
            
        }
        else if(Ready_Queue[0].priority_type == 4 && Queue_4.size() && current_Time >= Queue_1[l].arrival_time){
            cout <<  "IF 4" << endl;
            FCFS(Queue_4,l);
            

        }     
        else{
            current_Time += QUANTUM;  
            if(Queue_1.size())
            copyJobToNewQueue(Queue_1,Gantt_Queue,0, Gantt_Queue.size());
        }   
        
        

    }



}

void queueSortingHandle(vector<Process> P){
    int h = 0,j = 0,k = 0,l = 0;

    for(int i = 0; i < P.size(); i++){


                 if (P[i].priority_type == 1 ){ // Round Robin
                    copyJobToNewQueue(P, Queue_1, i,h);
                    h++;
                 }
                    
                else if (P[i].priority_type == 2 ){ // Priority 
                    copyJobToNewQueue(P, Queue_2, i,j);
                    j++;
                }
                else if (P[i].priority_type == 3){ // SJF
                    copyJobToNewQueue(P, Queue_3, i,k);
                    k++;
                }
                else if (P[i].priority_type == 4 ){ // FCFS
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


void RR(vector<Process> &P, const int i){ // per process

    if( current_Time >= P[i].arrival_time  && P[i].remaining_time > 0 ){
        
        copyJobToNewQueue(P, Gantt_Queue, i, Gantt_Queue.size());
        current_Time += QUANTUM;
        P[i].remaining_time -= QUANTUM;
        if (P[i].remaining_time <= 0){
            P.erase(P.begin()+i); // Does this work ?
            Ready_Queue.erase(Ready_Queue.begin());
        }

    }

    

}
void Priority(vector<Process> P, int i){

    if(current_Time >= P[i].arrival_time){
        copyJobToNewQueue(P, Gantt_Queue, i, Gantt_Queue.size());
    
        current_Time += P[0].remaining_time;
        P.erase(P.begin());
        Ready_Queue.erase(Ready_Queue.begin());
    }
}
void SJF(vector<Process> P, int i){

    if(current_Time >= P[i].arrival_time){
        copyJobToNewQueue(P, Gantt_Queue, i, Gantt_Queue.size());
        current_Time += P[0].remaining_time;
        P.erase(P.begin());
        Ready_Queue.erase(Ready_Queue.begin());
    }

}
void FCFS(vector<Process> P, int i){

    if(current_Time >= P[i].arrival_time){
        copyJobToNewQueue(P, Gantt_Queue, i, Gantt_Queue.size());
        current_Time += P[0].remaining_time;
        P.erase(P.begin());
        Ready_Queue.erase(Ready_Queue.begin());
    }

}
void calcWaitingTime(vector<Process> P, int i){

    for(int i = 0; i < P.size() ; i++)
        P[i].waiting_time = P[i - 1].waiting_time + P[i - 1].burst_time;
}
void calcTurnAroundTime(vector<Process> P, int i){

    for(int i = 0; i < P.size() ; i++)
        P[i].turnaroud_time = P[i].waiting_time + P[i].burst_time;


}

//  ===================================================
//  SORTING 


void sortMerge(vector<Process> &P, int type){ 

    if( 1 < P.size()){

        vector<Process> left_temp_vector(P.begin(), P.begin() + P.size() / 2 );
        sortMerge(left_temp_vector, type);

        vector<Process> right_temp_vector(P.begin() + P.size() / 2 , P.end());
        sortMerge(right_temp_vector, type);
        switch (type)
        {
            case 1: // Sorting Priority
                mergeHalvesPriority(P, left_temp_vector, right_temp_vector);
                break;
            case 2: // sorting jobs
                mergeHalvesJobs(P, left_temp_vector, right_temp_vector);
                break;

            default:
                break;
        }
        
    }

}

//  merge Priority  case 1 

void mergeHalvesPriority(vector<Process> &P, vector<Process> Left, vector<Process> Right){

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

//  merge Shortest Jobs   case 2

void mergeHalvesJobs(vector<Process> &P, vector<Process> Left, vector<Process> Right){

    P.clear();
    int i, j, index; 

    for(i = 0, j = 0, index = 0; i < Left.size() && j < Right.size(); index++ ){

        if(Left[i].burst_time <= Right[j].burst_time) {
                   P.push_back(Left.at(i));
                     i++;
        }
        else if(Left[i].burst_time > Right[j].burst_time ){
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

    
    queueSortingHandle(Ready_Queue);


    cout <<  " NEW QUEUES" << endl << endl; 

    // cout << "Processes 1:" << endl << endl;
    // printQueueProps(Queue_1);
    cout << "Round Robin:" << endl;
    printQueueJobs(Queue_1);

    //  cout << "Processes 2:" << endl << endl;
    // printQueueProps(Queue_2);
    cout << "Priority Order" << endl;
    printQueueJobs(Queue_2);

    //  cout << "Processes 3:" << endl << endl;
    // printQueueProps(Queue_3);
    cout << "Shortest Job First:" << endl;
    printQueueJobs(Queue_3);


    //  cout << "Processes 4:" << endl << endl;
    // printQueueProps(Queue_4);
    cout << "First Come First Served:" << endl;
    printQueueJobs(Queue_4);



    queueActivationHandler();

    printQueueJobs(Gantt_Queue);
    printQueueProps(Gantt_Queue);

    cout << "Current Time: " << current_Time; 

    return 0;
}