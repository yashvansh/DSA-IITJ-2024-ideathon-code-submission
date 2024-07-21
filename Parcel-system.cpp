#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <set>
#include <string.h>
#include <queue>
#include <stack>
#include <list>
using namespace std;

class Parcel {
public:
    int time_tick;
    std::string parcel_id;
    std::string origin;
    std::string destination;
    int priority;
    bool delivered;
    std::string current_location;

    Parcel() = default;
    Parcel(int time_tick, std::string parcel_id, std::string origin, std::string destination, int priority) {
        this->time_tick = time_tick;
        this->parcel_id = parcel_id;
        this->origin = origin;
        this->destination = destination;
        this->priority = priority;
        this->delivered = false;
        this->current_location = origin;
    }
    ~Parcel(){}
};

class MaxHeap {
public:
    std::vector<Parcel*> heap;

    int parent(int i) {
        // fill in this function
        return i/2;
    }

    int left(int i) {
        // fill in this function
        return 2*i;
    }

    int right(int i) {
        // fill in this function
        return (2*i)+1;
    }

    Parcel * get_max() {
        // fill in this function
        if(heap.size()>=1) return heap[1];
        return nullptr;
    }

    Parcel * extract_max() {
        // function that returns the max element and removes it from the heap
        Parcel* temp=heap[1];
        swap(heap[1],heap[heap.size()-1]);
        heap.pop_back();
        for(int j=1;j<heap.size();j++) max_heapify(j);
        return temp;
    }

    void max_heapify(int i) {
        // function that maintains the max heap property
        while((2*i)+1<heap.size() && (heap[i]->priority<heap[2*i]->priority || heap[i]->priority < heap[(2*i)+1]->priority)){
            if(heap[i]->priority < heap[2*i]->priority) swap(heap[i],heap[2*i]);
            if(heap[i]->priority < heap[(2*i)+1]->priority) swap(heap[i],heap[(2*i) + 1]);
        }
    }

    void insert(Parcel * item) {
        // function that inserts item into the heap
        Parcel* pointer=item;
        if(heap.size()==0) heap.push_back(pointer);
        heap.push_back(pointer);
        int i= heap.size()-1;
        while(i/2 > 0 && heap[i]->priority > heap[i/2]->priority) {
            swap(heap[i],heap[i/2]);
            i=i/2;
        }

    }

    bool is_empty() {
        return heap.empty();
    }
    ~MaxHeap(){}
};

class FreightCar {
public:
    int max_parcel_capacity;
    std::vector<Parcel*> parcels;
    std::string destination_city;
    std::string next_link;
    std::string current_location;
    bool sealed;
    set<string> visited;

    FreightCar(int max_parcel_capacity) {
        this->max_parcel_capacity = max_parcel_capacity;
        // instantiate parcels vector as empty initially
        this->parcels = {};
        this->destination_city = "";
        this->next_link = "";
        this->current_location = "";
        this->sealed = true;
    }

    void load_parcel(Parcel *parcel) {
        // load parcel
        if(parcels.size()<max_parcel_capacity) parcels.push_back(parcel);
    }

    bool can_move() {
        // if length of parcels is equal to max_parcel_capacity return true else return false
        if(parcels.size()==max_parcel_capacity) return true;
        else return false;
    }

    // void move(std::string destination) {
    //     // function that moves freight car to destination (link)
    // }
    ~FreightCar(){}
};

class Vertex {
public:
    std::string name;
    std::vector<FreightCar*> freight_cars;
    std::vector<Vertex*> neighbors;
    // trains_to_move is dict of destination and freight cars 
    std::map<std::string, std::vector<FreightCar*>> trains_to_move;
    int min_freight_cars_to_move;
    int max_parcel_capacity;
    std::unordered_map<std::string, MaxHeap> parcel_destination_heaps;
    std::vector<FreightCar*> sealed_freight_cars;
    std::vector<Parcel*> all_parcels;
    map<string,vector<FreightCar*>> firstlink;
    map<string,vector<string>> bfs_path;
    map<string,vector<string>> dfs_paths;
    Vertex() = default;
    Vertex(std::string name, int min_freight_cars_to_move, int max_parcel_capacity) {
        this->name = name;
        this->freight_cars = {};
        this->neighbors = {};
        this->trains_to_move = {};
        this->min_freight_cars_to_move = min_freight_cars_to_move;
        this->max_parcel_capacity = max_parcel_capacity;
        this->parcel_destination_heaps = {};
        this->sealed_freight_cars = {};
        this->all_parcels = {};
    }

