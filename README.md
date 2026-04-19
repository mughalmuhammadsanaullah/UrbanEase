# UrbanEase: Store Analytics and Advanced Management System

| **Course:** Object Oriented Programming  
| **Assignment:** #3     
| National University of Computer & Emerging Sciences, Islamabad

---

## What's the Problem?

UrbanEase is a fast-growing fashion retail brand operating hundreds of stores across Pakistan's major cities — Karachi, Lahore, Islamabad, and Faisalabad. As the business scales, manually tracking store performance, understanding regional sales patterns, managing logistics, and forecasting future sales has become increasingly complex. There was no centralized system capable of extracting meaningful insights from store data in an efficient and structured way.

---

## How We Solved It

We designed and implemented a fully object-oriented C++ analytics system built around a clean class hierarchy. The solution is divided into four core tasks:

- **Data Initialisation (Task 0):** On startup, the program either loads previously saved store data from a file or generates a fresh dataset (minimum 100 stores) with realistic randomized values. All data persists across program runs via file I/O.

- **Structured Data Representation (Task 1):** Stores are modeled through a rich class hierarchy — a `Person` base class with `Employee` and `Manager` derived classes, a `Store` class composed of `GeoCoordinates`, `Analytics`, and `Forecast` objects, and aggregator classes like `Cluster` and `Report`.

- **Geographic Clustering (Task 2):** A k-Means clustering algorithm groups stores first by geographic proximity (latitude/longitude), then sub-groups them by profitability — helping optimize inventory distribution and regional logistics.

- **Performance Ranking (Task 3):** Stores are scored using a weighted composite metric combining total annual sales, average monthly growth, and profitability. A ranked report highlights the top 10 and bottom 5 stores.

- **Sales Forecasting (Task 4):** Each store's next month's sales are predicted using moving averages with trend and momentum adjustments. Forecasts include a confidence percentage and a warning flag for declining stores.

---

## Expected Outcome

- A program that loads or generates data for 500+ stores and runs a full analytics pipeline without any hard-coded values.
- Cluster reports showing store groupings with total revenue summaries.
- A ranked leaderboard of store performance with composite scores.
- Per-store sales forecasts with confidence levels and decline warnings.
- Demonstration of all required OOP concepts: inheritance, polymorphism, operator overloading, dynamic memory management, composition, and aggregation.

---

## Technologies Used

- **Language:** C++ (Standard: C++11 or later)
- **Concepts:** Object-Oriented Programming — inheritance, polymorphism, operator overloading, dynamic memory (`new`/`delete`), deep copy constructors, composition, aggregation
- **I/O:** Plain-text file-based persistent storage
- **Algorithm:** k-Means clustering, weighted scoring, moving average forecasting
- **Tools:** Any standard C++ compiler (g++, MSVC, Clang)
