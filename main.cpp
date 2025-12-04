//
//  main.cpp
//  Final Project
//
//  Created by Eric Perez on 12/4/25.
//

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <limits>
#include <iomanip>

using namespace std;

double randDouble(double min, double max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}

bool randChance(double p) {
    return randDouble(0.0, 1.0) < p;
}

class Env {
public:
    double temp;
    double hum;
    double light;
    int hour;

    Env() {
        temp = 22.0;
        hum = 60.0;
        light = 0.0;
        hour = 8;
    }

    void nextHour() {
        hour++;
        if (hour >= 24) {
            hour = 0;
        }
    }

    bool isDay() const {
        return hour >= 6 && hour <= 19;
    }

    void updateAir() {
        temp += randDouble(-0.4, 0.4);

        double target = 50.0;
        hum += (target - hum) * 0.02 + randDouble(-0.8, 0.8);

        if (hum < 20.0) hum = 20.0;
        if (hum > 90.0) hum = 90.0;

        if (isDay()) {
            if (light < 0.2) {
                light += 0.03;
                if (light > 0.2) light = 0.2;
            }
        } else {
            if (light > 0.0) {
                light -= 0.03;
                if (light < 0.0) light = 0.0;
            }
        }
    }
};

class Tank {
public:
    double water;
    double food;
    double ph;
    bool pumpOn;

    Tank() {
        water = 40.0;
        food = 40.0;
        ph = 6.0;
        pumpOn = true;
    }

    void addWater(double amount) {
        water += amount;
        if (water > 100.0) water = 100.0;
    }

    void addFood(double amount) {
        food += amount;
        if (food > 120.0) food = 120.0;
    }

    void changePh(double d) {
        ph += d;
        if (ph < 4.0) ph = 4.0;
        if (ph > 8.0) ph = 8.0;
    }

    void useFromTank(double w, double f) {
        water -= w;
        food -= f;
        if (water < 0.0) water = 0.0;
        if (food < 0.0) food = 0.0;

        if (randChance(0.2)) {
            changePh(randDouble(-0.04, 0.06));
        }
    }

    double getEC() const {
        if (water <= 0.0) return 0.0;
        return (food / water) * 10.0;
    }
};

class Plant {
public:
    string species;
    string name;
    double health;
    double size;
    double waterLevel;
    double foodLevel;
    bool alive;

    double tempLow;
    double tempHigh;
    double humLow;
    double humHigh;
    double lightLow;
    double lightHigh;
    double ecLow;
    double ecHigh;
    double phLow;
    double phHigh;

    double waterUse;
    double foodUse;
    double growSpeed;

    Plant() {
        species = "Lettuce";
        name = "Plant";
        health = 100.0;
        size = 0.0;
        waterLevel = 80.0;
        foodLevel = 80.0;
        alive = true;

        tempLow = 18.0;
        tempHigh = 23.0;
        humLow = 50.0;
        humHigh = 70.0;
        lightLow = 0.4;
        lightHigh = 0.8;
        ecLow = 1.0;
        ecHigh = 1.8;
        phLow = 5.8;
        phHigh = 6.5;
        waterUse = 0.12;
        foodUse = 0.08;
        growSpeed = 0.8;
    }

    string getStage() const {
        if (size < 20.0) return "Small";
        if (size < 60.0) return "Growing";
        if (size < 120.0) return "Big";
        return "Ready";
    }

    void makeLettuce() {
        species = "Lettuce";
        tempLow = 18.0;
        tempHigh = 23.0;
        humLow = 50.0;
        humHigh = 70.0;
        lightLow = 0.4;
        lightHigh = 0.8;
        ecLow = 1.0;
        ecHigh = 1.8;
        phLow = 5.8;
        phHigh = 6.5;
        waterUse = 0.12;
        foodUse = 0.08;
        growSpeed = 0.8;
    }

    void makeBasil() {
        species = "Basil";
        tempLow = 20.0;
        tempHigh = 26.0;
        humLow = 45.0;
        humHigh = 65.0;
        lightLow = 0.6;
        lightHigh = 0.95;
        ecLow = 1.4;
        ecHigh = 2.2;
        phLow = 5.5;
        phHigh = 6.5;
        waterUse = 0.10;
        foodUse = 0.07;
        growSpeed = 0.9;
    }

