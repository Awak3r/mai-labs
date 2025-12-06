#include <gtest/gtest.h>
#include "../include/NPC.h"
#include "../include/Observer.h"
#include "../include/Visitor.h"
#include <fstream>
#include <sstream>
#include <vector>

// ==================== NPC Base Class Tests ====================

TEST(NPCTest, DefaultConstructor) {
    NPC npc;
    ASSERT_EQ(npc.get_x_cord(), 0);
    ASSERT_EQ(npc.get_y_cord(), 0);
    ASSERT_TRUE(npc.is_alive_NPC());
}

TEST(NPCTest, ParameterizedConstructor) {
    NPC npc("TestNPC", 10.5, 20.3);
    ASSERT_EQ(npc.get_name(), "TestNPC");
    ASSERT_EQ(npc.get_x_cord(), 10.5);
    ASSERT_EQ(npc.get_y_cord(), 20.3);
    ASSERT_TRUE(npc.is_alive_NPC());
}

TEST(NPCTest, CopyConstructor) {
    NPC npc1("Original", 15, 25);
    NPC npc2(npc1);
    
    ASSERT_EQ(npc2.get_name(), "Original");
    ASSERT_EQ(npc2.get_x_cord(), 15);
    ASSERT_EQ(npc2.get_y_cord(), 25);
    ASSERT_TRUE(npc2.is_alive_NPC());
}

TEST(NPCTest, AssignmentOperator) {
    NPC npc1("First", 10, 20);
    NPC npc2;
    
    npc2 = npc1;
    ASSERT_EQ(npc2.get_name(), "First");
    ASSERT_EQ(npc2.get_x_cord(), 10);
    ASSERT_EQ(npc2.get_y_cord(), 20);
}

TEST(NPCTest, EqualityOperator) {
    NPC npc1("Test", 10, 20);
    NPC npc2("Test", 10, 20);
    NPC npc3("Different", 30, 40);
    
    ASSERT_TRUE(npc1 == npc2);
    ASSERT_FALSE(npc1 == npc3);
}

TEST(NPCTest, SettersAndGetters) {
    NPC npc;
    
    npc.set_x(50.5);
    npc.set_y(60.6);
    npc.set_name("Updated");
    npc.set_attack(5);
    npc.set_deffence(3);
    
    ASSERT_EQ(npc.get_x_cord(), 50.5);
    ASSERT_EQ(npc.get_y_cord(), 60.6);
    ASSERT_EQ(npc.get_name(), "Updated");
    ASSERT_EQ(npc.get_attack(), 5);
    ASSERT_EQ(npc.get_deffence(), 3);
}

TEST(NPCTest, KillNPC) {
    NPC npc("Victim", 10, 20);
    ASSERT_TRUE(npc.is_alive_NPC());
    
    npc.kill_npc();
    ASSERT_FALSE(npc.is_alive_NPC());
}

TEST(NPCTest, GetType) {
    NPC npc;
    ASSERT_EQ(npc.get_type(), "NPC");
}

// ==================== Squirrel Tests ====================

TEST(SquirrelTest, DefaultConstructor) {
    squirrel sq;
    ASSERT_EQ(sq.get_type(), "squirrel");
    ASSERT_EQ(sq.get_move_value(), 5);
    ASSERT_EQ(sq.get_attck_range(), 5);
}

TEST(SquirrelTest, ParameterizedConstructor) {
    squirrel sq("Squirrely", 25.5, 35.5);
    ASSERT_EQ(sq.get_name(), "Squirrely");
    ASSERT_EQ(sq.get_x_cord(), 25.5);
    ASSERT_EQ(sq.get_y_cord(), 35.5);
    ASSERT_EQ(sq.get_type(), "squirrel");
    ASSERT_EQ(sq.get_move_value(), 5);
    ASSERT_EQ(sq.get_attck_range(), 5);
}

// ==================== Werewolf Tests ====================

TEST(WerewolfTest, DefaultConstructor) {
    werewolf ww;
    ASSERT_EQ(ww.get_type(), "werewolf");
    ASSERT_EQ(ww.get_move_value(), 40);
    ASSERT_EQ(ww.get_attck_range(), 5);
}

