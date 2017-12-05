/*
  This is the .h file for the room.cpp, it includes the item struct, the function prototypes, and variables
  Author: Dieter Voegels
  Date: 12/01
*/
#include <iostream>
#include <map>

using namespace std;

struct item{
  char* name;
  char* description;
  int id;
};

class room{
 public:
  room(char* newName, char* description);
  void addItem(char* itemName, item* item);
  void deleteItem(char* itemName);
  item* getItem(char* itemName);
  void getItems();
  void addExit(char* direction, char* room);
  char* getExit(char* direction);
  void getExits();
  char* getName();
  char* getDescription();
  bool checkVisited();
  void setVisited();
  ~room();
 private:
  map<char*, item*> items;
  char* description;
  char* name;
  map<char*, char*> exits;
  bool visited;
};

