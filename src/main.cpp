#include <Arduino.h>

#include <iostream>
#include <random>
#include <cstdio>
#include <fstream>
#include <string>
#include <iomanip> //set precision

//time implementation
#include <chrono>
#include <thread>
using namespace std::chrono_literals;
//
using namespace std;

class TempSensorSim {
    public:
        double read() {
            static std::mt19937 gen(std::random_device{}());
            static std::uniform_real_distribution<> dist(19.0, 30.0); //random values between ()
            return dist(gen);
        }
};



int main() {

    //creation d'un fichier CSV "export"
    ofstream fichier("export.csv"); 

    //en-tête CSV
    fichier << "timestamp, temperature \n"; 

    TempSensorSim capteur; //obj declaration



    if (fichier.is_open()) {

        //format the value with 2 digits
        fichier << fixed << setprecision(2); 

            for (int i = 0; i < 10; i++) {
    auto now = chrono::system_clock::now();
    auto now_c = chrono::system_clock::to_time_t(now);

    double valeur = capteur.read();  // Lecture d'une valeur aléatoire

    // écriture CSV : timestamp, valeur
    fichier << put_time(localtime(&now_c), "%F %T") 
            << "," 
            << fixed << setprecision(2) << valeur 
            << "\n";

    // Affichage console
    printf("%.2lf\n", valeur);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Pause 1s
}



    fichier.close();
}


return 0;
}