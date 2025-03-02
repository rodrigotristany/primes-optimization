#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex prime_mutex; // For safely adding primes to the list
std::vector<int> global_primes;

// Function to check if a number is prime
bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

auto start_timer() {
    return std::chrono::high_resolution_clock::now();
}

auto end_timer() {
    return std::chrono::high_resolution_clock::now();
}

// Single-threaded prime finder
std::vector<int> find_primes_single_thread(int limit) {
    std::vector<int> primes;
    for (int i = 2; i <= limit; ++i) {
        if (is_prime(i)) {
            primes.push_back(i);
        }
    }
    return primes;
}

// Multi-threaded prime finder
void find_primes_multi_thread(int from, int to, std::vector<int>& primes, std::mutex& prime_mutex) {
    for (int i = from; i <= to; ++i) {
        if (is_prime(i)) {
            std::lock_guard<std::mutex> lock(prime_mutex); 
            primes.push_back(i);
        }
    }
}

int main() {
    int limit = 10000000; // Adjust this to test performance
    std::cout << "Finding primes up to " << limit << "...\n";
    std::cout << "SINGLE THREAD" << "...\n";

    // Single-threaded execution
    auto start_time = start_timer();
    std::vector<int> primes = find_primes_single_thread(limit);
    auto end_time = end_timer();

    // Print first 10 primes (to check correctness)
    std::cout << "First 10 primes: ";
    for (size_t i = 0; i < std::min(primes.size(), size_t(10)); ++i) {
        std::cout << primes[i] << " ";
    }
    std::cout << "\nTotal primes found: " << primes.size() << std::endl;
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Single thread execution time: " << elapsed.count() << " seconds\n";

    std::cout << "\n\nMULTI THREAD" << std::endl;;
    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "Number of hardware threads available: " << cores << std::endl;

    // Multi-threaded execution - 10 threads
    start_time = start_timer();
    std::vector<std::thread> threads;
    int segments = limit / 10;
    
    for (int i = 0; i < 10; i++) {
        int from = segments * i + 1;
        int to = segments * (i + 1);
        threads.push_back(std::thread(find_primes_multi_thread, from, to, std::ref(global_primes), std::ref(prime_mutex)));
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    end_time = end_timer();
    
    std::cout << "\nTotal primes found: " << global_primes.size() << std::endl;
    elapsed = end_time - start_time;
    std::cout << "Multi thread execution time: " << elapsed.count() << " seconds\n";

    return 0;
}