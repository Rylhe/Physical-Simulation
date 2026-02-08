//
// Created by 71907 on 2025/4/29.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <iostream>

using namespace std;

class Player {
  public:
    // Basic data
    string player_name; // the full name of the player
    int number; // the player’s number
    int height; // the player’s height (in cm)
    int weight; // the player’s weight (in kg)
    bool left_footed; // whether the player’s dominant foot is the left one
    int speed, stamina, jumping, shooting, passing;
    int dribbling, tackling, marking, strength;
    float x[2]; // 2D coordinates of player’s position on the pitch (in m)
    float v[2]; // 2D vector with player’s speed components (in m/s)

    // Constructor
    Player(const string& name, int num) {
      player_name = name;
      number = num;

      height = weight = 0;
      left_footed = false;
      speed = stamina = jumping = shooting = passing = 0;
      dribbling = tackling = marking = strength = 0;
      x[0] = x[1] = 0.0f;
      v[0] = v[1] = 0.0f;
    }

    // Functions
    int get_physical_ability() const {
      return (speed + stamina + jumping) / 3 ;
    }
    int get_offence_ability() const {
      return (shooting + passing + dribbling) / 3 ;
    }
    int get_defence_ability() const {
      return (marking + tackling + strength) / 3 ;
    }

    // Summary
    void printPlayerInfo() const {
    cout << "Player Name: " << player_name
              << ", Number: " << number << "\n"
              << " Physical : " << get_physical_ability()
              << ", Offence: " << get_offence_ability()
              << ", Strength: " << get_defence_ability()
              << endl;
    }
    // Movement
    void move() {
      x[0] += v[0] * 0.01f;
      x[1] += v[1] * 0.01f;
    }
    void update_speed(float v_new[2]) {
      v[0] = v_new[0];
      v[1] = v_new[1];
    }
};

#endif //PLAYER_HPP