    void makeTomato() {
        species = "Tomato";
        tempLow = 21.0;
        tempHigh = 27.0;
        humLow = 50.0;
        humHigh = 70.0;
        lightLow = 0.7;
        lightHigh = 1.0;
        ecLow = 2.0;
        ecHigh = 2.8;
        phLow = 5.5;
        phHigh = 6.5;
        waterUse = 0.18;
        foodUse = 0.12;
        growSpeed = 1.1;
    }

    double updatePlant(const Env &env, Tank &tank) {
        if (!alive) return 0.0;

        if (tank.pumpOn && tank.water > 0.1) {
            tank.useFromTank(waterUse, foodUse);
            waterLevel += 1.0;
            foodLevel += 1.0;
        } else {
            waterLevel -= 2.5;
            foodLevel -= 2.0;
        }

        if (waterLevel > 100.0) waterLevel = 100.0;
        if (waterLevel < 0.0) waterLevel = 0.0;
        if (foodLevel > 100.0) foodLevel = 100.0;
        if (foodLevel < 0.0) foodLevel = 0.0;

        double stress = 0.0;

        if (env.temp < tempLow || env.temp > tempHigh) {
            stress += 5.0;
        }
        if (env.hum < humLow || env.hum > humHigh) {
            stress += 3.0;
        }
        if (env.light < lightLow || env.light > lightHigh) {
            stress += 4.0;
        }

        double ec = tank.getEC();
        if (ec < ecLow || ec > ecHigh) {
            stress += 4.0;
        }
        if (tank.ph < phLow || tank.ph > phHigh) {
            stress += 3.0;
        }

        if (waterLevel < 30.0) stress += 6.0;
        if (foodLevel < 30.0) stress += 4.0;

        if (stress == 0.0) {
            double g = growSpeed + randDouble(0.1, 0.3);
            size += g;
            health += 0.3;
        } else {
            double loss = stress * 0.15;
            health -= loss;
            double g = growSpeed - stress * 0.05;
            size += g;
            if (size < 0.0) size = 0.0;
        }

        if (health > 100.0) health = 100.0;
        if (health <= 0.0) {
            health = 0.0;
            alive = false;
        }

        return stress;
    }
};

class Control {
public:
    double tempLow;
    double tempHigh;
    double humLow;
    double humHigh;
    double lightLow;
    double lightHigh;
    double ecLow;
    double ecHigh;
    double phLow;
    double phHigh;

    bool heaterOn;
    bool mistOn;
    bool lightsOn;

    Control() {
        tempLow = 20.0;
        tempHigh = 24.0;
        humLow = 50.0;
        humHigh = 70.0;
        lightLow = 0.5;
        lightHigh = 0.9;
        ecLow = 1.2;
        ecHigh = 2.4;
        phLow = 5.5;
        phHigh = 6.5;

        heaterOn = false;
        mistOn = false;
        lightsOn = true;
    }

    void runControl(Env &env, Tank &tank) {
        if (env.temp < tempLow) {
            heaterOn = true;
            env.temp += 0.6;
        } else if (env.temp > tempHigh) {
            heaterOn = false;
            env.temp -= 0.5;
        } else {
            heaterOn = false;
        }

        if (env.hum < humLow) {
            mistOn = true;
            env.hum += 1.0;
        } else if (env.hum > humHigh) {
            mistOn = false;
            env.hum -= 0.9;
        } else {
            mistOn = false;
        }

        if (env.isDay()) {
            if (!lightsOn && randChance(0.9)) {
                lightsOn = true;
            }
        } else {
            if (lightsOn && randChance(0.95)) {
                lightsOn = false;
            }
        }

        if (lightsOn) {
            if (env.light < lightLow) {
                env.light += 0.1;
            } else if (env.light > lightHigh) {
                env.light -= 0.05;
            }
            if (env.light < 0.0) env.light = 0.0;
            if (env.light > 1.0) env.light = 1.0;
        }

        double ec = tank.getEC();
        if (ec < ecLow) {
            tank.addFood(0.4);
        }

        if (tank.ph < phLow) {
            tank.changePh(0.05);
        } else if (tank.ph > phHigh) {
            tank.changePh(-0.05);
        }
    }
};

class GreenhouseSim {
public:
    Env env;
    Tank tank;
    Control ctrl;
    vector<Plant> plants;
    int day;
    int simHour;
    double stressTotal;
    bool pumpBroken;
    bool powerOff;

