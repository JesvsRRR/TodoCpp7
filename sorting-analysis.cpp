#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;
using namespace std::chrono;

// Declaración de funciones de ordenamiento
void bubbleSort(vector<int>& arr);
void countingSort(vector<int>& arr);
void heapSort(vector<int>& arr);
void insertionSort(vector<int>& arr);
void mergeSort(vector<int>& arr, int left, int right);
void quickSort(vector<int>& arr, int low, int high);
void selectionSort(vector<int>& arr);

// Funciones auxiliares
void heapify(vector<int>& arr, int n, int i);
void merge(vector<int>& arr, int left, int mid, int right);
int partition(vector<int>& arr, int low, int high);
vector<int> readFile(const string& filename);

// Tamaños de archivos
const vector<int> SIZES = {100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 
                          10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};

int main() {
    const string DIRECTORY_PATH = "C:/Users/Usuario/Desktop/CppTODO/"; 

    // Vector de pares (nombre del algoritmo, puntero a función)
    vector<pair<string, void(*)(vector<int>&)>> algorithms = {
        {"Bubble Sort", bubbleSort},
        {"Counting Sort", countingSort},
        {"Heap Sort", heapSort},
        {"Insertion Sort", insertionSort},
        {"Selection Sort", selectionSort}
    };

    // Para cada algoritmo
    for (const auto& algo : algorithms) {
        cout << algo.first << " :";
        
        // Para cada tamaño de archivo
        for (int size : SIZES) {
            string filename = DIRECTORY_PATH + "file_" + to_string(size) + ".txt";
            vector<int> arr = readFile(filename);
            
            if (!arr.empty()) {
                // Medir tiempo
                auto start = high_resolution_clock::now();
                algo.second(arr);
                auto end = high_resolution_clock::now();
                
                // Calcular duración en milisegundos
                auto duration = duration_cast<milliseconds>(end - start);
                cout << " " << duration.count() << "ms";
            } else {
                cout << " ERROR";
            }
        }
        cout << endl;
    }

    // Manejar Merge Sort y Quick Sort separadamente debido a sus parámetros adicionales
    cout << "Merge Sort :";
    for (int size : SIZES) {
        string filename = DIRECTORY_PATH + "file_" + to_string(size) + ".txt";
        vector<int> arr = readFile(filename);
        
        if (!arr.empty()) {
            auto start = high_resolution_clock::now();
            mergeSort(arr, 0, arr.size() - 1);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            cout << " " << duration.count() << "ms";
        } else {
            cout << " ERROR";
        }
    }
    cout << endl;

    cout << "Quick Sort :";
    for (int size : SIZES) {
        string filename = DIRECTORY_PATH + "file_" + to_string(size) + ".txt";
        vector<int> arr = readFile(filename);
        
        if (!arr.empty()) {
            auto start = high_resolution_clock::now();
            quickSort(arr, 0, arr.size() - 1);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            cout << " " << duration.count() << "ms";
        } else {
            cout << " ERROR";
        }
    }
    cout << endl;

    return 0;
}

vector<int> readFile(const string& filename) {
    vector<int> numbers;
    ifstream file(filename);
    string line;
    
    if (file.is_open()) {
        getline(file, line);
        // Eliminar corchetes
        line = line.substr(line.find('[') + 1);
        line = line.substr(0, line.find(']'));
        
        // Crear stream para procesar números
        stringstream ss(line);
        string number;
        
        // Leer números separados por comas
        while (getline(ss, number, ',')) {
            numbers.push_back(stoi(number));
        }
        
        file.close();
    }
    
    return numbers;
}

void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void countingSort(vector<int>& arr) {
    int max = *max_element(arr.begin(), arr.end());
    int min = *min_element(arr.begin(), arr.end());
    int range = max - min + 1;
    
    vector<int> count(range), output(arr.size());
    
    for (int i : arr) {
        count[i - min]++;
    }
    
    for (int i = 1; i < count.size(); i++) {
        count[i] += count[i - 1];
    }
    
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - min] - 1] = arr[i];
        count[arr[i] - min]--;
    }
    
    arr = output;
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        swap(arr[i], arr[min_idx]);
    }
}