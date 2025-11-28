/**
 * ============================================================================
 * Process Management System with Middle-Packing Sequential Storage
 * ============================================================================
 * 
 * This file implements a process management system for an operating system
 * using a sequential storage structure (array-based) with middle-packing
 * arrangement.
 * 
 * ============================================================================
 * MIDDLE-PACKING ILLUSTRATION
 * ============================================================================
 * 
 * Array Capacity: 10 slots (indices 0-9)
 * 
 * Initial state (empty):
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │ _ │ _ │ _ │ _ │ _ │ _ │ _ │ _ │ _ │ _ │
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
 *   0   1   2   3   4   5   6   7   8   9
 *                     ↑
 *                   start=5, end=5
 * 
 * After inserting P1 (first process):
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │ _ │ _ │ _ │ _ │P1 │ _ │ _ │ _ │ _ │ _ │
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
 *   0   1   2   3   4   5   6   7   8   9
 *                 ↑   ↑
 *             start=4 end=5
 * 
 * After inserting P2 (at end):
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │ _ │ _ │ _ │ _ │P1 │P2 │ _ │ _ │ _ │ _ │
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
 *   0   1   2   3   4   5   6   7   8   9
 *                 ↑       ↑
 *             start=4   end=6
 * 
 * After inserting P3 (at start/front):
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │ _ │ _ │ _ │P3 │P1 │P2 │ _ │ _ │ _ │ _ │
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
 *   0   1   2   3   4   5   6   7   8   9
 *             ↑           ↑
 *         start=3       end=6
 * 
 * ============================================================================
 * WHY MIDDLE-PACKING INSTEAD OF STARTING FROM INDEX 0?
 * ============================================================================
 * 
 * Traditional 0-Based Approach:
 * - Elements start at index 0 and grow rightward
 * - Inserting at front requires shifting ALL elements right → O(n)
 * - Only back insertion is O(1)
 * 
 * Middle-Packing Approach:
 * - Elements start in the center of the array
 * - Can grow in BOTH directions (left and right)
 * - Front insertion: O(1) - just decrement start pointer
 * - Back insertion: O(1) - just increment end pointer
 * 
 * ADVANTAGES:
 * 1. Bidirectional O(1) insertions (when space available on both sides)
 * 2. Ideal for process schedulers where high-priority processes may be
 *    inserted at the front
 * 3. Balanced space utilization on both sides
 * 4. Reduces rebalancing frequency compared to single-side growth
 * 
 * WHEN REBALANCING OCCURS:
 * - When one side fills up but space exists on the other side
 * - Elements are shifted to re-center the data
 * - Example: start=0 but end < capacity → move elements to center
 * 
 * TRADE-OFFS:
 * | Aspect          | Middle-Packing     | 0-Based           |
 * |-----------------|--------------------|--------------------|
 * | Insert at end   | O(1) amortized     | O(1) amortized     |
 * | Insert at front | O(1) amortized     | O(n) always        |
 * | Space overhead  | Minimal            | None               |
 * | Complexity      | Moderate           | Simple             |
 * | Cache locality  | Excellent          | Excellent          |
 * | Rebalancing     | When unbalanced    | Never needed       |
 * 
 * ============================================================================
 * TIME COMPLEXITY ANALYSIS
 * ============================================================================
 * 
 * 1. INSERT OPERATION
 *    - Best case: O(1) - space available at preferred end
 *    - Average case: O(1) amortized
 *    - Worst case: O(n) - requires resize/rebalance
 * 
 * 2. UPDATE STATUS OPERATION
 *    - Best case: O(1) - process is at first position
 *    - Average case: O(n) - linear search required
 *    - Worst case: O(n) - process at last position or not found
 * 
 * 3. REMOVE READY PROCESS OPERATION
 *    - Best case: O(1) - ready process is at start or end
 *    - Average case: O(n) - search + potential shift
 *    - Worst case: O(n) - ready process in middle, requires shift
 * 
 * ============================================================================
 */

#include <iostream>
#include <string>

// ============================================================================
// DATA STRUCTURES
// ============================================================================

/**
 * Process status enumeration
 * Represents the possible states of a process in the system.
 */
enum class ProcessStatus {
    READY,      // Process is ready to run
    RUNNING,    // Process is currently executing
    WAITING,    // Process is waiting for an event
    TERMINATED  // Process has finished execution
};

