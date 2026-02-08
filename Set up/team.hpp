//
// Created by 71907 on 2025/4/29.
//

#ifndef TEAM_HPP
#define TEAM_HPP

#include "player.hpp"
#include <list>
#include <string>
#include <iostream>

using namespace std;

class Team {
private:
  // Variables
  string name;
  string colour; // the team’s colour
  list<Player> PlayerList; // list container of Player objects

public:
  // Constructor
  Team(const string& nm, const string& col, const list<Player>& PL) {
    name = nm;
    colour = col;
    PlayerList = PL;
  };

  // void Functions
  string get_name() const {
    return name;
  }
  string get_colour() const {
    return colour;
  }
  list<Player> get_PlayerList() const {
    return PlayerList;
  }
  void set_name(const string& nm ) {
    name = nm;
  }
  void set_colour(const string& col ) {
    colour = col;
  }
  void set_PlayerList(const list<Player>& PL ) {
    PlayerList = PL;
  }

  void addPlayer(const Player& P) {
    PlayerList.push_front(P);
  }

  void printPlayersList() const {
    for (auto& p : PlayerList) {
      cout << p.number << " : " << p.player_name << "\n" << endl;
    }
  }

  int get_offence_ability() const {
    if (PlayerList.empty()) {
      return 0;
    }
    int sum = 0;
    for (auto& p : PlayerList) {
      sum += p.get_offence_ability();
    }
    return sum / PlayerList.size();
  }
  int get_defence_ability() const {
    if (PlayerList.empty()) {
      return 0;
    }
    int sum = 0;
    for (auto& p : PlayerList) {
      sum += p.get_defence_ability();
    }
    return sum / PlayerList.size();

  }
  int get_physical_ability() const {
    if (PlayerList.empty()) {
      return 0;
    }
    int sum = 0;
    for (auto& p : PlayerList) {
      sum += p.get_physical_ability();
    }
    return sum / PlayerList.size();
  }
};

#endif //TEAM_HPP