    void add_neighbor(Vertex* neighbor) {
        // add neighbor to neighbors vector
        int k=0;
        Vertex * neighs=neighbor;
        for(auto neigh : neighbors){
            if(neigh->name==neighs->name) {k=1;break;}
        }
        if(k==0) neighbors.push_back(neighs);
    }

    // add get neighbors function
    std::vector<Vertex*> get_neighbors() {
        return this->neighbors;
    }

    std::vector<Parcel*> get_all_current_parcels() {
        // get all parcels from all destination heaps
        return this->all_parcels;
    }

    // void clean_unmoved_freight_cars() {
    //     // after time tick t, freight cars with parcels < min parcels should be deleted and parcels are to be added to heap
    // }

    // void loadParcel(Parcel parcel) {
    //     // load parcel into heap
    // }

    void loadFreightCars() {
        // load parcels into freight cars (making sure that each freight car has parcels from only one destination, and # parcels == min_parcel_capacity)
        //forming cars
        for(auto sealed:freight_cars){
            if(sealed->destination_city!=name){
            if(bfs_path[sealed->destination_city].size()>0) sealed->next_link=bfs_path[sealed->destination_city][1];
            firstlink[sealed->next_link].push_back(sealed);
            }
        }
        for(auto& key : parcel_destination_heaps){
            while(key.second.heap.size()>max_parcel_capacity){
                FreightCar*newcar=new FreightCar(max_parcel_capacity);
                newcar->visited.insert(name);
                newcar->current_location= name;
                newcar->destination_city=key.first;
                if(bfs_path[key.first].size()>0)  {newcar->next_link=bfs_path[key.first][1];
                firstlink[newcar->next_link].push_back(newcar);}
                newcar->sealed=true;
                for(int i=0;i<max_parcel_capacity;i++){
                  Parcel* parc= key.second.extract_max();
                  newcar->load_parcel(parc);
                }
                freight_cars.push_back(newcar);
            }
        }
        //trying to group parcels freight cars which are not grouped yet
        for(auto temp:firstlink){
            if(temp.second.size()<min_freight_cars_to_move-1){
                for(auto car:temp.second){
                if( dfs_paths[car->destination_city].size()>0){
                 string link = dfs_paths[car->destination_city][1];
                 int k=0;
                 for(auto visi:car->visited) if(visi==link){k=1;break;}
                 if(firstlink[link].size()>=min_freight_cars_to_move-1 && k==0){
                  firstlink[link].push_back(car);
                  firstlink[car->next_link].erase(remove(firstlink[car->next_link].begin(), firstlink[car->next_link].end(), car), firstlink[car->next_link].end());
                  car->next_link=link;
                 }                  
                
                }
                }
            }
        }
        //making train
        for(auto temp:firstlink){
            if(temp.second.size()>=min_freight_cars_to_move) {
                vector<FreightCar*> tp;
                for(auto car:temp.second) {
                tp.push_back(car);
                }
                trains_to_move[temp.first]=tp;
            }
        }

    }

    // void print_parcels_in_freight_cars() {
    //     for (auto& [destination, heap] : this->parcel_destination_heaps) {
    //         for (auto& parcel : heap.heap) {
    //             std::cout << "Parcel ID: " << parcel->parcel_id << ", Parcel origin: " << parcel->origin << ", Parcel destination: " << parcel->destination << ", Parcel priority: " << parcel->priority << std::endl;
    //         }
    //     }
    // }
    void print_parcels_in_freight_cars() {
    for (auto it = this->parcel_destination_heaps.begin(); it != this->parcel_destination_heaps.end(); ++it) {
        auto& destination = it->first;
        auto& heap = it->second;

        for (auto parcel_it = heap.heap.begin(); parcel_it != heap.heap.end(); ++parcel_it) {
            auto& parcel = *parcel_it;
            std::cout << "Parcel ID: " << parcel->parcel_id << ", Parcel origin: " << parcel->origin << ", Parcel destination: " << parcel->destination << ", Parcel priority: " << parcel->priority << std::endl;
        }
    }
}