/**
 * Convert ProcessStatus to string for display
 */
std::string statusToString(ProcessStatus status) {
    switch (status) {
        case ProcessStatus::READY:      return "READY";
        case ProcessStatus::RUNNING:    return "RUNNING";
        case ProcessStatus::WAITING:    return "WAITING";
        case ProcessStatus::TERMINATED: return "TERMINATED";
    }
    return "UNKNOWN"; // Should never reach here with valid enum
}

/**
 * Process structure
 * Contains all information about a single process.
 */
struct Process {
    int pid;              // Unique process identifier
    std::string name;     // Process name
    ProcessStatus status; // Current status
    int priority;         // Non-negative integer (smaller = higher priority)

    Process() : pid(0), name(""), status(ProcessStatus::READY), priority(0) {}

    Process(int pid, const std::string& name, ProcessStatus status, int priority)
        : pid(pid), name(name), status(status), priority(priority) {}
};

/**
 * ProcessList - Sequential storage with middle-packing arrangement
 * 
 * Elements are stored centered in the array to allow efficient bidirectional
 * growth. Both start and end indices can grow outward from the center.
 */
class ProcessList {
private:
    Process* array;      // Dynamic array storage
    size_t capacity;     // Total array capacity
    size_t start;        // Index of first element (inclusive)
    size_t end;          // Index one past last element (exclusive)

    static const size_t DEFAULT_CAPACITY = 10;

    /**
     * Pad a string with spaces on the left to achieve specified width.
     * If the string is already at or exceeds the width, it is returned unchanged.
     * Used as a replacement for std::setw() without requiring <iomanip>.
     */
    static std::string padLeft(const std::string& str, size_t width) {
        if (str.length() >= width) return str;
        return std::string(width - str.length(), ' ') + str;
    }

    /**
     * Rebalance elements to center them in the array
     * Called when one side fills up but there's space on the other side.
     * Time Complexity: O(n)
     */
    void rebalance() {
        size_t count = size();
        size_t newStart = (capacity - count) / 2;
        
        if (newStart == start) return;
        
        Process* temp = new Process[count];
        for (size_t i = 0; i < count; ++i) {
            temp[i] = array[start + i];
        }
        
        for (size_t i = 0; i < count; ++i) {
            array[newStart + i] = temp[i];
        }
        
        delete[] temp;
        start = newStart;
        end = newStart + count;
    }

    /**
     * Resize the array to a new capacity
     * Allocates a new array and copies elements to the center.
     * Time Complexity: O(n)
     */
    void resize(size_t newCapacity) {
        Process* newArray = new Process[newCapacity];
        size_t count = size();
        size_t newStart = (newCapacity - count) / 2;
        
        for (size_t i = 0; i < count; ++i) {
            newArray[newStart + i] = array[start + i];
        }
        
        delete[] array;
        array = newArray;
        capacity = newCapacity;
        start = newStart;
        end = newStart + count;
    }

    /**
     * Find index of process with given pid
     * Time Complexity: O(n)
     */
    size_t findByPid(int pid) const {
        for (size_t i = start; i < end; ++i) {
            if (array[i].pid == pid) return i;
        }
        return end; // Not found
    }

public:
    ProcessList(size_t initialCapacity = DEFAULT_CAPACITY) 
        : capacity(initialCapacity == 0 ? DEFAULT_CAPACITY : initialCapacity) {
        array = new Process[capacity];
        // Initialize start and end to the middle - KEY TO MIDDLE-PACKING
        start = capacity / 2;
        end = start;
    }

    ~ProcessList() { delete[] array; }

    // Disable copy to prevent shallow copies
    ProcessList(const ProcessList&) = delete;
    ProcessList& operator=(const ProcessList&) = delete;

    // ========================================================================
    // OPERATION 1: INSERT A PROCESS
    // ========================================================================
    /**
     * Insert a new process into the list (at the back)
     * 
     * Time Complexity:
     * - Best case: O(1) when there's space
     * - Worst case: O(n) when resize is needed
     * - Amortized: O(1)
     * 
     * @param p Process to insert
     * @return true if successful, false if duplicate pid
     */
    bool insert(const Process& p) {
        // Check for duplicate pid
        if (findByPid(p.pid) != end) return false;
        
        // Check if we need to make room at the back
        if (end >= capacity) {
            if (start > 0) {
                rebalance(); // Space at front, rebalance to center
            } else {
                resize(capacity * 2); // Completely full, resize
            }
        }
        
        // Insert at back - O(1)
        array[end] = p;
        ++end;
        return true;
    }

