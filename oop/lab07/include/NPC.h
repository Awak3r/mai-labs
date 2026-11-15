#pragma once
#include <cstddef>
#include <cstring>
#include <list>
#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>
#include <cstdio>
#include <utility>
#include <vector>
#include <cmath>
#include <random>

#define MAX_LENGTH 256

class NPC {
    public:
        NPC() : x_cord(0), y_cord(0), is_alive(true) {}
        NPC(const std::string& nam, double x, double y): name(nam), x_cord(x), y_cord(y), 
            is_alive(true) {}
        NPC(const NPC& other) : name(other.name), x_cord(other.x_cord), y_cord(other.y_cord), is_alive(true) {}
        NPC& operator=(const NPC& other) {
            if (this != &other) {
                name = other.name;
                x_cord = other.x_cord;
                y_cord = other.y_cord;
                is_alive = other.is_alive;
            }
            return *this;
        }
        bool operator==(const NPC& other) const {
            return x_cord == other.x_cord && y_cord == other.y_cord && name == other.name && is_alive == other.is_alive;
        }
        void set_x(double x) { x_cord = x; }
        void set_y(double y) { y_cord = y; }
        void set_name(const std::string& nam) { name = nam; }
        double get_x_cord() const { return x_cord; }
        double get_y_cord() const { return y_cord; }
        void set_attack(int n) { attack_value = n; }
        void set_deffence(int n) {deffence_value = n; }
        int get_attack() {return  attack_value; }
        int get_deffence() {return deffence_value;}
        double get_move_value() {return move_value; }
        double get_attck_range() {return attack_range; }
        void kill_npc() { is_alive = false; }
        bool is_alive_NPC() { return is_alive;}
        std::string get_name() const { return name; }
        virtual std::string get_type() const { return "NPC"; }
        virtual ~NPC() noexcept = default;
    protected:
        bool is_alive;
        double x_cord;
        double y_cord;
        int attack_value = 0;
        int deffence_value = 0;
        std::string name;
        double move_value = 0;
        double attack_range = 0;
        
};

class NPC_array {
    public:
        void set_map_size(double x, double y) { map_x_size = x, map_y_size = y; }
        std::pair<double, double> get_map_size() {return std::pair<double, double> (map_x_size, map_y_size); }
        size_t get_size() const { return array.size(); }
        void add_NPC(std::unique_ptr<NPC>&& npc) {
            if (npc->get_x_cord() < 0 || npc->get_x_cord() > get_map_size().first || 
            npc->get_y_cord() < 0 || npc->get_y_cord() > get_map_size().second) {
                fprintf(stderr, "error: invalid NPC coords\n");
                return;
            }
            array.push_back(std::move(npc));
        }
        void remove_at(double x, double y) {
            array.remove_if([x, y](const std::unique_ptr<NPC>& npc) {
                return npc->get_x_cord() == x && npc->get_y_cord() == y;
            });
        }
        void remove_npc(const std::string& name) {
            array.remove_if([&name](const std::unique_ptr<NPC>& npc) {
                return npc->get_name() == name;
            });
        }
        std::list<std::unique_ptr<NPC>>& get_npcs() {
            return array;
        }
        void print_all() const {
            for (const auto& npc : array) {
                std::cout << npc->get_type() << " " << npc->get_name() << " " 
                        << npc->get_x_cord() << " " << npc->get_y_cord() << " "
                        << npc->get_attack() << " " << npc->get_deffence() << "\n";
            }
        }
        void clear() {
            array.clear();
        }
    private:
        std::list<std::unique_ptr<NPC>> array;
        double map_x_size = 100;
        double map_y_size = 100;
};

class squirrel: public NPC {
    public:
        squirrel() : NPC() { move_value = 5; attack_range = 5;}
        squirrel(const std::string& nam, double x, double y) : NPC(nam, x, y) { move_value = 5, attack_range = 5;}
        std::string get_type() const override { return "squirrel"; } 

};

class werewolf: public NPC {
    public:
        werewolf() : NPC() {move_value = 40; attack_range = 5;}
        werewolf(const std::string& nam, double x, double y) : NPC(nam, x, y) {move_value = 40; attack_range = 5;}
        std::string get_type() const override { return "werewolf"; }
};

class druid: public NPC {
    public:
        druid() : NPC() {move_value = 10; attack_range = 10;}
        druid(const std::string& nam, double x, double y) : NPC(nam, x, y) {move_value = 10; attack_range = 10;}
        std::string get_type() const override { return "druid"; }
};


class NPCFactory {
    public:
        static std::unique_ptr<NPC> create_npc(const std::string& npc_type, 
                                            const std::string& name, 
                                            double x, double y) {
            if (npc_type == "squirrel") {
                return std::make_unique<squirrel>(name, x, y);
            }
            else if (npc_type == "werewolf") {
                return std::make_unique<werewolf>(name, x, y);
            }
            else if (npc_type == "druid") {
                return std::make_unique<druid>(name, x, y);
            }
            else {
                throw std::logic_error("invalid NPC type: " + npc_type);
            }
        }
        static void load_from_file(const char* filename, NPC_array& arr) {
            FILE* file = fopen(filename, "r");
            if (!file) {
                throw std::runtime_error("can't open file");
            }
            char type[MAX_LENGTH];
            char name[MAX_LENGTH];
            double x, y;
            int line_number = 0;
            while (fscanf(file, "%255s %255s %lf %lf", type, name, &x, &y) == 4) {
                line_number++;
                if (x < 0 || x > arr.get_map_size().first || y < 0 || y > arr.get_map_size().second) {
                    fprintf(stderr, "Line %d: invalid NPC coords (%.2f, %.2f)\n", 
                            line_number, x, y);
                    continue;
                }
                try {
                    std::string type_str(type);
                    std::string name_str(name);
                    auto npc = create_npc(type_str, name_str, x, y);
                    arr.add_NPC(std::move(npc));
                } catch (const std::exception& e) {
                    fprintf(stderr, "Line %d: error creating NPC: %s\n", 
                            line_number, e.what());
                }
            }
            fclose(file);
        }
        static void save_to_file(const char* filename, NPC_array& arr) {
            FILE* file = fopen(filename, "w");
            if (!file) {
                throw std::runtime_error("can't open file");
            }
            for (auto& npc : arr.get_npcs()) {
                fprintf(file, "%s %s %lf %lf\n", 
                        npc->get_type().c_str(), 
                        npc->get_name().c_str(),
                        npc->get_x_cord(), 
                        npc->get_y_cord());
            }
            fclose(file);
        }
        static void generate_random_npcs(NPC_array& arr, int count) {
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
};