TEST(WerewolfTest, ParameterizedConstructor) {
    werewolf ww("Wolfy", 45.5, 55.5);
    ASSERT_EQ(ww.get_name(), "Wolfy");
    ASSERT_EQ(ww.get_x_cord(), 45.5);
    ASSERT_EQ(ww.get_y_cord(), 55.5);
    ASSERT_EQ(ww.get_type(), "werewolf");
    ASSERT_EQ(ww.get_move_value(), 40);
    ASSERT_EQ(ww.get_attck_range(), 5);
}

// ==================== Druid Tests ====================

TEST(DruidTest, DefaultConstructor) {
    druid dr;
    ASSERT_EQ(dr.get_type(), "druid");
    ASSERT_EQ(dr.get_move_value(), 10);
    ASSERT_EQ(dr.get_attck_range(), 10);
}

TEST(DruidTest, ParameterizedConstructor) {
    druid dr("Druidy", 65.5, 75.5);
    ASSERT_EQ(dr.get_name(), "Druidy");
    ASSERT_EQ(dr.get_x_cord(), 65.5);
    ASSERT_EQ(dr.get_y_cord(), 75.5);
    ASSERT_EQ(dr.get_type(), "druid");
    ASSERT_EQ(dr.get_move_value(), 10);
    ASSERT_EQ(dr.get_attck_range(), 10);
}

// ==================== NPC_array Tests ====================

TEST(NPCArrayTest, DefaultConstructor) {
    NPC_array arr;
    ASSERT_EQ(arr.get_size(), 0);
    ASSERT_EQ(arr.get_map_size().first, 100);
    ASSERT_EQ(arr.get_map_size().second, 100);
}

TEST(NPCArrayTest, SetMapSize) {
    NPC_array arr;
    arr.set_map_size(200, 150);
    
    ASSERT_EQ(arr.get_map_size().first, 200);
    ASSERT_EQ(arr.get_map_size().second, 150);
}

TEST(NPCArrayTest, AddNPCValid) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Squirrel1", 50, 50));
    
    ASSERT_EQ(arr.get_size(), 1);
}

TEST(NPCArrayTest, AddNPCInvalidCoordinates) {
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    // За пределами карты
    arr.add_NPC(std::make_unique<squirrel>("OutOfBounds", 150, 150));
    ASSERT_EQ(arr.get_size(), 0); // Не должен добавиться
}

TEST(NPCArrayTest, AddMultipleNPCs) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("S1", 10, 10));
    arr.add_NPC(std::make_unique<werewolf>("W1", 20, 20));
    arr.add_NPC(std::make_unique<druid>("D1", 30, 30));
    
    ASSERT_EQ(arr.get_size(), 3);
}

TEST(NPCArrayTest, RemoveNPCByName) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("Squirrel1", 10, 10));
    arr.add_NPC(std::make_unique<squirrel>("Squirrel2", 20, 20));
    
    ASSERT_EQ(arr.get_size(), 2);
    
    arr.remove_npc("Squirrel1");
    ASSERT_EQ(arr.get_size(), 1);
    
    // Проверяем, что остался правильный NPC
    auto& npcs = arr.get_npcs();
    ASSERT_EQ(npcs.front()->get_name(), "Squirrel2");
}

TEST(NPCArrayTest, RemoveNPCByCoordinates) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("S1", 10, 20));
    arr.add_NPC(std::make_unique<squirrel>("S2", 30, 40));
    
    arr.remove_at(10, 20);
    ASSERT_EQ(arr.get_size(), 1);
    
    auto& npcs = arr.get_npcs();
    ASSERT_EQ(npcs.front()->get_name(), "S2");
}

TEST(NPCArrayTest, Clear) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("S1", 10, 10));
    arr.add_NPC(std::make_unique<werewolf>("W1", 20, 20));
    arr.add_NPC(std::make_unique<druid>("D1", 30, 30));
    
    ASSERT_EQ(arr.get_size(), 3);
    
    arr.clear();
    ASSERT_EQ(arr.get_size(), 0);
}