    /**
     * Insert at front - demonstrates middle-packing advantage
     * Time Complexity: O(1) amortized
     */
    bool insertFront(const Process& p) {
        if (findByPid(p.pid) != end) return false;
        
        if (start == 0) {
            if (end < capacity) {
                rebalance();
            } else {
                resize(capacity * 2);
            }
        }
        
        // Insert at front - O(1), just decrement start!
        --start;
        array[start] = p;
        return true;
    }

    // ========================================================================
    // OPERATION 2: UPDATE STATUS
    // ========================================================================
    /**
     * Update the status of a process with given pid
     * 
     * Time Complexity: O(n) - linear search required
     * 
     * @param pid Process ID to find
     * @param newStatus New status to set
     * @return true if found and updated, false otherwise
     */
    bool updateStatus(int pid, ProcessStatus newStatus) {
        size_t index = findByPid(pid);
        if (index == end) return false;
        
        array[index].status = newStatus;
        return true;
    }

    // ========================================================================
    // OPERATION 3: REMOVE READY PROCESS
    // ========================================================================
    /**
     * Remove an arbitrary ready process from the list
     * 
     * Removal strategy minimizes element shifts by:
     * - If at start: just increment start → O(1)
     * - If at end-1: just decrement end → O(1)
     * - If in middle: shift elements → O(n)
     * 
     * Time Complexity: O(n) - search + potential shift
     * 
     * @param removedPid Output - the pid of the removed process
     * @return true if a ready process was found and removed
     */
    bool removeReadyProcess(int& removedPid) {
        // Find first READY process - O(n)
        size_t readyIndex = end;
        for (size_t i = start; i < end; ++i) {
            if (array[i].status == ProcessStatus::READY) {
                readyIndex = i;
                break;
            }
        }
        
        if (readyIndex == end) return false; // No READY process found
        
        removedPid = array[readyIndex].pid;
        
        // Removal with minimal shifts
        if (readyIndex == start) {
            // At front - O(1) removal
            ++start;
        } else if (readyIndex == end - 1) {
            // At back - O(1) removal
            --end;
        } else {
            // In middle - shift to minimize work
            size_t distFromStart = readyIndex - start;
            size_t distFromEnd = end - 1 - readyIndex;
            
            if (distFromStart <= distFromEnd) {
                // Shift left portion right
                for (size_t i = readyIndex; i > start; --i) {
                    array[i] = array[i - 1];
                }
                ++start;
            } else {
                // Shift right portion left
                for (size_t i = readyIndex; i < end - 1; ++i) {
                    array[i] = array[i + 1];
                }
                --end;
            }
        }
        return true;
    }

    // ========================================================================
    // UTILITY FUNCTIONS
    // ========================================================================
    
    size_t size() const { return end - start; }
    bool isEmpty() const { return start == end; }
    size_t getCapacity() const { return capacity; }
    size_t getStartIndex() const { return start; }
    size_t getEndIndex() const { return end; }

    const Process* getByPid(int pid) const {
        size_t index = findByPid(pid);
        return (index == end) ? nullptr : &array[index];
    }

    /**
     * Print the current state showing middle-packing arrangement
     */
    void printState() const {
        std::cout << "\n=== ProcessList State (Middle-Packing) ===" << std::endl;
        std::cout << "Capacity: " << capacity << ", Size: " << size() << std::endl;
        std::cout << "Start: " << start << ", End: " << end << std::endl;
        
        std::cout << "\nArray visualization:" << std::endl;
        std::cout << "┌";
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "────";
            if (i < capacity - 1) std::cout << "┬";
        }
        std::cout << "┐" << std::endl;
        
        std::cout << "│";
        for (size_t i = 0; i < capacity; ++i) {
            if (i >= start && i < end) {
                std::cout << padLeft("P" + std::to_string(array[i].pid), 3) << " ";
            } else {
                std::cout << "  _ ";
            }
            std::cout << "│";
        }
        std::cout << std::endl;
        