    GreenhouseSim() {
        day = 1;
        simHour = 0;
        stressTotal = 0.0;
        pumpBroken = false;
        powerOff = false;

        Plant p1;
        p1.makeLettuce();
        p1.name = "Lettuce A";

        Plant p2;
        p2.makeBasil();
        p2.name = "Basil A";

        Plant p3;
        p3.makeTomato();
        p3.name = "Tomato A";

        plants.push_back(p1);
        plants.push_back(p2);
        plants.push_back(p3);
    }

    void printHeader() const {
        cout << "================ GREENHOUSE SIM ================\n";
        cout << "Day " << day << " | ";
        cout << setw(2) << setfill('0') << env.hour << ":00";
        cout << setfill(' ') << "   (Hour " << simHour << ")\n";
        cout << "===============================================\n\n";
    }

    void showInfo() const {
        cout << fixed << setprecision(1);
        cout << "Room:\n";
        cout << "  Temp : " << env.temp << " C\n";
        cout << "  Hum  : " << env.hum << " %\n";
        cout << "  Light: " << env.light << "  (Lights " << (ctrl.lightsOn ? "ON" : "OFF") << ")\n";
        cout << "  Time : " << env.hour << ":00 " << (env.isDay() ? "[Day]\n" : "[Night]\n");

        cout << "\nTank:\n";
        cout << "  Water: " << tank.water << " L\n";
        cout << "  Food : " << tank.food << " units   EC ~ " << tank.getEC() << "\n";
        cout << "  pH   : " << tank.ph << "\n";
        cout << "  Pump : " << (tank.pumpOn ? "ON" : "OFF");
        if (pumpBroken) cout << " [BROKEN]";
        cout << "\n";

        cout << "\nStatus:\n";
        cout << "  Power off : " << (powerOff ? "YES" : "NO") << "\n";
        cout << "  Total stress: " << stressTotal << "\n\n";
    }

    void showPlants() const {
        cout << "Plants:\n";
        if (plants.empty()) {
            cout << "  (no plants)\n\n";
            return;
        }
        cout << left << setw(15) << "Name"
             << setw(12) << "Type"
             << setw(10) << "Health"
             << setw(12) << "Size"
             << setw(10) << "Stage"
             << setw(8) << "Alive" << "\n";

        cout << string(65, '-') << "\n";

        for (size_t i = 0; i < plants.size(); i++) {
            const Plant &p = plants[i];
            cout << left << setw(15) << p.name
                 << setw(12) << p.species
                 << setw(10) << fixed << setprecision(1) << p.health
                 << setw(12) << p.size
                 << setw(10) << p.getStage()
                 << setw(8) << (p.alive ? "Yes" : "No") << "\n";
        }
        cout << "\n";
    }

