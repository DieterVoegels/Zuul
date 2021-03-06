/*
  This is a clone of Zuul, the goal of the game is to collect items to be able to escape.
  Author: Dieter Voegels
  Date: 12/1
*/
//include needed libraries and classes
#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include "room.h"

using namespace std;

//point struct to store (x,y) position and its character
struct point{
  int x;
  int y;
  char* character;
};

//function prototypes
void createItems(item* (&items)[5]);
void createRooms(map<char*, room*> &rooms, item* (&items)[5]);
void exit(map<char*, room*> &rooms, char* &currentRoom, map<char*, item*> &inventory);
void pickUp(map<char*, room*> &rooms, char* &currentRoom, item* (&items)[5], map<char*, item*> &inventory);
void drop(map<char*, room*> &rooms, char* &currentRoom, item* (&items)[5], map<char*, item*> &inventory);
void drawMap(map<char*, room*> rooms, char* currentRoom);

//constructor
int main(){
  //initialize variables
  item* items[5];
  map<char*, room*> rooms;
  map<char*, item*> inventory;
  //create items and rooms
  createItems(items);
  createRooms(rooms, items);

  char input[50];
  char* currentRoom = "cellRoom1";
  bool quit = false;

  //user instruction
  cout << "Type a command and press enter. This program is case sensitive." << endl;

  //main loop
  while(quit == false){
    //set variables and display information
    rooms[currentRoom]->setVisited();
    cout << endl;
    cout << "You are in a " << rooms[currentRoom]->getName() << ". " << rooms[currentRoom]->getDescription() << endl;
    //if you are in the final room, run the end sequence asking if you want to exit
    if(strcmp(currentRoom, "dungeon4Room4") == 0){
      cout << "You see light pouring out of the room, do you wish to exit? yes/no" << endl;
      cin.getline(input, 10);
      //if you want to exit, check the inventory
      if(strcmp(input, "yes") == 0){
	//if you have a sword, shield, and armor then you can fight off the monster and exit
	if(inventory.find("sword") != inventory.end() && inventory.find("shield") != inventory.end() && inventory.find("armor") != inventory.end()){
	  cout << "You escaped!" << endl;
	  quit = true;
	  continue;
	}
	//if you dont have those items then you die
	else{
	  cout << "You are ill-prepared and are killed by a botchling." << endl;
	  quit = true;
	  continue;
	}
      }	
    }
    //print out exits, items, and commands
    cout << "The exits are:" << endl;
    rooms[currentRoom]->getExits();
    cout << "The items in the room are:" << endl;
    rooms[currentRoom]->getItems();
    cout << "Type drop, pick up, inventory, exit, map, quit or help." << endl;
    cout << endl;
    cin.getline(input, 50);
    //if they type drop, run the drop function
    if(strcmp(input, "drop") == 0){
      drop(rooms, currentRoom, items, inventory);
    }
    //if they type pick up then run the pick up function
    if(strcmp(input, "pick up") == 0){
      pickUp(rooms, currentRoom, items, inventory);
    }
    //run the inventory function if they type inventory
    if(strcmp(input, "inventory") == 0){
      //if your inventory is empty, then print out you dont have items
      if(inventory.empty() == false){
	for(map<char*, item*>::iterator it = inventory.begin(); it != inventory.end(); it++){
	  cout << it->first << endl;
	}
      }
      else{
	cout << "You do not have any items." << endl;
      }
    }
    //if they type exit, run the exit command
    if(strcmp(input, "exit") == 0){
      exit(rooms, currentRoom, inventory);
    }
    //print out the map if they type map
    if(strcmp(input, "map") == 0){
      drawMap(rooms, currentRoom);
    }
    //print commands if they type help
    if(strcmp(input, "help") == 0){
      cout << "type 'drop' to drop an item." << endl;
      cout << "type 'pick up' to pick up an item." << endl;
      cout << "type 'inventory' to view your inventory." << endl;
      cout << "type 'quit' to quit the program." << endl;
    }
    //if they type quit, exit the loop which ends the game
    if(strcmp(input, "quit") == 0){
      cout << "Thank you for playing" << endl;
      quit = true;
    }
  }
}

