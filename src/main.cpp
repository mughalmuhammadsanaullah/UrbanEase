#include "StartupMenu.h"
#include "KMeans.h"
#include "Report.h"
#include <iostream>

using namespace std;

void demonstrateOOP(Store* stores, int count)
{
    cout << "\n========== OOP DEMONSTRATION ==========\n\n";

    cout << "-- operator<< (first store) --\n";
    cout << stores[0];

    cout << "-- operator== --\n";
    cout << "stores[0] == stores[0]: " << (stores[0] == stores[0]) << "\n";
    cout << "stores[0] == stores[1]: " << (stores[0] == stores[1]) << "\n\n";

    cout << "-- Analytics operator[] --\n";
    cout << "Month 0 sales: " << stores[0].getAnalytics()[0] << "\n";
    cout << "Month 1 sales: " << stores[0].getAnalytics()[1] << "\n\n";

    cout << "-- Analytics operator+ and operator> --\n";
    Analytics combined = stores[0].getAnalytics() + stores[1].getAnalytics();
    cout << "Combined total sales: " << combined.totalSales() << "\n";
    cout << "Store 0 > Store 1: "
        << (stores[0].getAnalytics() > stores[1].getAnalytics()) << "\n\n";

    cout << "-- Forecast operator++ --\n";
    Forecast f = stores[0].getForecast();
    cout << "Before: month " << f.getCurrentMonth() << "\n";
    Forecast old = f++;
    cout << "After f++: f is month " << f.getCurrentMonth()
        << ", old copy is month " << old.getCurrentMonth() << "\n";
    ++f;
    cout << "After ++f: month " << f.getCurrentMonth() << "\n\n";

    cout << "-- Forecast operator+ (cluster forecast) --\n";
    Forecast clusterForecast;
    for (int i = 0; i < 3; i++)
        clusterForecast = clusterForecast + stores[i].getForecast();
    cout << clusterForecast << "\n\n";

  
    cout << "-- Polymorphism (Person* pointing to Manager) --\n";
    Person* p = new Manager("Demo Manager", 40, 999, 120000, "Operations", 20000);
    p->display();
    cout << "\n\n";
    delete p;

    cout << "-- Employee pre/post increment --\n";
    Employee e("Test Emp", 28, 777, 50000);
    cout << "Original: "; e.display(); cout << "\n";
    ++e;
    cout << "After ++e: "; e.display(); cout << "\n";
    Employee snap = e++;
    cout << "snap from e++: "; snap.display(); cout << "\n";
    cout << "e after e++: ";   e.display();    cout << "\n";
}

void runTask2(Store* stores, int count)
{
    cout << "\n========================================\n";
    cout << "  TASK 2 - GEOGRAPHIC CLUSTERING        \n";
    cout << "========================================\n";

    KMeans kmeans(stores, count, 6);
    kmeans.run();
    kmeans.printClusterReport();

    if (kmeans.getK() >= 2)
    {
        cout << "\n-- Demonstrating Cluster operator+ (merge two clusters) --\n";
        Cluster mergedCluster = kmeans.getCluster(0) + kmeans.getCluster(1);
        cout << mergedCluster;
    }
}

void runTask3(Store* stores, int count)
{
    cout << "\n========================================\n";
    cout << "  TASK 3 - PERFORMANCE RANKING          \n";
    cout << "========================================\n";

 
    Report report(stores, count);

    cout << report;

    cout << "\n-- Demonstrating Analytics operator< and operator> --\n";

    Store* top = report.getTopStore();
    Store* bottom = report.getBottomStore();

    if (top && bottom)
    {
        cout << "  Top store    : " << top->getStoreID()
            << "  (annual sales: "
            << top->getAnalytics().annualSales() << ")\n";
        cout << "  Bottom store : " << bottom->getStoreID()
            << "  (annual sales: "
            << bottom->getAnalytics().annualSales() << ")\n";
        cout << "  top > bottom : "
            << (top->getAnalytics() > bottom->getAnalytics()) << "\n";
        cout << "  bottom < top : "
            << (bottom->getAnalytics() < top->getAnalytics()) << "\n";
    }
}

void runTask4(Store* stores, int count)
{
    cout << "\n========================================\n";
    cout << "  TASK 4 - PREDICTIVE FORECASTING      \n";
    cout << "========================================\n";

  
    for (int i = 0; i < count; i++)
        stores[i].computeForecast();

    cout << "\n--- Individual Store Forecasts (first 3 stores) ---\n";

    int show = (count < 3) ? count : 3;
    for (int i = 0; i < show; i++)
    {
        cout << "\nStore: " << stores[i].getStoreID() << " (" << stores[i].getCity() << ")\n";
        cout << stores[i].getForecast();   
    }

    cout << "\n--- 3-Month Rolling Forecast for Store 0 ---\n";
    cout << "(Using post-increment: forecast++ each step)\n\n";

    Forecast rolling = stores[0].getForecast();  

    for (int step = 0; step < 3; step++)
    {
        Forecast snapshot = rolling++;   
        cout << "Step " << (step + 1) << ":\n";
        cout << snapshot;                
    }

    cout << "\n--- Cluster-Wide Aggregate Forecast (all stores) ---\n";

    Forecast clusterForecast;    

    for (int i = 0; i < count; i++)
        clusterForecast = clusterForecast + stores[i].getForecast();

    cout << clusterForecast;

    cout << "\n--- Stores with Decline Warning ---\n";


    int warnCount = 0;
    for (int i = 0; i < count; i++)
    {
        if (stores[i].getForecast().getWarningFlag())
        {
            cout << "  WARNING: " << stores[i].getStoreID()
                << " (" << stores[i].getCity() << ")"
                << "  predicted: " << stores[i].getForecast().getPredictedSales()
                << "  confidence: " << stores[i].getForecast().getConfidenceLevel() << "%\n";
            warnCount++;
        }
    }

    if (warnCount == 0)
        cout << "  No stores flagged for decline.\n";
    else
        cout << "\n  Total stores flagged: " << warnCount << " / " << count << "\n";
}

int main()
{
   
    int count = 0;
    Store* stores = runStartupMenu(count);

    if (!stores || count == 0)
    {
        cout << "No stores loaded. Exiting.\n";
        return 1;
    }

    cout << "\nWorking with " << count << " stores.\n";

    // TASK 1 
    demonstrateOOP(stores, count);

    // TASK 2 
    runTask2(stores, count);

    // TASK 3 
    runTask3(stores, count);

    // TASK 4 
    runTask4(stores, count);

    delete[] stores;
    stores = nullptr;

    cout << "\nProgram complete.\n";
    return 0;
}
