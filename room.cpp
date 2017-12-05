/*
  This is the cpp file that adds each rooms name, description, exits, and items, allows them to be deleted, and prints out all items and exits. Also, it checks to see if the player has entered the room before.
  Author: Dieter Voegels
  Date: 12/01
*/
#include <iostream>
#include <map>
#include <cstring>
#include "room.h"

using namespace std;

//constructor
room::room(char* newName, char* newDescription){
  name = newName;
  description = newDescription;
  visited = false;
}

//destructor
room::~room(){
  delete name;
  delete description;
}
//add and item to the room
void room::addItem(char* itemName, item* item){
  items[itemName] = item;
}
//delete and item from the room
void room::deleteItem(char* itemName){
  for(map<char*, item*>::iterator it = items.begin(); it != items.end(); it++){
    if(strcmp(itemName, it->first) == 0){
      items.erase(it);
      break;
    }
  }
}
//get an item from the room
item* room::getItem(char* itemName){
  for(map<char*, item*>::iterator it = items.begin(); it != items.end(); it++){
    if(strcmp(itemName, it->first) == 0){
      return items[it->first];
    }
  }
  if(items.find(itemName) == items.end()){
    cout << "You could not find that item in the room." << endl;
    item* itemPointer = NULL;
    return itemPointer;
  }
}
//get all of the items in the room
void room::getItems(){
  for(map<char*, item*>::iterator it = items.begin(); it != items.end(); it++){
    cout << it->first << endl;
  }
  if(items.empty() == true){
    cout << "There are no items in this room." << endl;
  }
}
//add an exit to the room
void room::addExit(char* direction, char* room){
  exits[direction] = room;
}
//get an exit
char* room::getExit(char* direction){
  for(map<char*, char*>::iterator it = exits.begin(); it != exits.end(); it++){
    if(strcmp(direction, it->first) == 0){
      cout << "*door opens and closes*" << endl;
      return exits[it->first];
    }
  }
  if(exits.find(direction) == exits.end()){
    cout << "You got confused and hit a wall..." << endl;
    return "NULL";
  }
}
//get all of the exits
void room::getExits(){
  for(map<char*, char*>::iterator it = exits.begin(); it != exits.end(); it++){
    cout << it->first << endl;
  }
}
//get the name
char* room::getName(){
  return name;
}
//get the description
char* room::getDescription(){
  return description;
}
//check if the room has been visited
bool room::checkVisited(){
  return visited;
}
//set the visited state of the room
void room::setVisited(){
  visited = true;
}