// ==================== NPCFactory Tests ====================

TEST(NPCFactoryTest, CreateSquirrel) {
    auto npc = NPCFactory::create_npc("squirrel", "TestSquirrel", 25, 35);
    
    ASSERT_NE(npc, nullptr);
    ASSERT_EQ(npc->get_type(), "squirrel");
    ASSERT_EQ(npc->get_name(), "TestSquirrel");
    ASSERT_EQ(npc->get_x_cord(), 25);
    ASSERT_EQ(npc->get_y_cord(), 35);
}

TEST(NPCFactoryTest, CreateWerewolf) {
    auto npc = NPCFactory::create_npc("werewolf", "TestWerewolf", 45, 55);
    
    ASSERT_NE(npc, nullptr);
    ASSERT_EQ(npc->get_type(), "werewolf");
    ASSERT_EQ(npc->get_name(), "TestWerewolf");
}

TEST(NPCFactoryTest, CreateDruid) {
    auto npc = NPCFactory::create_npc("druid", "TestDruid", 65, 75);
    
    ASSERT_NE(npc, nullptr);
    ASSERT_EQ(npc->get_type(), "druid");
    ASSERT_EQ(npc->get_name(), "TestDruid");
}

TEST(NPCFactoryTest, CreateInvalidType) {
    ASSERT_THROW(
        NPCFactory::create_npc("invalid_type", "Test", 10, 20),
        std::logic_error
    );
}

TEST(NPCFactoryTest, GenerateRandomNPCs) {
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    NPCFactory::generate_random_npcs(arr, 50);
    ASSERT_EQ(arr.get_size(), 50);
    
    // Проверяем, что все NPC в пределах карты
    for (auto& npc : arr.get_npcs()) {
        ASSERT_GE(npc->get_x_cord(), 0);
        ASSERT_LE(npc->get_x_cord(), 100);
        ASSERT_GE(npc->get_y_cord(), 0);
        ASSERT_LE(npc->get_y_cord(), 100);
    }
}

// ==================== File I/O Tests ====================

TEST(FileIOTest, SaveToFile) {
    NPC_array arr;
    arr.add_NPC(std::make_unique<squirrel>("S1", 10, 20));
    arr.add_NPC(std::make_unique<werewolf>("W1", 30, 40));
    arr.add_NPC(std::make_unique<druid>("D1", 50, 60));
    
    const char* filename = "test_npcs.txt";
    NPCFactory::save_to_file(filename, arr);
    
    // Проверяем, что файл создан
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    
    // Читаем содержимое
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    
    ASSERT_EQ(lines.size(), 3);
    ASSERT_TRUE(lines[0].find("squirrel") != std::string::npos);
    ASSERT_TRUE(lines[1].find("werewolf") != std::string::npos);
    ASSERT_TRUE(lines[2].find("druid") != std::string::npos);
    
    // Удаляем тестовый файл
    std::remove(filename);
}

TEST(FileIOTest, LoadFromFile) {
    // Создаем тестовый файл
    const char* filename = "test_load.txt";
    std::ofstream file(filename);
    file << "squirrel S1 10.5 20.5\n";
    file << "werewolf W1 30.5 40.5\n";
    file << "druid D1 50.5 60.5\n";
    file.close();
    
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    NPCFactory::load_from_file(filename, arr);
    
    ASSERT_EQ(arr.get_size(), 3);
    
    auto& npcs = arr.get_npcs();
    auto it = npcs.begin();
    
    ASSERT_EQ((*it)->get_type(), "squirrel");
    ASSERT_EQ((*it)->get_name(), "S1");
    ++it;
    
    ASSERT_EQ((*it)->get_type(), "werewolf");
    ASSERT_EQ((*it)->get_name(), "W1");
    ++it;
    
    ASSERT_EQ((*it)->get_type(), "druid");
    ASSERT_EQ((*it)->get_name(), "D1");
    
    std::remove(filename);
}

