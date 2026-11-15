#pragma once
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <list>
#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>
#include <cstdio>
#include <vector>
#include <cmath>
#include "NPC.h"
#include "Observer.h"

class NPCVisitor {
public:
    static int do_throw(int a, int b) {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> dist(a, b);
        return dist(gen);
    }
    virtual ~NPCVisitor() = default;
    void add_observer(std::unique_ptr<Observer>&& obs) {  
        observer_array.push_back(std::move(obs));
    }
    void notify(const std::string& event) {
        for (auto& obs : observer_array) {
            obs->update(event);
        }
    }
    virtual void visit_squirrel(std::list<std::string>& to_do, 
                                std::unique_ptr<NPC>& attacker, 
                                NPC* target) {}
    virtual void visit_werewolf(std::list<std::string>& to_do, 
                                std::unique_ptr<NPC>& attacker, 
                                NPC* target) {}
    virtual void visit_druid(std::list<std::string>& to_do, 
                            std::unique_ptr<NPC>& attacker, 
                            NPC* target) {}
    virtual void do_visit(std::list<std::string>& to_do, 
                         std::unique_ptr<NPC>& attacker, 
                         NPC* target) {
        if (attacker->get_type() == "squirrel") visit_squirrel(to_do, attacker, target);
        if (attacker->get_type() == "werewolf") visit_werewolf(to_do, attacker, target);
        if (attacker->get_type() == "druid") visit_druid(to_do, attacker, target);
    }
    std::list<NPC*> find_in_rad(NPC_array& arr, std::unique_ptr<NPC>& npc) {
        std::list<NPC*> to_visit;
        double rad = npc->get_attck_range();
        for (auto& to_npc : arr.get_npcs()) {
            if (npc.get() == to_npc.get()) {
                continue;
            }
            double dx = npc->get_x_cord() - to_npc->get_x_cord();
            double dy = npc->get_y_cord() - to_npc->get_y_cord();
            double dist_sq = dx * dx + dy * dy;
            if (dist_sq <= (rad * rad) && npc->is_alive_NPC() && to_npc->is_alive_NPC()) {
                to_visit.push_back(to_npc.get()); 
            }
        }
        return to_visit;
    }
    std::string move_event_string(std::unique_ptr<NPC>& npc) {
        std::string event = "NPC: " + npc->get_type() + " " + npc->get_name() + 
                          " изменил позицию на: (" + 
                          std::to_string(npc->get_x_cord()) + ", " + 
                          std::to_string(npc->get_y_cord()) + ")";
        return event;
    }
    virtual void do_move(NPC_array& arr) {}
private:
    std::list<std::unique_ptr<Observer>> observer_array;
};

class CombatVisitor : public NPCVisitor {
public:
    std::string combat_event_string(std::unique_ptr<NPC>& npc, NPC* to_npc) {
        std::string event = "NPC " + to_npc->get_type() + " " + to_npc->get_name() + 
                          " убит. Убийца: " + npc->get_type() + " " + npc->get_name() + ".";
        return event;
    }
    bool can_kill(std::unique_ptr<NPC>& npc, NPC* to_npc) {
        return (npc->get_attack() >= to_npc->get_deffence());
    }
    void visit_squirrel(std::list<std::string>& to_delete, 
                       std::unique_ptr<NPC>& npc, 
                       NPC* to_npc) override {
        if (to_npc->get_type() == "druid" || to_npc->get_type() == "werewolf") {
            npc->set_attack(do_throw(1, 6));
            to_npc->set_deffence(do_throw(1, 6));
            if (can_kill(npc, to_npc)) {
                to_npc->kill_npc();
                to_delete.push_back(to_npc->get_name());
                notify(combat_event_string(npc, to_npc));
            }
        }
    }
    void visit_werewolf(std::list<std::string>& to_delete, 
                       std::unique_ptr<NPC>& npc, 
                       NPC* to_npc) override {
        if (to_npc->get_type() == "druid") {
            npc->set_attack(do_throw(1, 6));
            to_npc->set_deffence(do_throw(1, 6));
            if (can_kill(npc, to_npc)) {
                to_npc->kill_npc();
                to_delete.push_back(to_npc->get_name());
                notify(combat_event_string(npc, to_npc));
            }
        }
    }
    void visit_druid(std::list<std::string>& to_delete, 
                    std::unique_ptr<NPC>& npc, 
                    NPC* to_npc) override {
    }
    void do_combat(NPC_array& arr, std::unique_ptr<NPC>& npc, 
                   std::list<NPC*> target_found) {
        std::list<std::string> to_delete;
        for (auto* to_npc : target_found) {
            do_visit(to_delete, npc, to_npc);
        }
        for (auto& cur : to_delete) {
                arr.remove_npc(cur);
        }
    }
    void do_move(NPC_array& arr) override {
        for (auto& npc : arr.get_npcs()) {
            if (!npc->is_alive_NPC()) continue;
            int try_count = 4;
            bool do_step = false;
            while (try_count != 0 && do_step == false) {
                int direction = do_throw(1, 4);
                switch (direction) {
                    case 1: { 
                        if (!(npc->get_x_cord() - npc->get_move_value() < 0 || 
                              npc->get_x_cord() - npc->get_move_value() > arr.get_map_size().first || 
                              npc->get_y_cord() < 0 || 
                              npc->get_y_cord() > arr.get_map_size().second)) {
                            npc->set_x(npc->get_x_cord() - npc->get_move_value());
                            do_step = true;
                        }
                        try_count--;
                    } break;
                    case 2: { 
                        if (!(npc->get_x_cord() + npc->get_move_value() < 0 || 
                              npc->get_x_cord() + npc->get_move_value() > arr.get_map_size().first || 
                              npc->get_y_cord() < 0 || 
                              npc->get_y_cord() > arr.get_map_size().second)) {
                            npc->set_x(npc->get_x_cord() + npc->get_move_value());
                            do_step = true;
                        }
                        try_count--;
                    } break;
                    case 3: { 
                        if (!(npc->get_x_cord() < 0 || 
                              npc->get_x_cord() > arr.get_map_size().first || 
                              npc->get_y_cord() + npc->get_move_value() < 0 || 
                              npc->get_y_cord() + npc->get_move_value() > arr.get_map_size().second)) {
                            npc->set_y(npc->get_y_cord() + npc->get_move_value());
                            do_step = true;
                        }
                        try_count--;
                    } break;
                    case 4: { 
                        if (!(npc->get_x_cord() < 0 || 
                              npc->get_x_cord() > arr.get_map_size().first || 
                              npc->get_y_cord() - npc->get_move_value() < 0 || 
                              npc->get_y_cord() - npc->get_move_value() > arr.get_map_size().second)) {
                            npc->set_y(npc->get_y_cord() - npc->get_move_value());
                            do_step = true;
                        }
                        try_count--;
                    } break;
                }
            }
            if (do_step == true) {
                notify(move_event_string(npc));
            }
        }
    }
};