        std::cout << "└";
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << "────";
            if (i < capacity - 1) std::cout << "┴";
        }
        std::cout << "┘" << std::endl;
        
        // Print indices with fixed width
        std::cout << " ";
        for (size_t i = 0; i < capacity; ++i) {
            std::cout << padLeft(std::to_string(i), 4) << " ";
        }
        std::cout << std::endl;
        
        // Show start/end markers
        std::cout << " ";
        for (size_t i = 0; i < capacity; ++i) {
            if (i == start && i == end) std::cout << " S=E ";
            else if (i == start) std::cout << "  S  ";
            else if (i == end) std::cout << "  E  ";
            else std::cout << "     ";
        }
        std::cout << std::endl;
        
        if (!isEmpty()) {
            std::cout << "\nProcesses:" << std::endl;
            for (size_t i = start; i < end; ++i) {
                std::cout << "  PID=" << array[i].pid 
                          << ", Name=" << array[i].name 
                          << ", Status=" << statusToString(array[i].status)
                          << ", Priority=" << array[i].priority << std::endl;
            }
        }
        std::cout << "==========================================\n" << std::endl;
    }
};

// ============================================================================
// DEMONSTRATION / TEST
// ============================================================================

int main() {
    std::cout << "============================================================" << std::endl;
    std::cout << "  Process Management System with Middle-Packing Demo" << std::endl;
    std::cout << "============================================================\n" << std::endl;

    ProcessList list(10);

    // Show initial empty state
    std::cout << ">>> Initial empty state (notice start=5, end=5 at center):" << std::endl;
    list.printState();

    // Insert processes
    std::cout << ">>> Inserting P1 at back:" << std::endl;
    list.insert(Process(1, "Init", ProcessStatus::READY, 1));
    list.printState();

    std::cout << ">>> Inserting P2 at back:" << std::endl;
    list.insert(Process(2, "Shell", ProcessStatus::READY, 2));
    list.printState();

    std::cout << ">>> Inserting P3 at FRONT (middle-packing advantage - O(1)):" << std::endl;
    list.insertFront(Process(3, "Kernel", ProcessStatus::RUNNING, 0));
    list.printState();

    std::cout << ">>> Inserting P4 at back, P5 at front:" << std::endl;
    list.insert(Process(4, "Driver", ProcessStatus::WAITING, 3));
    list.insertFront(Process(5, "Scheduler", ProcessStatus::READY, 0));
    list.printState();

    // Update status operation
    std::cout << ">>> Updating P2 status: READY → RUNNING" << std::endl;
    if (list.updateStatus(2, ProcessStatus::RUNNING)) {
        std::cout << "   Status updated successfully!" << std::endl;
    }
    list.printState();

    // Remove ready process operation
    std::cout << ">>> Removing first READY process:" << std::endl;
    int removedPid;
    if (list.removeReadyProcess(removedPid)) {
        std::cout << "   Removed process with PID=" << removedPid << std::endl;
    }
    list.printState();

    std::cout << ">>> Removing another READY process:" << std::endl;
    if (list.removeReadyProcess(removedPid)) {
        std::cout << "   Removed process with PID=" << removedPid << std::endl;
    }
    list.printState();

    // Test duplicate pid rejection
    std::cout << ">>> Attempting to insert duplicate PID=3:" << std::endl;
    if (!list.insert(Process(3, "Duplicate", ProcessStatus::READY, 5))) {
        std::cout << "   Correctly rejected duplicate PID!" << std::endl;
    }

    // Test update non-existent
    std::cout << "\n>>> Attempting to update non-existent PID=999:" << std::endl;
    if (!list.updateStatus(999, ProcessStatus::TERMINATED)) {
        std::cout << "   Correctly returned false for non-existent PID!" << std::endl;
    }

    // Test remove when no READY processes
    std::cout << "\n>>> Removing remaining READY processes until none left:" << std::endl;
    while (list.removeReadyProcess(removedPid)) {
        std::cout << "   Removed PID=" << removedPid << std::endl;
    }
    std::cout << "   No more READY processes to remove!" << std::endl;
    list.printState();

    std::cout << "============================================================" << std::endl;
    std::cout << "  Demo Complete - Middle-Packing Successfully Demonstrated!" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