    ~Vertex(){}
};

class Graph {

public:
    std::list<Vertex*> vertices;
    std::vector<std::pair<Vertex*, Vertex*>> edges;
    
    void add_edge(std::string source, std::string destination, int min_freight_cars_to_move, int max_parcel_capacity) {
        // check if source and destination vertices exist
        // if they do not exist, create them
        // add edges accordingly
        int k=0;
        Vertex * destiny;
        Vertex * origin;
        for(auto vert:vertices){
             if(vert->name==source) {Vertex *temp= vert;origin=temp;k=1;break;}
        }
        if(k==0){
            origin=new Vertex(source,min_freight_cars_to_move,max_parcel_capacity);
            vertices.push_back(origin);
        }
        
        k=0;
        for(auto ver:vertices){
             if(ver->name==destination) {Vertex *temp2 = ver;destiny=temp2; k=1;break;}
        }
        if(k==0){
            destiny=new Vertex(destination,min_freight_cars_to_move,max_parcel_capacity);
            vertices.push_back(destiny);
        }
       origin->add_neighbor(destiny);
        destiny->add_neighbor(origin);
        k=0;
        for(auto edge:edges){
            if((edge.first->name==origin->name && edge.second->name==destiny->name) || (edge.first->name==destiny->name && edge.second->name==origin->name) ) {k=1;break;}
        }
        if(k==0){
            edges.push_back(make_pair(origin,destiny));
        }
    }

    void print_graph() {
        std::cout << "Printing graph, vertices: " << vertices.size() << std::endl;
        for (auto& vertex : this->vertices) {
            std::cout << "Vertex: " << vertex->name << std::endl;
            std::cout << "Neighbors: ";
            for (auto& neighbor : vertex->neighbors) {
                std::cout << neighbor->name << " ";
            }
            std::cout << std::endl;
        }
    }

    Vertex * findvertex(string& temp){
        for(auto tp:vertices){
            if(tp->name==temp) {Vertex* found=tp;return found;}
        }
        return nullptr;
    }
    std::vector<std::string> bfs(std::string source, std::string destination) {
        // function to return path from source to destination using bfs
        queue<Vertex*> explore;
        vector<Vertex*> visited;
        vector<string> path;
        map<string,string> discovery;
        Vertex * origin= findvertex(source);
        if(source==destination || origin==nullptr) return path;
        explore.push(origin);
        visited.push_back(origin);
        while(!explore.empty()){
            int c=0;
            for(auto neigh : explore.front()->neighbors){
                int k=0;
                for(auto vis:visited) {
                    if(vis==neigh) {k=1;break;}
                }
                if(k==0){
                  discovery.insert({neigh->name,explore.front()->name});
                  if(neigh->name==destination) {c=1;break;} 
                  explore.push(neigh);
                  visited.push_back(neigh);
                }            
            }
            if(c==1) break;
            explore.pop();
        }
        
        string itr=destination;
        if(discovery.find(itr)==discovery.end()) return path;
        path.insert(path.begin(),itr);
        while(itr!=source){
            itr=discovery[itr];
            path.insert(path.begin(),itr);
        }
     return path;
 }

    std::vector<std::string> dfs(std::string source, std::string destination) {
       // function to return path from source to destination using dfs
    set<string> visited;
    Vertex * origin=findvertex(source);
    vector<string> path;
    if(source==destination || origin==nullptr) return path;
    path.push_back(origin->name);
    DFS(origin,destination,visited,path);
    if(path.size()==2 && path[1]!=destination) {path.clear();return path;}
    else return path;
}
   int DFS(Vertex* origin, string destination, set<string>& visited, vector<string>& path) {
   visited.insert(origin->name);
    if (origin->name == destination) {
       return 1;
    }
    else{
    for (auto nei : origin->get_neighbors()) {
        if (visited.count(nei->name) == 0) {
            path.push_back(nei->name);
            int i=DFS(nei, destination, visited, path);
            if(i==1) return 1;
            path.pop_back();
        }
    }
    }
    return 0;
}


    void groupFreightCars() {
        // for every vertex, group freight cars by links
        for(auto vertex:vertices){
          vertex->loadFreightCars();
        }
    }

