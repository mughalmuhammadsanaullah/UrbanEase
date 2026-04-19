#include "KMeans.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>   

#define INT_MAX 2147483647;
#define DOUBLE_MAX 1.79769e+308;

using namespace std;

KMeans::KMeans(Store* storesArr, int count, int numClusters) : stores(storesArr), storeCount(count), k(numClusters),
    clusters(nullptr),
    centroids(nullptr),
    assignments(nullptr),
    subClusters(nullptr),
    subClusterTiers(3),
    salesMin(0.0), salesMax(1.0),
    latMin(0.0), latMax(1.0),
    lonMin(0.0), lonMax(1.0),
    maxIterations(100)
{
    clusters = new Cluster[k];
    centroids = new Centroid[k];
    assignments = new int[storeCount];

    for (int i = 0; i < storeCount; i++)
        assignments[i] = -1;

    subClusters = new Cluster * [k];
    for (int i = 0; i < k; i++)
        subClusters[i] = new Cluster[subClusterTiers];

}

KMeans::~KMeans()
{
    delete[] clusters;
    clusters = nullptr;

    delete[] centroids;
    centroids = nullptr;

    delete[] assignments;
    assignments = nullptr;

    if (subClusters != nullptr)
    {
        for (int i = 0; i < k; i++)
        {
            delete[] subClusters[i];
            subClusters[i] = nullptr;
        }
        delete[] subClusters;
        subClusters = nullptr;
    }
}


void KMeans::computeScalingBounds()
{
    if (storeCount == 0) return;

    // Initialise bounds to first store's values
    salesMin = salesMax = stores[0].getAnalytics().totalSales();
    latMin = latMax = stores[0].getLocation().getLatitude();
    lonMin = lonMax = stores[0].getLocation().getLongitude();

    for (int i = 1; i < storeCount; i++)
    {
        double s = stores[i].getAnalytics().totalSales();
        double la = stores[i].getLocation().getLatitude();
        double lo = stores[i].getLocation().getLongitude();

        if (s < salesMin) salesMin = s;
        if (s > salesMax) salesMax = s;
        if (la < latMin)   latMin = la;
        if (la > latMax)   latMax = la;
        if (lo < lonMin)   lonMin = lo;
        if (lo > lonMax)   lonMax = lo;
    }

    // Guard against division by zero if all stores have same value
    if (salesMax == salesMin) salesMax = salesMin + 1.0;
    if (latMax == latMin)   latMax = latMin + 1.0;
    if (lonMax == lonMin)   lonMax = lonMin + 1.0;
}

double KMeans::normaliseSales(double sales) const
{
    return (sales - salesMin) / (salesMax - salesMin);
}

double KMeans::normaliseLat(double lat) const
{
    return (lat - latMin) / (latMax - latMin);
}

double KMeans::normaliseLon(double lon) const
{
    return (lon - lonMin) / (lonMax - lonMin);
}

Centroid KMeans::storeAsCentroid(int i) const
{
    return Centroid(
        normaliseLat(stores[i].getLocation().getLatitude()),
        normaliseLon(stores[i].getLocation().getLongitude()),
        normaliseSales(stores[i].getAnalytics().totalSales())
    );
}

void KMeans::seedCentroids()
{
    int* usedIndices = new int[k];
    int  usedCount = 0;

    for (int c = 0; c < k; c++)
    {
        int chosen = -1;

        do 
        {
            chosen = rand() % storeCount;

            bool alreadyUsed = false;
            for (int j = 0; j < usedCount; j++)
            {
                if (usedIndices[j] == chosen)
                {
                    alreadyUsed = true;
                    break;
                }
            }

            if (!alreadyUsed) break;

        } while (true);

        usedIndices[usedCount++] = chosen;
        centroids[c] = storeAsCentroid(chosen);

        // Name the cluster after the city of the seed store
        clusters[c].setClusterName(stores[chosen].getCity() + " Cluster " + to_string(c + 1));
    }

    delete[] usedIndices;
}


bool KMeans::assignStores()
{
    bool changed = false;

    for (int i = 0; i < storeCount; i++)
    {
        Centroid storePt = storeAsCentroid(i);

        double bestDist = DOUBLE_MAX;
        int bestCluster = 0;

        // Compare distance to each of the k centroids
        for (int c = 0; c < k; c++)
        {
            double d = storePt.distanceTo(centroids[c]);
            if (d < bestDist)
            {
                bestDist = d;
                bestCluster = c;
            }
        }

        // Check if this store changed its cluster
        if (assignments[i] != bestCluster)
        {
            assignments[i] = bestCluster;
            changed = true;
        }
    }

    return changed;
}