//exit function, takes three maps
void exit(map<char*, room*> &rooms, char* &currentRoom, map<char*, item*> &inventory){
  cout << "which exit do you wish to go through?" << endl;
  //get which exit they want to go through
  char input[10];
  cin >> input;
  cin.ignore();
  //check if the exit is in cellRoom2 and is north
  if(strcmp(currentRoom, "cellRoom2") == 0 && strcmp(input, "north") == 0){
    //if they dont have the key to exit the cell, tell the player that they need a key
    if(inventory.find("key") == inventory.end()){
      cout << "This door seems to be locked. There might be a key somewhere." << endl;
    }
    //if they have the key, let them go through
    else{
      cout << "You unlocked the door." << endl;
      //change current room to the next room
      char* newRoom = rooms[currentRoom]->getExit(input);
      if(strcmp(newRoom, "NULL") != 0){
	currentRoom = newRoom;
      }
    }
  }
  //if the exit exists, then change the current room to the next room
  else{
    char* newRoom = rooms[currentRoom]->getExit(input);
    if(strcmp(newRoom, "NULL") != 0){
      currentRoom = newRoom;
    }
  }
}

//create all of the items in the game
void createItems(item* (&items)[5]){
  //create each item, initializing the id, description, and name
  item* armor = new item;
  armor->id = 0;
  armor->description = "A set of armor.";
  armor->name = "armor";

  item* shield = new item;
  shield->id = 1;
  shield ->description = "A wooden shield.";
  shield->name = "shield";

  item* sword = new item;
  sword->id = 2;
  sword->description = "A silver sword.";
  sword->name = "sword";

  item* key = new item;
  key->id = 3;
  key->description = "A key.";
  key->name = "key";

  item* book = new item;
  book->id = 4;
  book->description = "Your favorite book.";
  book->name = "book";

  //add to the array
  items[armor->id] = armor;
  items[shield->id] = shield;
  items[sword->id] = sword;
  items[key->id] = key;
  items[book->id] = book;
}

//pick up function
void pickUp(map<char*, room*> &rooms, char* &currentRoom, item* (&items)[5], map<char*, item*> &inventory){
  cout << "What item do you want to pick up?" << endl;
  //get what item they want to pick up
  char input[10];
  cin >> input;
  //search the room for the item, if the item does not exist tell the player
  item* item = rooms[currentRoom]->getItem(input);
  if(item){
    inventory[item->name] = item;
    rooms[currentRoom]->deleteItem(item->name);
  }
  cin.ignore();
}

//drop function
void drop(map<char*, room*> &rooms, char* &currentRoom, item* (&items)[5], map<char*, item*> &inventory){
  cout << "What item do you want to drop?" << endl;
  //get what item they want to drop
  char input[10];
  cin >> input;
  //look through the inventory for that item, if they have it then drop ip
  for(map<char*, item*>::iterator it = inventory.begin(); it != inventory.end(); it++){
    //if it is in the inventory, then delete it from the inventory and add it to the room
    if(strcmp(input,it->first) == 0){
      inventory.erase(it->second->name);
      rooms[currentRoom]->addItem(it->second->name, it->second);
    }
  } 
  cin.ignore();
}

