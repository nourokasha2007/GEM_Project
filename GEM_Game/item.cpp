
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Item {
protected:
    string name;
    bool collected;
    string type;

public:
    Item(){
        name = "";
        type = "";
        collected = false;
    }
    Item(string n, bool c, string t) {
        name = n;
        type=t;
        collected = c;
    }

    void collect() {
        collected = true;
    }

    string getName() {
        return name;
    }

    bool isCollected() {
        return collected;
    }
    string getType() {
        return type;
        }

};





 

    
    
    
    
    class Tool : public Item {
    private:
        int uses=0;
    public:
    
        Tool() : Item() {
            uses=0;
        }
        
        
        Tool(string n, bool c, int u): Item(n,c, "Tool") {
            uses = u;
        }
        
        void use() {
            if (uses > 0) {
                uses--;
                cout << name << " used. Remaining uses: " << uses << endl;
            }
            else {
                cout << name << " has no uses left." << endl;
            }
        }
        
        int getUses() {
            return uses;
        }
    };




class Inventory {
private:
    vector<Item*> items;

public:
    void addItem(Item* item) {
        items.push_back(item);
    }

    void showItems() {
        for (int i = 0; i < items.size(); i++) {
            cout << items[i]->getName() << endl;
        }
    }

    // count specific item (THIS replaces map)
    int countItem(string name) {
        int count = 0;

        for (int i = 0; i < items.size(); i++) {
            if (items[i]->getName() == name) {
                count++;
            }
        }

        return count;
    }
};