    void addPlant() {
        cout << "Add plant:\n";
        cout << "1) Lettuce\n";
        cout << "2) Basil\n";
        cout << "3) Tomato\n";
        cout << "Choice: ";
        int choice;
        cin >> choice;
        if (!cin || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Bad choice.\n\n";
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string nickname;
        cout << "Plant name: ";
        getline(cin, nickname);
        if (nickname.empty()) nickname = "Plant";

        Plant p;
        if (choice == 1) p.makeLettuce();
        else if (choice == 2) p.makeBasil();
        else p.makeTomato();

        p.name = nickname;
        plants.push_back(p);

        cout << "Added " << p.species << " called \"" << p.name << "\".\n\n";
    }

    void removePlant() {
        if (plants.empty()) {
            cout << "No plants to remove.\n\n";
            return;
        }

        cout << "Pick plant to remove:\n";
        for (size_t i = 0; i < plants.size(); i++) {
            cout << (i + 1) << ") " << plants[i].name << " (" << plants[i].species << ")\n";
        }
        cout << "Choice: ";
        int idx;
        cin >> idx;
        if (!cin || idx < 1 || idx > (int)plants.size()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Bad index.\n\n";
            return;
        }
        string removedName = plants[idx - 1].name;
        plants.erase(plants.begin() + (idx - 1));
        cout << "Removed " << removedName << ".\n\n";
    }

    void changeTargets() {
        cout << "Change control values:\n";
        cout << "1) Temp\n";
        cout << "2) Humidity\n";
        cout << "3) Light\n";
        cout << "4) EC\n";
        cout << "5) pH\n";
        cout << "Choice: ";
        int c;
        cin >> c;
        if (!cin || c < 1 || c > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Bad choice.\n\n";
            return;
        }
        double minVal, maxVal;
        cout << "New min: ";
        cin >> minVal;
        cout << "New max: ";
        cin >> maxVal;
        if (minVal > maxVal) {
            double tmp = minVal;
            minVal = maxVal;
            maxVal = tmp;
        }

        if (c == 1) {
            ctrl.tempLow = minVal;
            ctrl.tempHigh = maxVal;
        } else if (c == 2) {
            ctrl.humLow = minVal;
            ctrl.humHigh = maxVal;
        } else if (c == 3) {
            ctrl.lightLow = minVal;
            ctrl.lightHigh = maxVal;
        } else if (c == 4) {
            ctrl.ecLow = minVal;
            ctrl.ecHigh = maxVal;
        } else if (c == 5) {
            ctrl.phLow = minVal;
            ctrl.phHigh = maxVal;
        }

        cout << "Updated.\n\n";
    }

    void fillTank() {
        tank.addWater(40.0);
        tank.addFood(20.0);
        cout << "Tank refilled.\n\n";
    }

    void flipPump() {
        tank.pumpOn = !tank.pumpOn;
        cout << "Pump is now " << (tank.pumpOn ? "ON" : "OFF") << ".\n\n";
    }

    void clearProblems() {
        pumpBroken = false;
        powerOff = false;
        cout << "Problems cleared.\n\n";
    }

    void randomStuff() {
        if (!pumpBroken && randChance(0.02)) {
            pumpBroken = true;
            tank.pumpOn = false;
            cout << "[EVENT] Pump got stuck.\n";
        }

        if (!powerOff && randChance(0.01)) {
            powerOff = true;
            cout << "[EVENT] Power went out.\n";
        }

        if (powerOff && randChance(0.1)) {
            powerOff = false;
            cout << "[EVENT] Power came back.\n";
        }

        if (randChance(0.01)) {
            cout << "[EVENT] Small bug problem, plants stressed.\n";
            stressTotal += 5.0;
        }
    }

    void runHour() {
        simHour++;
        env.nextHour();
        env.updateAir();
        randomStuff();

        if (!powerOff) {
            ctrl.runControl(env, tank);
        }

        if (pumpBroken) {
            tank.pumpOn = false;
        }

        double hourStress = 0.0;
        for (size_t i = 0; i < plants.size(); i++) {
            hourStress += plants[i].updatePlant(env, tank);
        }
        stressTotal += hourStress;

        if (tank.water < 5.0) {
            stressTotal += 10.0;
        }

        if (env.hour == 0) {
            day++;
        }
    }

    void skipHours(int h) {
        for (int i = 0; i < h; i++) {
            runHour();
        }
        cout << "Skipped " << h << " hour(s).\n\n";
    }

    void showMenu() {
        cout << "================ MENU ================\n";
        cout << "1) View info\n";
        cout << "2) +1 hour\n";
        cout << "3) +6 hours\n";
        cout << "4) +24 hours\n";
        cout << "5) Add plant\n";
        cout << "6) Remove plant\n";
        cout << "7) Change control\n";
        cout << "8) Refill tank\n";
        cout << "9) Toggle pump\n";
        cout << "10) Clear problems\n";
        cout << "0) Quit\n";
        cout << "Choice: ";
    }

    void run() {
        bool running = true;
        while (running) {
            printHeader();
            showMenu();

            int choice;
            cin >> choice;
            if (!cin) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Bad input.\n";
                continue;
            }
            cout << "\n";

            if (choice == 1) {
                showInfo();
                showPlants();
            } else if (choice == 2) {
                skipHours(1);
            } else if (choice == 3) {
                skipHours(6);
            } else if (choice == 4) {
                skipHours(24);
            } else if (choice == 5) {
                addPlant();
            } else if (choice == 6) {
                removePlant();
            } else if (choice == 7) {
                changeTargets();
            } else if (choice == 8) {
                fillTank();
            } else if (choice == 9) {
                flipPump();
            } else if (choice == 10) {
                clearProblems();
            } else if (choice == 0) {
                running = false;
                cout << "Goodbye.\n";
            } else {
                cout << "Unknown option.\n";
            }
        }
    }
};

int main() {
    GreenhouseSim sim;
    sim.run();
    return 0;
}