//create all of the rooms in the level
void createRooms(map<char*,room*> &rooms, item* (&items)[5]){
  //create all of the rooms
  room* cellRoom1 = new room("Cell", "This is your cell. You see your book in the corner on the floor.");
  room* cellRoom2 = new room("Cell", "This is your cell.");
  room* cellRoom3 = new room("Cell", "This is your cell. You see something shiny on the floor.");
  room* dungeon1Room1 = new room("Dungeon", "This is a room of the dungeon.");
  room* dungeon1Room2 = new room("Dungeon", "This is a room of the dungeon");
  room* dungeon1Room3 = new room("Dungeon", "This is a room of the dungeon");
  room* dungeon2Room1 = new room("Dungeon", "This is a room of the dungeon.");
  room* dungeon2Room2 = new room("Dungeon", "This room seems to have nothing in it, until you see the half eaten remains of a human.");
  room* dungeon2Room3 = new room("Dungeon", "This is a room of the dungeon.");
  room* dungeon3Room1 = new room("Dungeon", "This is a room of the dungeon.");
  room* dungeon3Room2 = new room("Dungeon", "This is a room of the dungeon.");
  room* dungeon4Room1 = new room("Dungeon", "You scream in terror as bats fly into your face in attempt to flood out of the door you opened.");
  room* dungeon4Room2 = new room("Dungeon", "This is a room of the dungeon.");
  room* dungeon4Room3 = new room("Dungeon", "This is a room of the dungeon.");
  room* dungeon4Room4 = new room("Dungeon", "This is a room of the dungeon.");

  //create the exits for the rooms
  cellRoom1->addExit("north", "cellRoom2");

  cellRoom2->addExit("north", "dungeon1Room1");
  cellRoom2->addExit("west", "cellRoom3");
  cellRoom2->addExit("south", "cellRoom1");

  cellRoom3->addExit("east", "cellRoom2");

  dungeon1Room1->addExit("north", "dungeon2Room1");
  dungeon1Room1->addExit("south", "cellRoom2");
  dungeon1Room1->addExit("east", "dungeon1Room2");

  dungeon1Room2->addExit("west", "dungeon1Room1");
  dungeon1Room2->addExit("east", "dungeon1Room3");
  
  dungeon1Room3->addExit("north", "dungeon3Room1");
  dungeon1Room3->addExit("south", "dungeon4Room1");
  dungeon1Room3->addExit("west", "dungeon1Room2");

  dungeon2Room1->addExit("south", "dungeon1Room1");
  dungeon2Room1->addExit("west", "dungeon2Room2");

  dungeon2Room2->addExit("east", "dungeon2Room1");
  dungeon2Room2->addExit("west", "dungeon2Room3");

  dungeon2Room3->addExit("east", "dungeon2Room2");

  dungeon3Room1->addExit("north", "dungeon3Room2");
  dungeon3Room1->addExit("south", "dungeon1Room3");

  dungeon3Room2->addExit("south", "dungeon3Room1");
  
  dungeon4Room1->addExit("north", "dungeon1Room3");
  dungeon4Room1->addExit("east", "dungeon4Room3");
  dungeon4Room1->addExit("south", "dungeon4Room2");

  dungeon4Room2->addExit("north", "dungeon4Room1");
  
  dungeon4Room3->addExit("east", "dungeon4Room4");
  dungeon4Room3->addExit("west", "dungeon4Room1");
  
  dungeon4Room4->addExit("west", "dungeon4Room3");

  //add the items to the rooms
  cellRoom3->addItem(items[3]->name, items[3]);
  cellRoom1->addItem(items[4]->name, items[4]);
  dungeon2Room3->addItem(items[0]->name, items[0]);
  dungeon3Room2->addItem(items[1]->name, items[1]);
  dungeon4Room3->addItem(items[2]->name, items[2]);

  //add the room to the map
  rooms["cellRoom1"] = cellRoom1;
  rooms["cellRoom2"] = cellRoom2;
  rooms["cellRoom3"] = cellRoom3;
  
  rooms["dungeon1Room1"] = dungeon1Room1;
  rooms["dungeon1Room2"] = dungeon1Room2;
  rooms["dungeon1Room3"] = dungeon1Room3;
  
  rooms["dungeon2Room1"] = dungeon2Room1;
  rooms["dungeon2Room2"] = dungeon2Room2;
  rooms["dungeon2Room3"] = dungeon2Room3;
  
  rooms["dungeon3Room1"] = dungeon3Room1;
  rooms["dungeon3Room2"] = dungeon3Room2;
  
  rooms["dungeon4Room1"] = dungeon4Room1;
  rooms["dungeon4Room2"] = dungeon4Room2;
  rooms["dungeon4Room3"] = dungeon4Room3;
  rooms["dungeon4Room4"] = dungeon4Room4;
}

