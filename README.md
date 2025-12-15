# Student Management System

A comprehensive Student Management System implemented using AVL Tree data structure in C++. This system efficiently manages student records with automatic tree balancing.

## Features

- **Self-Balancing AVL Tree**: Automatically maintains O(log n) time complexity for operations
- **Student Records**: Each student has a unique 5-digit ID (10000-99999) and name
- **Complete CRUD Operations**: Create, Read, Update, and Delete student records
- **Input Validation**: Ensures data integrity with proper validation
- **Sorted Display**: Shows all students sorted by ID using in-order traversal
- **Tree Information**: Display tree height and statistics
- **Single-File Implementation**: All code contained in one main.cpp file

## File Structure

```
├── main.cpp          # Complete Student Management System (single file)
├── Makefile          # Build configuration
├── Student.h         # Legacy: Student class definition (not used)
├── Student.cpp       # Legacy: Student class implementation (not used)
├── AVLTree.h         # Legacy: AVL Tree class definition (not used)
├── AVLTree.cpp       # Legacy: AVL Tree implementation (not used)
├── DSA_AVL_tree.cpp  # Original AVL tree implementation (demo)
└── process_manager.cpp  # Process manager demo
```

**Note**: The current implementation uses a **single-file approach** where all code (Student class, AVLNode structure, AVLTree class, and main function) is contained in `main.cpp`. The separate header and source files are legacy files kept for reference.

## Compilation

### Using Makefile (Recommended)

```bash
make              # Build the project
make clean        # Clean build artifacts
make run          # Build and run the program
```

### Manual Compilation

To compile the Student Management System manually (single file):

```bash
g++ -o student_management main.cpp -std=c++11
```

## Usage

Run the compiled program:

```bash
./student_management
```

### Menu Options

1. **Add Student**: Insert a new student record
2. **Remove Student**: Delete a student by ID
3. **Search Student**: Find a student by ID
4. **Display All Students**: Show all students sorted by ID
5. **Update Student**: Modify student information
6. **Display Tree Info**: Show tree statistics (height, total students)
7. **Exit**: Exit the program

### Example Usage

```
=== Student Management System ===
1. Add Student
2. Remove Student
3. Search Student
4. Display All Students
5. Update Student
6. Display Tree Info
7. Exit
Enter choice: 1
Enter Student ID (5-digit number, 10000-99999): 12345
Enter Student Name: John Doe
Successfully added student: ID=12345, Name="John Doe"

Enter choice: 3
Enter Student ID to search: 12345
Found: 12345 - John Doe

Enter choice: 4
=== All Students (sorted by ID) ===
  12345 - John Doe
```

## AVL Tree Operations

The implementation includes all necessary AVL tree operations:

- **Left Rotation**: Balances right-heavy subtrees
- **Right Rotation**: Balances left-heavy subtrees
- **Left-Right Rotation**: Handles left-right case
- **Right-Left Rotation**: Handles right-left case

### Time Complexity

- Insert: O(log n)
- Delete: O(log n)
- Search: O(log n)
- Display All: O(n)

### Space Complexity

- O(n) where n is the number of students

## Input Validation

- Student IDs must be 5-digit numbers (10000-99999)
- Duplicate IDs are not allowed
- Empty names are rejected
- Invalid menu choices are handled gracefully

## Memory Management

- Proper destructor implementation ensures no memory leaks
- Tree is destroyed recursively when the program exits
- All dynamically allocated nodes are properly freed

## Implementation Details

### Single-File Architecture

All code is contained in a single `main.cpp` file as per requirements:
- **Student Class**: Encapsulates student data (ID and name) with validation for 5-digit IDs
- **AVLNode Class**: Contains student data, left/right child pointers, and tracks node height for balancing
- **AVLTree Class**: Implements self-balancing AVL tree with automatic rotations after insertions and deletions
- **Main Function**: Provides menu-driven interface for user interaction

### AVL Tree Rotations

The implementation includes all necessary AVL tree rotations:
- **Left Rotation**: Balances right-heavy subtrees
- **Right Rotation**: Balances left-heavy subtrees
- **Left-Right Rotation**: Handles left-right case
- **Right-Left Rotation**: Handles right-left case

All rotations are performed automatically after insertions and deletions to maintain the AVL tree balance property.

## Testing

The system has been tested with:
- Adding multiple students
- Searching for existing and non-existing students
- Updating student information
- Deleting students and maintaining tree balance
- Validating input (invalid IDs, duplicates)
- Edge cases (empty tree, single node)

All tests pass successfully, demonstrating proper AVL tree functionality and correct handling of all operations.
