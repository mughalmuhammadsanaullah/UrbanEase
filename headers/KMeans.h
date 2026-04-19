#pragma once

#include "Cluster.h"
#include "Centroid.h"

class KMeans
{
private:

    Store* stores;      
    int storeCount;  
    int k;

    Cluster* clusters;

    Centroid* centroids;

    int* assignments;

    Cluster** subClusters;  
    int subClusterTiers;  

    double salesMin, salesMax;
    double latMin, latMax;
    double lonMin, lonMax;

    int maxIterations;

    void computeScalingBounds();

    Centroid storeAsCentroid(int storeIndex) const;

    double normaliseSales(double sales) const;
    double normaliseLat(double lat) const;
    double normaliseLon(double lon) const;

    void seedCentroids();

    bool assignStores();

    void updateCentroids();

    void buildSubClusters();

    void sortByTotalSales(Store** arr, int count);

public:

    KMeans(Store* stores, int storeCount, int k = 6);
    ~KMeans();

  
    void run();

    void printClusterReport() const;

    int getK() const;
    Cluster& getCluster(int index) const;
    Cluster& getSubCluster(int clusterIdx, int tierIdx) const;
};