void KMeans::updateCentroids()
{
    // Accumulators for computing the mean
    double* sumLat = new double[k];
    double* sumLon = new double[k];
    double* sumSales = new double[k];
    int* cnt = new int[k];

    for (int c = 0; c < k; c++)
    {
        sumLat[c] = sumLon[c] = sumSales[c] = 0.0;
        cnt[c] = 0;
    }

    // Sum up all assigned stores' normalised features
    for (int i = 0; i < storeCount; i++)
    {
        int c = assignments[i];
        if (c < 0 || c >= k) continue;

        sumLat[c] += normaliseLat(stores[i].getLocation().getLatitude());
        sumLon[c] += normaliseLon(stores[i].getLocation().getLongitude());
        sumSales[c] += normaliseSales(stores[i].getAnalytics().totalSales());
        cnt[c]++;
    }

    // Compute new centroid = mean of assigned stores
    for (int c = 0; c < k; c++)
    {
        if (cnt[c] > 0)
        {
            centroids[c].setFeature(0, sumLat[c] / cnt[c]);
            centroids[c].setFeature(1, sumLon[c] / cnt[c]);
            centroids[c].setFeature(2, sumSales[c] / cnt[c]);
        }
        
    }

    delete[] sumLat;
    delete[] sumLon;
    delete[] sumSales;
    delete[] cnt;
}

void KMeans::sortByTotalSales(Store** arr, int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        int minIdx = i;
        for (int j = i + 1; j < count; j++)
        {
            if (arr[j]->getAnalytics().totalSales() <
                arr[minIdx]->getAnalytics().totalSales())
            {
                minIdx = j;
            }
        }
        
        Store* temp = arr[i];
        arr[i] = arr[minIdx];
        arr[minIdx] = temp;
    }
}

void KMeans::buildSubClusters()
{
    for (int c = 0; c < k; c++)
    {
        // Name the three tiers
        string base = clusters[c].getClusterName();
        subClusters[c][0].setClusterName(base + " - High Performance");
        subClusters[c][1].setClusterName(base + " - Average Performance");
        subClusters[c][2].setClusterName(base + " - Low Performance");

        // Collect all Store* pointers for this geo-cluster
        int clusterSize = clusters[c].getStoreCount();
        if (clusterSize == 0) continue;

        Store** sorted = new Store * [clusterSize];

        for (int i = 0; i < clusterSize; i++)
            sorted[i] = clusters[c].getStore(i);

        // Sort ascending by totalSales
        sortByTotalSales(sorted, clusterSize);

        
        int lowEnd = clusterSize / 3;
        int midEnd = (clusterSize * 2) / 3;

       
        for (int i = 0; i < lowEnd; i++)
            subClusters[c][2].addStore(sorted[i]);

        for (int i = lowEnd; i < midEnd; i++)
            subClusters[c][1].addStore(sorted[i]);

        for (int i = midEnd; i < clusterSize; i++)
            subClusters[c][0].addStore(sorted[i]);

        delete[] sorted;
        // Note: NOT delete[] sorted[i] — that would destroy Stores
    }
}

void KMeans::run()
{
    if (storeCount == 0 || k <= 0) return;

    cout << "Running k-Means clustering (k=" << k << ", stores=" << storeCount << ")...\n";
    computeScalingBounds();

    seedCentroids();

    int iteration = 0;

    for (iteration = 0; iteration < maxIterations; iteration++)
    {
        bool changed = assignStores();

        if (!changed)
        {
            cout << "  Converged after " << iteration + 1 << " iteration(s).\n";
            break;
        }

        updateCentroids();
    }

    if (iteration == maxIterations)
        cout << "  Reached max iterations (" << maxIterations << ").\n";

    for (int c = 0; c < k; c++)
        clusters[c].clear();

    for (int i = 0; i < storeCount; i++)
    {
        int c = assignments[i];
        if (c >= 0 && c < k)
            clusters[c].addStore(&stores[i]);  
    }

 
    for (int c = 0; c < k; c++)
    {
        if (clusters[c].getStoreCount() > 0)
        {
            string city = clusters[c].getStore(0)->getCity();
            clusters[c].setClusterName(
                city + " Geo-Cluster " + to_string(c + 1)
            );
        }
    }

    buildSubClusters();

    cout << "  Clustering complete.\n";
}

void KMeans::printClusterReport() const
{
    cout << "\n";
    cout << "############################################\n";
    cout << "  GEOGRAPHIC CLUSTER REPORT (Tier 1)        \n";
    cout << "############################################\n\n";

    for (int c = 0; c < k; c++)
    {
        
        cout << clusters[c];

        cout << "  >> Performance Sub-Clusters (Tier 2):\n";

        for (int t = 0; t < subClusterTiers; t++)
        {
            cout << "    ";
            // Indent the sub-cluster report
            cout << subClusters[c][t].getClusterName()
                << " | Stores: " << subClusters[c][t].getStoreCount()
                << " | Revenue: "
                << fixed << setprecision(2)
                << subClusters[c][t].totalRevenue() << "\n";
        }
        cout << "\n";
    }
}


int KMeans::getK() const
{
    return k;
}

Cluster& KMeans::getCluster(int index) const
{
    return clusters[index];
}

Cluster& KMeans::getSubCluster(int clusterIdx, int tierIdx) const
{
    return subClusters[clusterIdx][tierIdx];
}