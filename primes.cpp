#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex prime_mutex; // For safely adding primes to the list

// Function to check if a number is prime
bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
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

// TODO: Implement a multithreaded version

int main() {
    int limit = 100000; // Adjust this to test performance
    std::cout << "Finding primes up to " << limit << "...\n";

    // Single-threaded execution
    std::vector<int> primes = find_primes_single_thread(limit);

    // Print first 10 primes (to check correctness)
    std::cout << "First 10 primes: ";
    for (size_t i = 0; i < std::min(primes.size(), size_t(10)); ++i) {
        std::cout << primes[i] << " ";
    }
    std::cout << "\nTotal primes found: " << primes.size() << std::endl;

    return 0;
}