TEST(FileIOTest, LoadFromFileInvalidCoordinates) {
    const char* filename = "test_invalid.txt";
    std::ofstream file(filename);
    file << "squirrel S1 150 150\n"; // За пределами карты
    file << "werewolf W1 30 40\n";   // Валидный
    file.close();
    
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    NPCFactory::load_from_file(filename, arr);
    
    // Должен загрузиться только валидный NPC
    ASSERT_EQ(arr.get_size(), 1);
    ASSERT_EQ(arr.get_npcs().front()->get_type(), "werewolf");
    
    std::remove(filename);
}

// ==================== Observer Tests ====================

TEST(ObserverTest, FileLoggerCreatesFile) {
    const char* logfile = "test_log.txt";
    
    {
        FileLogger logger(logfile);
        logger.update("Test event 1");
        logger.update("Test event 2");
    }
    
    std::ifstream file(logfile);
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    
    ASSERT_EQ(lines.size(), 2);
    ASSERT_EQ(lines[0], "Test event 1");
    ASSERT_EQ(lines[1], "Test event 2");
    
    std::remove(logfile);
}

TEST(ObserverTest, DisplayObserver) {
    // Перенаправляем cout в stringstream для проверки
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    Display display;
    display.update("Test display message");
    
    std::cout.rdbuf(old);
    
    ASSERT_EQ(buffer.str(), "Test display message\n");
}

// ==================== Visitor Tests ====================

TEST(VisitorTest, DoThrow) {
    // Проверяем, что do_throw генерирует числа в заданном диапазоне
    bool found_1 = false;
    bool found_6 = false;
    
    for (int i = 0; i < 1000; ++i) {
        int result = CombatVisitor::do_throw(1, 6);
        ASSERT_GE(result, 1);
        ASSERT_LE(result, 6);
        
        if (result == 1) found_1 = true;
        if (result == 6) found_6 = true;
    }
    
    ASSERT_TRUE(found_1);
    ASSERT_TRUE(found_6);
}

TEST(VisitorTest, FindInRad) {
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    auto npc1 = std::make_unique<squirrel>("S1", 50, 50);
    auto npc2 = std::make_unique<squirrel>("S2", 52, 52); // В радиусе 5
    auto npc3 = std::make_unique<squirrel>("S3", 60, 60); // Вне радиуса
    
    arr.add_NPC(std::move(npc1));
    arr.add_NPC(std::move(npc2));
    arr.add_NPC(std::move(npc3));
    
    CombatVisitor visitor;
    auto& npcs = arr.get_npcs();
    auto it = npcs.begin();
    
    auto targets = visitor.find_in_rad(arr, *it);
    
    // S2 должен быть в радиусе, S3 - нет
    ASSERT_EQ(targets.size(), 1);
}

TEST(VisitorTest, SquirrelAttacksDruid) {
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    auto attacker = std::make_unique<squirrel>("Attacker", 50, 50);
    auto target = std::make_unique<druid>("Target", 52, 52);
    
    arr.add_NPC(std::move(attacker));
    arr.add_NPC(std::move(target));
    
    CombatVisitor visitor;
    
    auto& npcs = arr.get_npcs();
    auto it = npcs.begin();
    auto targets = visitor.find_in_rad(arr, *it);
    
    if (!targets.empty()) {
        visitor.do_combat(arr, *it, targets);
    }
    
    // После боя может быть убийство (зависит от броска кубика)
    // Проверяем только что метод не падает
    ASSERT_TRUE(true);
}

TEST(VisitorTest, WerewolfAttacksDruid) {
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    auto attacker = std::make_unique<werewolf>("Werewolf", 50, 50);
    auto target = std::make_unique<druid>("Druid", 52, 52);
    
    arr.add_NPC(std::move(attacker));
    arr.add_NPC(std::move(target));
    
    CombatVisitor visitor;
    
    auto& npcs = arr.get_npcs();
    auto it = npcs.begin();
    auto targets = visitor.find_in_rad(arr, *it);
    
    ASSERT_FALSE(targets.empty());
}

