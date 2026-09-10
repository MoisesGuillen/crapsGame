// |    CRAPS  GAME     |
// Moises Guillen
// 10:10 PM - 9/9/26
#include <algorithm>
#include <iostream>
#include <random>
#include <print>

int dieRoll(std::mt19937& mt_alpha){

    // Generate uniform numbers between 1 and 6
    static std::uniform_int_distribution<> die_6{1,6};

    auto die1{ die_6(mt_alpha)};
    auto die2{ die_6(mt_alpha)};
    auto dieTotal{die1+die2};
    std::cout << '\t' << " DIE #" << 1 << ".  " << die1 << '\n';
    std::cout << '\t' << " DIE #" << 2 << ".  " << die2 << '\n';
    std::cout << '\t' << " TOTAL : " << dieTotal << "\n";

    return dieTotal;
}

int main(int argc, char *argv[]) {
    // PRNG Setup
    // Acquire X random ints from random_device for our seed
    std::random_device rd_alpha{};
    std::array<unsigned,100> seeds{};
    std::generate(seeds.begin(),seeds.end(), std::ref(rd_alpha));
    std::seed_seq ss_alpha{ seeds.begin(), seeds.end() };

    // Initialize our MT with the std::seed_seq
    std::mt19937 mt_alpha{ ss_alpha };

    constexpr int starting_bank_roll{1000};
    auto bank_roll{starting_bank_roll};

    char play_again{'y'};
    while (play_again == 'y' || play_again == 'Y') {

        std::print("|*                 *|\n");
        std::print("|    CRAPS  GAME    |\n");
        std::print("|*                 *|\n");

        std::cout << "\tBANKROLL $" << bank_roll << "\n";
        std::cout << "\tPLACE_BET $";
        auto bet{0};

        while ( !(std::cin >> bet) || bet <= 0 || bet > bank_roll ) {
            std::cin.clear();   // clears ERROR flag if USER types letters
            std::cin.ignore(10000,'\n'); // discards trash input
            std::cout << "\tINVALID! Enter $1 to $" << bank_roll << ": $";
        }

        std::cin.ignore(10000, '\n'); // Throws away extra garbage (like .99) left
        std::cout << "\n";

        auto first_roll{dieRoll(mt_alpha)};
        std::cout << "\n\t\t";

        if (first_roll == 7 || first_roll == 11) {
            std::cout << "U WIN!\n";
            bank_roll += bet;
        } else if (first_roll == 2 || first_roll == 3 || first_roll == 12) {
            std::cout << "U LOSE\n";
            bank_roll -= bet;
        } else {
            std::cout << " Point is " << first_roll << "\n\n";
            std::cout << "--- ROLLING FOR POINT ---\n\n";

            while (true) {
                auto next_roll{dieRoll(mt_alpha)};
                std::cout << '\n';
                if (next_roll == first_roll) {
                    std::cout << "\tU WIN! {Hit the Point}\n";
                    bank_roll += bet;
                    break;
                } else if (next_roll == 7) {
                    std::cout << "\tSEVEN OUT! U LOSE\n";
                    bank_roll -= bet;
                    break;
                }
            }     // If anything else , loop runs again!
        }


        if (bank_roll <= 0) {std::cout << "\nU RAN OUT OF MONEY! GET F*CKED LOL \n"; break;}

        std::cout << "\n\tPLAY AGAIN? (Y/N): ";
        std::cin >> play_again; std::cout << "\n\n";
    }

    std::cout << "    CASHING OUT...    \n";
    std::cout << "\tFINAL BANKROLL $" << bank_roll << '\n';

    auto profit{ bank_roll - starting_bank_roll };
    if (profit > 0) {
        std::cout << "\tNET PROFIT " << "    $" << profit << "\n";
    } else if (profit < 0) {
        std::cout << "\tNET LOSS " << "     -$" << (profit * -1) << "\n";
    } else {
        std::cout << "\tBROKE EVEN!\n";
    }

    return {};
}
