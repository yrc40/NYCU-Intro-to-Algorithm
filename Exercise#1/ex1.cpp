#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include <queue>

using namespace std;

const int INF = numeric_limits<int>::max();

// Helper function to print the Young tableau
void printTableau(ofstream &file, const vector<vector<int>>& tableau) {
    for (const auto& row : tableau) {
        for (const auto& elem : row) {
            if (elem == INF) {
                file << "x ";
            } else {
                file << elem << " ";
            }
        }
        file << "\r\n";  // Windows CRLF
    }
}

// Helper function to min-heapify the Young tableau
void minHeapify(vector<vector<int>>& tableau, int m, int n, int i, int j) {
    int smallest_i = i, smallest_j = j;

    if (i + 1 < m && tableau[i + 1][j] < tableau[smallest_i][smallest_j]) {
        smallest_i = i + 1;
        smallest_j = j;
    }
    if (j + 1 < n && tableau[i][j + 1] < tableau[smallest_i][smallest_j]) {
        smallest_i = i;
        smallest_j = j + 1;
    }
    
    if (smallest_i != i || smallest_j != j) {
        swap(tableau[i][j], tableau[smallest_i][smallest_j]);
        minHeapify(tableau, m, n, smallest_i, smallest_j);
    }
}

// Function to insert an element into the Young tableau
void insertElement(vector<vector<int>>& tableau, int m, int n, int element) {
    if (tableau[m - 1][n - 1] < INF) {
        cout << "Tableau is full!" << endl;
        return;
    }
    
    int i = m - 1, j = n - 1;
    tableau[i][j] = element;
    
    while (i > 0 || j > 0) {
        int parent_i = i - (i > 0), parent_j = j - (j > 0);
        if (tableau[parent_i][parent_j] > tableau[i][j]) {
            swap(tableau[parent_i][parent_j], tableau[i][j]);
            i = parent_i;
            j = parent_j;
        } else {
            break;
        }
    }
}

// Function to extract the minimum element from the Young tableau
int extractMin(vector<vector<int>>& tableau, int m, int n) {
    int minElement = tableau[0][0];
    tableau[0][0] = INF;
    minHeapify(tableau, m, n, 0, 0);
    return minElement;
}

int main() {
    ifstream inputFile("input.txt");
    ofstream outputFile("output2.txt");
    
    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Fail to open file." << endl;
        return 1;
    }

    int numTests;
    inputFile >> numTests;

    for (int t = 0; t < numTests; ++t) {
        int operation;
        inputFile >> operation;
         inputFile.ignore(1);

        string line;
        if (getline(inputFile, line)) {
            cout << "insert target: " << line << endl;
        } else {
            cerr << "fail to read line" << endl;
        }
        int m, n;
        inputFile >> m >> n;
        vector<vector<int>> tableau(m, vector<int>(n, INF));

        if (operation == 1) {
            int k;
            inputFile >> k;
            vector<int> elements(k);
            for (int &el : elements) inputFile >> el;
            outputFile << "Insert ";
            for (int el : elements) outputFile << el << " ";
            outputFile << "\r\n";
            
            for (auto& row : tableau) {
                for (int &el : row) {
                    string cell;
                    inputFile >> cell;
                    if (cell == "x") {
                        el = INF;
                    } else {
                        try {
                            el = stoi(cell);
                        } catch (const std::invalid_argument& e) {
                            cerr << "無效的數字格式: " << cell << endl;
                            el = INF;  // 或其他處理方式
                        }
                    }
                }
            }
            
            for (int el : elements) insertElement(tableau, m, n, el);
            printTableau(outputFile, tableau);
        } 
        else if (operation == 2) {
            outputFile << "Extract-min ";
            
            for (auto& row : tableau) {
                for (int &el : row) {
                    string cell;
                    inputFile >> cell;
                    if (cell == "x") {
                        el = INF;
                    } else {
                        try {
                            el = stoi(cell);
                        } catch (const std::invalid_argument& e) {
                            cerr << "無效的數字格式: " << cell << endl;
                            el = INF;  // 或其他處理方式
                        }
                    }
                }
            }
            
            int minElement = extractMin(tableau, m, n);
            outputFile << minElement << "\r\n";
            printTableau(outputFile, tableau);
        }

        outputFile << "\r\n";
    }

    inputFile.close();
    outputFile.close();
    return 0;
}