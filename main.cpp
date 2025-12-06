#include "include/NPC.h"
#include "include/Observer.h"
#include "include/Visitor.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <chrono>
#include <random>

std::shared_mutex arr_mutex;
std::mutex cout_mutex;
std::mutex queue_mutex;

std::queue<std::string> message_queue;

void generate_random_npcs(NPC_array& arr, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> pos_dist(0, 100);
    std::uniform_int_distribution<> type_dist(0, 2);
    std::vector<std::string> types = {"squirrel", "werewolf", "druid"};
    for (int i = 0; i < count; ++i) {
        std::string type = types[type_dist(gen)];
        std::string name = type + "_" + std::to_string(i);
        double x = pos_dist(gen);
        double y = pos_dist(gen);
        arr.add_NPC(NPCFactory::create_npc(type, name, x, y));
    }
}

class QueueObserver : public Observer {
public:
    void update(const std::string& event) override {
        std::lock_guard<std::mutex> lock(queue_mutex);
        message_queue.push(event);
    }
};

void movement_thread(NPC_array& arr, CombatVisitor& visitor, 
                     std::atomic<bool>& running) {
    while (running) {
        {
            std::lock_guard<std::shared_mutex> lock(arr_mutex);
            visitor.do_move(arr);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void combat_thread(NPC_array& arr, CombatVisitor& visitor,
                   std::atomic<bool>& running) {
    while (running) {
        {
            std::lock_guard<std::shared_mutex> lock(arr_mutex);
            for (auto& npc : arr.get_npcs()) {
                if (!npc->is_alive_NPC()) continue;                
                std::list<NPC*> target_found = visitor.find_in_rad(arr, npc);
                if (!target_found.empty()) {
                    visitor.do_combat(arr, npc, target_found);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void output_thread(std::atomic<bool>& running) {
    while (running) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            while (!message_queue.empty()) {
                std::lock_guard<std::mutex> cout_lock(cout_mutex);
                std::cout << message_queue.front() << std::endl;
                message_queue.pop();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    NPC_array arr;
    arr.set_map_size(100, 100);    
    generate_random_npcs(arr, 50);
    std::cout << "\nСгенерировано:\n";
    arr.print_all();
    std::cout << "\nСтарт:\n\n";
    CombatVisitor visitor;
    visitor.add_observer(std::make_unique<QueueObserver>());
    std::atomic<bool> running{true};
    std::thread move_thread(movement_thread, std::ref(arr), 
                           std::ref(visitor), std::ref(running));
    std::thread fight_thread(combat_thread, std::ref(arr), 
                            std::ref(visitor), std::ref(running));
    std::thread print_thread(output_thread, std::ref(running));
    std::this_thread::sleep_for(std::chrono::seconds(30));
    running = false;   
    move_thread.join();
    fight_thread.join();
    print_thread.join();
    std::cout << "\nСписок выживших: \n";
    {
        std::lock_guard<std::shared_mutex> lock(arr_mutex);
        arr.print_all();
    }
    
    return 0;
}