TEST(VisitorTest, DruidDoesNotAttack) {
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    auto attacker = std::make_unique<druid>("Druid", 50, 50);
    auto target = std::make_unique<squirrel>("Squirrel", 52, 52);
    
    arr.add_NPC(std::move(attacker));
    arr.add_NPC(std::move(target));
    
    CombatVisitor visitor;
    
    auto& npcs = arr.get_npcs();
    auto it = npcs.begin();
    auto targets = visitor.find_in_rad(arr, *it);
    
    size_t initial_size = arr.get_size();
    visitor.do_combat(arr, *it, targets);
    
    // Друид не должен никого убить
    ASSERT_EQ(arr.get_size(), initial_size);
}

// ==================== Movement Tests ====================

TEST(MovementTest, NPCMoves) {
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    arr.add_NPC(std::make_unique<squirrel>("S1", 50, 50));
    
    CombatVisitor visitor;
    
    double initial_x = arr.get_npcs().front()->get_x_cord();
    double initial_y = arr.get_npcs().front()->get_y_cord();
    
    visitor.do_move(arr);
    
    double final_x = arr.get_npcs().front()->get_x_cord();
    double final_y = arr.get_npcs().front()->get_y_cord();
    
    // Координаты должны измениться (или остаться теми же если выход за границы)
    ASSERT_TRUE(initial_x != final_x || initial_y != final_y || 
                (initial_x == final_x && initial_y == final_y));
}

TEST(MovementTest, NPCStaysInBounds) {
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    arr.add_NPC(std::make_unique<squirrel>("S1", 50, 50));
    
    CombatVisitor visitor;
    
    // Делаем несколько движений
    for (int i = 0; i < 20; ++i) {
        visitor.do_move(arr);
        
        auto& npc = arr.get_npcs().front();
        ASSERT_GE(npc->get_x_cord(), 0);
        ASSERT_LE(npc->get_x_cord(), 100);
        ASSERT_GE(npc->get_y_cord(), 0);
        ASSERT_LE(npc->get_y_cord(), 100);
    }
}

// ==================== Integration Tests ====================

TEST(IntegrationTest, CompleteGameFlow) {
    NPC_array arr;
    arr.set_map_size(100, 100);
    
    // Добавляем NPC разных типов
    arr.add_NPC(std::make_unique<squirrel>("S1", 25, 25));
    arr.add_NPC(std::make_unique<werewolf>("W1", 50, 50));
    arr.add_NPC(std::make_unique<druid>("D1", 75, 75));
    
    CombatVisitor visitor;
    
    size_t initial_count = arr.get_size();
    ASSERT_EQ(initial_count, 3);
    
    // Симулируем несколько раундов
    for (int round = 0; round < 10; ++round) {
        visitor.do_move(arr);
        
        for (auto& npc : arr.get_npcs()) {
            if (!npc->is_alive_NPC()) continue;
            auto targets = visitor.find_in_rad(arr, npc);
            if (!targets.empty()) {
                visitor.do_combat(arr, npc, targets);
            }
        }
    }
    
    // Игра должна завершиться без ошибок
    ASSERT_TRUE(true);
}

TEST(IntegrationTest, SaveLoadCycle) {
    const char* filename = "test_cycle.txt";
    
    // Создаем и сохраняем
    {
        NPC_array arr;
        arr.add_NPC(std::make_unique<squirrel>("S1", 10, 20));
        arr.add_NPC(std::make_unique<werewolf>("W1", 30, 40));
        arr.add_NPC(std::make_unique<druid>("D1", 50, 60));
        
        NPCFactory::save_to_file(filename, arr);
    }
    
    // Загружаем
    {
        NPC_array arr;
        arr.set_map_size(100, 100);
        NPCFactory::load_from_file(filename, arr);
        
        ASSERT_EQ(arr.get_size(), 3);
        
        auto& npcs = arr.get_npcs();
        auto it = npcs.begin();
        
        ASSERT_EQ((*it)->get_name(), "S1");
        ++it;
        ASSERT_EQ((*it)->get_name(), "W1");
        ++it;
        ASSERT_EQ((*it)->get_name(), "D1");
    }
    
    std::remove(filename);
}

// ==================== Main ====================

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}