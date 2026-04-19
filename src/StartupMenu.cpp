#include "StartupMenu.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

Store* runStartupMenu(int& outCount) 
{

    srand(time(0));  

    cout << "\n==========================================\n";
    cout << "       UrbanEase Analytics System               \n";
    cout << "==========================================\n";
    cout << "  1. Load existing data from file\n";
    cout << "  2. Generate new dataset\n";
    cout << "==========================================\n";
    cout << "Enter choice (1 or 2): ";

    int choice;
    cin >> choice;

    if (choice == 1) 
    {
        ifstream test(DATA_FILE);

        if (!test.is_open()) 
        {
            cout << "File \"" << DATA_FILE << "\" not found. " << "Falling back to generation.\n";
            choice = 2;   // fall through to generation below
        }
        else 
        {
            test.close();

            Store* stores = loadAllStores(DATA_FILE, outCount);

            if (stores && outCount > 0) 
            {
                cout << "Loaded " << outCount << " stores from \"" << DATA_FILE << "\".\n";
                return stores;
            }

            cout << "Load failed. Falling back to generation.\n";
            choice = 2;
        }
    }

    
    int n = 0;
    do 
    {
        cout << "Enter number of stores to generate (min " << MIN_STORES << "): ";
        cin >> n;

        if (n < MIN_STORES)
            cout << "ERROR: Must be at least " << MIN_STORES << ". Try again.\n";
    } while (n < MIN_STORES);

    cout << "Generating " << n << " stores...\n";

    Store* stores = new Store[n];

    for (int i = 0; i < n; i++) 
    {
        stores[i] = generateStore(i + 1);
        if ((i + 1) % 100 == 0)
            cout << "  " << (i + 1) << " / " << n << " done...\n";
    }

    outCount = n;

    saveAllStores(stores, n, DATA_FILE);

    return stores;
}