    void moveTrains() {
        // for every vertex, move freight cars to next link
        for(auto vertex:vertices){
            for(auto train:vertex->trains_to_move){
                string temp=train.first;
                Vertex*destiny = findvertex(temp);
                for(auto car:train.second){
                destiny->freight_cars.push_back(car);
                for(auto parc:car->parcels) vertex->all_parcels.erase(remove(vertex->all_parcels.begin(),vertex->all_parcels.end(),parc),vertex->all_parcels.end());
                vertex->freight_cars.erase(remove(vertex->freight_cars.begin(), vertex->freight_cars.end(), car), vertex->freight_cars.end());
                car->current_location=destiny->name;
                car->visited.insert(car->current_location);
                for(auto parc:car->parcels) {
                    destiny->all_parcels.push_back(parc);
                    parc->current_location=destiny->name;
                    if(parc->current_location==parc->destination) parc->delivered=true;
                }
                }
                
            }
            vertex->firstlink.clear();
            vertex->trains_to_move.clear();
        }
    }
    ~Graph(){}
};

class PRC {
public:
    Graph graph;
    std::vector<FreightCar*> freight_cars;
    std::map<std::string, Parcel*> parcels;
    std::map<int, std::vector<Parcel*>> parcels_with_time_tick;
    int max_parcel_capacity;
    int min_freight_cars_to_move;
    int time_tick;
    std::map<std::string, std::string> old_state;
    std::map<std::string, std::string> new_state;
    int max_time_tick;
    map<int,vector<string>> delivered_till_time;

    PRC(int max_parcel_capacity, int min_freight_cars_to_move) {
        this->max_parcel_capacity = max_parcel_capacity;
        this->min_freight_cars_to_move = min_freight_cars_to_move;
        this->time_tick = 1;
        this->max_time_tick = 100;
        this->freight_cars = {};
        this->parcels = {};
        this->parcels_with_time_tick = {};
        this->old_state = {};
        this->new_state = {};

    }

    void create_graph(string filename) {
     // read graph.txt file and create graph
     Graph*newgraph=new Graph();
     ifstream ip(filename);
     while(ip.good()){
        string origin;
        string destiny;
        getline(ip,origin,' ');
        getline(ip,destiny,'\n');
        newgraph->add_edge(origin,destiny,min_freight_cars_to_move,max_parcel_capacity); 
     }
     graph = *newgraph;
     for(auto& vertex: graph.vertices){
        for(auto& destiny : graph.vertices){
            if(destiny->name!=vertex->name){
                vector<string> bfs=graph.bfs(vertex->name,destiny->name);
                vector<string> dfs=graph.dfs(vertex->name,destiny->name);
                vertex->bfs_path[destiny->name]=bfs;
                vertex->dfs_paths[destiny->name]=dfs;
            }
        }
     }
    }

    void process_parcels(string filename){
        // read parcels.txt file and create parcels
        ifstream ip(filename);
        while(ip.good()){
            string pid,origin,destiny;
            string tt,prio;
            getline(ip,tt,' ');
            getline(ip,pid,' ');
            getline(ip,origin,' ');
            getline(ip,destiny,' ');
            getline(ip,prio,'\n');
            int tick = stoi(tt);
            int priority = stoi(prio);
            Parcel*newpar;
            if(parcels.find(pid)==parcels.end()) newpar=new Parcel(tick,pid,origin,destiny,priority);
            else continue;
            newpar->current_location=origin;
            parcels.insert({pid,newpar});
            if(parcels_with_time_tick.find(tick)==parcels_with_time_tick.end()){
                vector<Parcel*> newtt;
                parcels_with_time_tick.insert({tick,newtt});
            }
            parcels_with_time_tick[tick].push_back(newpar);
        } 
        for(auto& parcel:parcels){
            new_state.insert({parcel.first,parcel.second->current_location});
        }
    }

    // function to return bookings at a particular time tick at a particular vertex
    std::vector<Parcel*> get_bookings(int time_tick, std::string vertex) {
        run_simulation(time_tick);
        return graph.findvertex(vertex)->all_parcels;
    }