//draw the map
void drawMap(map<char*, room*> rooms, char* currentRoom){
  
  //create vector of point pointers and initialize all of the points
  vector<point*> levelMap;

  //create the pointers used for the player
  point* pointPointer00 = new point;
  point* pointPointer01 = new point;

  //print out two stars in the room that you are in
  if(strcmp(currentRoom, "cellRoom1") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 7;
    pointPointer00->y = 9;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "cellRoom2") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 7;
    pointPointer00->y = 7;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "cellRoom3") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 4;
    pointPointer00->y = 7;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon1Room1") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 7;
    pointPointer00->y = 5;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon1Room2") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 10;
    pointPointer00->y = 5;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon1Room3") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 13;
    pointPointer00->y = 5;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon2Room1") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 7;
    pointPointer00->y = 3;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon2Room2") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 4;
    pointPointer00->y = 3;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon2Room3") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 1;
    pointPointer00->y = 3;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon3Room1") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 13;
    pointPointer00->y = 3;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon3Room2") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 13;
    pointPointer00->y = 1;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon4Room1") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 13;
    pointPointer00->y = 7;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon4Room2") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 13;
    pointPointer00->y = 9;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon4Room3") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 16;
    pointPointer00->y = 7;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  if(strcmp(currentRoom, "dungeon4Room4") == 0){
    point* pointpointer00 = new point;
    pointPointer00->x = 19;
    pointPointer00->y =7 ;
    pointPointer00->character = "*";
    levelMap.push_back(pointPointer00);

    point* pointpointer01 = new point;
    pointPointer01->x = pointPointer00->x+1;
    pointPointer01->y = pointPointer00->y;
    pointPointer01->character = "*";
    levelMap.push_back(pointPointer01);
  }

  //if you have been in the room, add it to the vector to be printed
  
  /*
  -------------
    cellRoom1
  -------------
  */
    
  //(7,10)"-"
  point* pointPointer1 = new point;
  pointPointer1->x = 7;
  pointPointer1->y = 10;
  pointPointer1->character = "-";
  levelMap.push_back(pointPointer1);
  
  //(8,10)"-"
  point* pointPointer2 = new point;
  pointPointer2->x = 8;
  pointPointer2->y = 10;
  pointPointer2->character = "-";
  levelMap.push_back(pointPointer2);
  
  //(6,9)"|"
  point* pointPointer3 = new point;
  pointPointer3->x = 6;
  pointPointer3->y = 9;
  pointPointer3->character = "|";
  levelMap.push_back(pointPointer3);
  
  //(9,9)"|"
  point* pointPointer4 = new point;
  pointPointer4->x = 9;
  pointPointer4->y = 9;
  pointPointer4->character = "|";
  levelMap.push_back(pointPointer4);
  
  //(7,8)"-"
  point* pointPointer5 = new point;
  pointPointer5->x = 7;
  pointPointer5->y = 8;
  pointPointer5->character = "-";
  levelMap.push_back(pointPointer5);
  
  //(8,8)"-"
  point* pointPointer6 = new point;
  pointPointer6->x = 8;
  pointPointer6->y = 8;
  pointPointer6->character = "-";
  levelMap.push_back(pointPointer6);

  /*
  -------------
    cellRoom2
  -------------
  */

  if(rooms["cellRoom2"]->checkVisited() == true){
    //(6,7)"|"
    point* pointPointer7 = new point;
    pointPointer7->x = 6;
    pointPointer7->y = 7;
    pointPointer7->character = "|";
    levelMap.push_back(pointPointer7);
    
    //(9,7)"|"
    point* pointPointer8 = new point;
    pointPointer8->x = 9;
    pointPointer8->y = 7;
    pointPointer8->character = "|";
    levelMap.push_back(pointPointer8);
    
    //(8,6)"-"
    point* pointPointer9 = new point;
    pointPointer9->x = 8;
    pointPointer9->y = 6;
    pointPointer9->character = "-";
    levelMap.push_back(pointPointer9);
    
    //(7,6)"-"
    point* pointPointer10 = new point;
    pointPointer10->x = 7;
    pointPointer10->y = 6;
    pointPointer10->character = "-";
    levelMap.push_back(pointPointer10);
  }
  
  /*
  -------------
    cellRoom3
  -------------
  */

  if(rooms["cellRoom3"]->checkVisited() == true){
  
    //(4,8)"-"
    point* pointPointer11 = new point;
    pointPointer11->x = 4;
    pointPointer11->y = 8;
    pointPointer11->character = "-";
    levelMap.push_back(pointPointer11);
    
    //(5,8)"-"
    point* pointPointer12 = new point;
    pointPointer12->x = 5;
    pointPointer12->y = 8;
    pointPointer12->character = "-";
    levelMap.push_back(pointPointer12);
    
    //(4,6)"-"
    point* pointPointer13 = new point;
    pointPointer13->x = 4;
    pointPointer13->y = 6;
    pointPointer13->character = "-";
    levelMap.push_back(pointPointer13);
    
    //(5,6)"-"
    point* pointPointer135 = new point;
    pointPointer135->x = 5;
    pointPointer135->y = 6;
    pointPointer135->character = "-";
    levelMap.push_back(pointPointer135);
    
    //(3,7)"|"
    point* pointPointer14 = new point;
    pointPointer14->x = 3;
    pointPointer14->y = 7;
    pointPointer14->character = "|";
    levelMap.push_back(pointPointer14);
  }
    
  /*
  ----------------
    dungeon1Room1
  ----------------
  */

  if(rooms["dungeon1Room1"]->checkVisited() == true){
  
    //(6,5)"|"
    point* pointPointer15 = new point;
    pointPointer15->x = 6;
    pointPointer15->y = 5;
    pointPointer15->character = "|";
    levelMap.push_back(pointPointer15);
    
    //(7,4)"-"
    point* pointPointer16 = new point;
    pointPointer16->x = 7;
    pointPointer16->y = 4;
    pointPointer16->character = "-";
    levelMap.push_back(pointPointer16);
    
    //(8,4)"-"
    point* pointPointer17 = new point;
    pointPointer17->x = 8;
    pointPointer17->y = 4;
    pointPointer17->character = "-";
    levelMap.push_back(pointPointer17);
    
    //(9,5)"|"
    point* pointPointer18 = new point;
    pointPointer18->x = 9;
    pointPointer18->y = 5;
    pointPointer18->character = "|";
    levelMap.push_back(pointPointer18);

  }
    
  /*
  -----------------
    dungeon1Room2
  -----------------
  */

  if(rooms["dungeon1Room2"]->checkVisited() == true){
    
    //(10,4)"-"
    point* pointPointer19 = new point;
    pointPointer19->x = 10;
    pointPointer19->y = 4;
    pointPointer19->character = "-";
    levelMap.push_back(pointPointer19);
    
    //(11,4)"-"
    point* pointPointer20 = new point;
    pointPointer20->x = 11;
    pointPointer20->y = 4;
    pointPointer20->character = "-";
    levelMap.push_back(pointPointer20);
    
    //(10,6)"-"
    point* pointPointer21 = new point;
    pointPointer21->x = 10;
    pointPointer21->y = 6;
    pointPointer21->character = "-";
    levelMap.push_back(pointPointer21);
    
    //(11,6)"-"
    point* pointPointer22 = new point;
    pointPointer22->x = 11;
    pointPointer22->y = 6;
    pointPointer22->character = "-";
    levelMap.push_back(pointPointer22);
    
    //(12,5)"|"
    point* pointPointer23 = new point;
    pointPointer23->x = 12;
    pointPointer23->y = 5;
    pointPointer23->character = "|";
    levelMap.push_back(pointPointer23);
  }
    
  /*
  -----------------
    dungeon1Room3
  -----------------
  */

  if(rooms["dungeon1Room3"]->checkVisited() == true){
  
    //(13,4)"-"
    point* pointPointer24 = new point;
    pointPointer24->x = 13;
    pointPointer24->y = 4;
    pointPointer24->character = "-";
    levelMap.push_back(pointPointer24);
    
    //(14,4)"-"
    point* pointPointer25 = new point;
    pointPointer25->x = 14;
    pointPointer25->y = 4;
    pointPointer25->character = "-";
    levelMap.push_back(pointPointer25);
    
    //(13,6)"-"
    point* pointPointer26 = new point;
    pointPointer26->x = 13;
    pointPointer26->y = 6;
    pointPointer26->character = "-";
    levelMap.push_back(pointPointer26);
    
    //(14,6)"-"
    point* pointPointer27 = new point;
    pointPointer27->x = 14;
    pointPointer27->y = 6;
    pointPointer27->character = "-";
    levelMap.push_back(pointPointer27);
    
    //(15,5)"|"
    point* pointPointer28 = new point;
    pointPointer28->x = 15;
    pointPointer28->y = 5;
    pointPointer28->character = "|";
    levelMap.push_back(pointPointer28);
  }
    
  /*
  -----------------
    dungeon2Room1
  -----------------
  */

  if(rooms["dungeon2Room1"]->checkVisited() == true){
    
    //(6,3)"|"
    point* pointPointer29 = new point;
    pointPointer29->x = 6;
    pointPointer29->y = 3;
    pointPointer29->character = "|";
    levelMap.push_back(pointPointer29);
    
    //(9,3)"|"
    point* pointPointer30 = new point;
    pointPointer30->x = 9;
    pointPointer30->y = 3;
    pointPointer30->character = "|";
    levelMap.push_back(pointPointer30);
    
    //(7,2)"-"
    point* pointPointer31 = new point;
    pointPointer31->x = 7;
    pointPointer31->y = 2;
    pointPointer31->character = "-";
    levelMap.push_back(pointPointer31);
    
    //(8,2)"-"
    point* pointPointer32 = new point;
    pointPointer32->x = 8;
    pointPointer32->y = 2;
    pointPointer32->character = "-";
    levelMap.push_back(pointPointer32);
  }
    
  /*
  ----------------
    dugeon2Room2
  ----------------
  */

  if(rooms["dungeon2Room2"]->checkVisited() == true){
    
    //(3,3)"|"
    point* pointPointer325 = new point;
    pointPointer325->x = 3;
    pointPointer325->y = 3;
    pointPointer325->character = "|";
    levelMap.push_back(pointPointer325);
    
    //(4,2)"-"
    point* pointPointer33 = new point;
    pointPointer33->x = 4;
    pointPointer33->y = 2;
    pointPointer33->character = "-";
    levelMap.push_back(pointPointer33);
    
    //(5,2)"-"
    point* pointPointer34 = new point;
    pointPointer34->x = 5;
    pointPointer34->y = 2;
    pointPointer34->character = "-";
    levelMap.push_back(pointPointer34);
    
    //(4,4)"-"
    point* pointPointer35 = new point;
    pointPointer35->x = 4;
    pointPointer35->y = 4;
    pointPointer35->character = "-";
    levelMap.push_back(pointPointer35);
    
    //(5,4)"-"
    point* pointPointer36 = new point;
    pointPointer36->x = 5;
    pointPointer36->y = 4;
    pointPointer36->character = "-";
    levelMap.push_back(pointPointer36);
  }
    
  /*
  -----------------
    dungeon2Room3
  -----------------
  */

  if(rooms["dungeon2Room3"]->checkVisited() == true){
  
    //(0,3)"|"
    point* pointPointer37 = new point;
    pointPointer37->x = 0;
    pointPointer37->y = 3;
    pointPointer37->character = "|";
    levelMap.push_back(pointPointer37);
    
    //(1,2)"-"
    point* pointPointer38 = new point;
    pointPointer38->x = 1;
    pointPointer38->y = 2;
    pointPointer38->character = "-";
    levelMap.push_back(pointPointer38);
    
    //(2,2)"-"
    point* pointPointer39 = new point;
    pointPointer39->x = 2;
    pointPointer39->y = 2;
    pointPointer39->character = "-";
    levelMap.push_back(pointPointer39);
    
    //(1,4)"-"
    point* pointPointer40 = new point;
    pointPointer40->x = 1;
    pointPointer40->y = 4;
    pointPointer40->character = "-";
    levelMap.push_back(pointPointer40);
    
    //(2,4)"-"
    point* pointPointer41 = new point;
    pointPointer41->x = 2;
    pointPointer41->y = 4;
    pointPointer41->character = "-";
    levelMap.push_back(pointPointer41);
  }
    
  /*
  ----------------
    dungeon3Room1
  -----------------
  */

  if(rooms["dungeon3Room1"]->checkVisited() == true){ 
    
    //(12,3)"|"
    point* pointPointer42 = new point;
    pointPointer42->x = 12;
    pointPointer42->y = 3;
    pointPointer42->character = "|";
    levelMap.push_back(pointPointer42);
    
    //(15,3)"|"
    point* pointPointer43 = new point;
    pointPointer43->x = 15;
    pointPointer43->y = 3;
    pointPointer43->character = "|";
    levelMap.push_back(pointPointer43);
    
    //(13,2)"-"
    point* pointPointer44 = new point;
    pointPointer44->x = 13;
    pointPointer44->y = 2;
    pointPointer44->character = "-";
    levelMap.push_back(pointPointer44);
    
    //(14,2)"-"
    point* pointPointer45 = new point;
    pointPointer45->x = 14;
    pointPointer45->y = 2;
    pointPointer45->character = "-";
    levelMap.push_back(pointPointer45);
  }
    
  /*
  -----------------
    dungeon3Room2
  -----------------
  */

  if(rooms["dungeon3Room2"]->checkVisited() == true){
  
    //(12,1)"|"
    point* pointPointer46 = new point;
    pointPointer46->x = 12;
    pointPointer46->y = 1;
    pointPointer46->character = "|";
    levelMap.push_back(pointPointer46);
    
    //(15,1)"|"
    point* pointPointer47 = new point;
    pointPointer47->x = 15;
    pointPointer47->y = 1;
    pointPointer47->character = "|";
    levelMap.push_back(pointPointer47);
    
    //(13,0)"-"
    point* pointPointer48 = new point;
    pointPointer48->x = 13;
    pointPointer48->y = 0;
    pointPointer48->character = "-";
    levelMap.push_back(pointPointer48);
    
    //(14,0)"-"
    point* pointPointer49 = new point;
    pointPointer49->x = 14;
    pointPointer49->y = 0;
    pointPointer49->character = "-";
    levelMap.push_back(pointPointer49);
  }
  
  /*
  ----------------
    dungon4Room1
  ----------------
  */

  if(rooms["dungeon4Room1"]->checkVisited() == true){
  
    //(12,7)"|"
    point* pointPointer50 = new point;
    pointPointer50->x = 12;
    pointPointer50->y = 7;
    pointPointer50->character = "|";
    levelMap.push_back(pointPointer50);
    
    //(15,7)"|"
    point* pointPointer51 = new point;
    pointPointer51->x = 15;
    pointPointer51->y = 7;
    pointPointer51->character = "|";
    levelMap.push_back(pointPointer51);
    
    //(13,8)"-"
    point* pointPointer52 = new point;
    pointPointer52->x = 13;
    pointPointer52->y = 8;
    pointPointer52->character = "-";
    levelMap.push_back(pointPointer52);
    
    //(14,8)"-"
    point* pointPointer53 = new point;
    pointPointer53->x = 14;
    pointPointer53->y = 8;
    pointPointer53->character = "-";
    levelMap.push_back(pointPointer53);
  }
    
  /*
  -----------------
    dungeon4Room2
  -----------------
  */

  if(rooms["dungeon4Room2"]->checkVisited() == true){
  
    //(12,9)"|"
    point* pointPointer54 = new point;
    pointPointer54->x = 12;
    pointPointer54->y = 9;
    pointPointer54->character = "|";
    levelMap.push_back(pointPointer54);
    
    //(15,9)"|"
    point* pointPointer55 = new point;
    pointPointer55->x = 15;
    pointPointer55->y = 9;
    pointPointer55->character = "|";
    levelMap.push_back(pointPointer55);
    
    //(13,10)"-"
    point* pointPointer56 = new point;
    pointPointer56->x = 13;
    pointPointer56->y = 10;
    pointPointer56->character = "-";
    levelMap.push_back(pointPointer56);
    
    //(14,10)"-"
    point* pointPointer57 = new point;
    pointPointer57->x = 14;
    pointPointer57->y = 10;
    pointPointer57->character = "-";
    levelMap.push_back(pointPointer57);
  }
    
  /*
  -----------------
    dungeon4Room3
  -----------------
  */

  if(rooms["dungeon4Room3"]->checkVisited() == true){
  
    //(16,6)"-"
    point* pointPointer58 = new point;
    pointPointer58->x = 16;
    pointPointer58->y = 6;
    pointPointer58->character = "-";
    levelMap.push_back(pointPointer58);
    
    //(17,6)"-"
    point* pointPointer59 = new point;
    pointPointer59->x = 17;
    pointPointer59->y = 6;
    pointPointer59->character = "-";
    levelMap.push_back(pointPointer59);
    
    //(16,8)"-"
    point* pointPointer60 = new point;
    pointPointer60->x = 16;
    pointPointer60->y = 8;
    pointPointer60->character = "-";
    levelMap.push_back(pointPointer60);
    
    //(17,8)"-"
    point* pointPointer61 = new point;
    pointPointer61->x = 17;
    pointPointer61->y = 8;
    pointPointer61->character = "-";
    levelMap.push_back(pointPointer61);
    
    //(18,7)"|"
    point* pointPointer62 = new point;
    pointPointer62->x = 18;
    pointPointer62->y = 7;
    pointPointer62->character = "|";
    levelMap.push_back(pointPointer62);
  }
    
  /*
  -----------------
    dungeon4Room4
  -----------------
  */

  if(rooms["dungeon4Room4"]->checkVisited() == true){
  
    //(19,6)"-"
    point* pointPointer63 = new point;
    pointPointer63->x = 19;
    pointPointer63->y = 6;
    pointPointer63->character = "-";
    levelMap.push_back(pointPointer63);
    
    //(20,6)"-"
    point* pointPointer64 = new point;
    pointPointer64->x = 20;
    pointPointer64->y = 6;
    pointPointer64->character = "-";
    levelMap.push_back(pointPointer64);

    //(19,8)"-"
    point* pointPointer65 = new point;
    pointPointer65->x = 19;
    pointPointer65->y = 8;
    pointPointer65->character = "-";
    levelMap.push_back(pointPointer65);
    
    //(20,8)"-"
    point* pointPointer66 = new point;
    pointPointer66->x = 20;
    pointPointer66->y = 8;
    pointPointer66->character = "-";
    levelMap.push_back(pointPointer66);
    
    //(21,7)"|"
    point* pointPointer67 = new point;
    pointPointer67->x = 21;
    pointPointer67->y = 7;
    pointPointer67->character = "|";
    levelMap.push_back(pointPointer67);
  }

  //initialize the variable
  bool ifPoint = false;

  //check each (x,y) point to see if there is something there, either -,|, or *, then print it out. Else, print out a space
  for(int y = 0; y <= 10; y++){
    for(int x = 0; x <= 21; x++){
      for(vector<point*>::iterator it = levelMap.begin(); it != levelMap.end(); it++){
	if((*it)->y == y && (*it)->x == x){
	  cout << (*it)->character;
	  ifPoint = true;
	  delete (*it);
	}
      }
      if(ifPoint == false){
	cout << " ";
      }
      else{
	ifPoint = false;
      }
    }
    cout << endl;
  }
}