    std::map<std::string, std::string> get_state() {
        std::map<std::string, std::string> state;
        // from this.parcels
        for (auto& parcel_pair : this->parcels) {
            Parcel * parcel = parcel_pair.second;
            state[parcel->parcel_id] = parcel->current_location;
        }
        return state;
    }

    // run simulation function, takes run_till_time_tick as argument
    // if run_till_time_tick is not provided then run simulation till max_time_tick

    void run_simulation(int run_till_time_tick = -1) {
        // function to run simulation
        while(time_tick<=run_till_time_tick && !convergence_check()){
        if(time_tick!=1)   old_state=new_state;
        graph.moveTrains();
        delivered_till_time[time_tick]=get_delivered_parcels();
        if(parcels_with_time_tick.find(time_tick)!=parcels_with_time_tick.end()){
         for(auto& parcel:parcels_with_time_tick[time_tick]){
          Vertex * vertex = graph.findvertex(parcel->origin);
          vertex->all_parcels.push_back(parcel);
            if(vertex->parcel_destination_heaps.find(parcel->destination)==vertex->parcel_destination_heaps.end()){
                MaxHeap * newheap=new MaxHeap();
                vertex->parcel_destination_heaps.insert({parcel->destination,*newheap});
            }
            vertex->parcel_destination_heaps[parcel->destination].insert(parcel);  
        }
        }
        graph.groupFreightCars();
        new_state=get_state();
        time_tick++;
        }

    }

    bool convergence_check(){
        // function to check convergence between old_state and new_state
        for (auto& parcel_pair : this->parcels) {
            if(old_state.size()==0 || new_state.find(parcel_pair.first)==new_state.end()) return false;
             if(old_state[parcel_pair.first]!=new_state[parcel_pair.first]) return false;
         }
         return true;
    }

    bool all_parcels_delivered() {
        for (auto& parcel_pair : this->parcels) {
            Parcel * parcel = parcel_pair.second;
            if (!parcel->delivered) {
                return false;
            }
        }
        return true;
    }

    std::vector<std::string> get_stranded_parcels() {
        // return parcel_ids of stranded parcels
        vector<string> stranded;
        for (auto& parcel_pair : this->parcels) {
            if(old_state.find(parcel_pair.second->parcel_id)!=old_state.end() &&  old_state[parcel_pair.second->parcel_id]==new_state[parcel_pair.second->parcel_id] && parcel_pair.second->time_tick<time_tick && parcel_pair.second->delivered==false) stranded.push_back(parcel_pair.first);
        }
        return stranded;
    }

    std::vector<std::string> get_delivered_parcels() {
        // return parcel_ids of delivered parcels
        vector<string> delivered;
        for(auto& parcel : parcels){
            if(parcel.second->delivered==true) delivered.push_back(parcel.first);
        }  
        return delivered;
    }

    // get status of all parcels at time tick t (status = id, current location, delivered/not delivered)
    std::vector<std::string> get_status_of_all_parcels(int t) {
        //  std::string status = parcel.parcel_id + " " + parcel.current_location + " " + std::to_string(parcel.delivered) + "\n" ;
        // return the status of all parcels at time tick t as a vector of strings (each string formatted as above)
        if(t>time_tick) run_simulation(t);
        vector<string> status;
        for(auto& parcel:parcels){
            Parcel * parc=parcel.second;
            status.push_back(parc->parcel_id + " " + parc->current_location + " " + to_string(parc->delivered) + "\n");
        }
        return status;
    }

    // get status of parcel_id at time tick t (status = id, current location, delivered/not delivered)
    std::string get_status_of_parcel(std::string parcel_id, int t) {
        // status = parcel.parcel_id + " " + parcel.current_location + " " + std::to_string(parcel.delivered);
        // return status of one parcel at time tick t as a string (formatted as above)
        if(t>time_tick) run_simulation(t);
        return parcel_id + " " + parcels[parcel_id]->current_location + " " + to_string(parcels[parcel_id]->delivered);
    }

    // get parcels delivered up to time tick t
    std::vector<std::string> get_parcels_delivered_up_to_time_tick(int t) {
        // return list of parcel_ids of parcels delivered up to time tick t
         if(t>time_tick) {
            run_simulation(t);
            return get_delivered_parcels();
         }
         else return delivered_till_time[t];
    }
    ~PRC(){}
};

