// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_PROG_H
#define RETRO_PROG_H

#include "gpu/renderer.h"
#include "util/arena.h"
#include "util/map.h"

/// Forward declares
typedef struct Statement Statement;
typedef struct ProgramTreeNode ProgramTreeNode;
typedef struct ProgramTreeIterator ProgramTreeIterator;

typedef struct ProgramTreeNode {
    Statement *stmt;
    ProgramTreeNode *left;
    ProgramTreeNode *right;
} ProgramTreeNode;

typedef struct ProgramTreeIterator {
    Statement *stmt;
    ProgramTreeIterator *next;
} ProgramTreeIterator;

typedef struct ProgramTree {
    ProgramTreeNode *root;
    MemoryArena arena;
} ProgramTree;

/// Creates a new program tree
/// @param tree The program tree
void program_tree_create(ProgramTree *tree);

/// Destroys the program tree
/// @param tree The program tree
void program_tree_destroy(ProgramTree *tree);

/// Clears all nodes of the program tree
void program_tree_clear(ProgramTree *tree);

/// Inserts the given statement into the program tree
/// @param tree The program tree
/// @param stmt The statement
void program_tree_insert(ProgramTree *tree, Statement *stmt);

/// Retrieves a program tree node from the given line
/// @param tree The program tree
/// @param line The line which is requested
/// @return The request program tree node or NULL
ProgramTreeNode *program_tree_get(ProgramTree *tree, usize line);

enum {
    PROGRAM_MARGIN_SIZE = 30,
    PROGRAM_MEMORY_SIZE = 0x10000
};

typedef struct Program {
    /// The symbols which are stored in the program.
    /// Symbols can be function definitions or user defined variables.
    HashMap *symbols;

    /// The program memory, which is usually 64 Kb.
    /// TODO(plank): Not really in use yet, we want to write some data
    /// (like keyboard input) to specific memory locations as described
    /// in the Applesoft BASIC spec.
    u8 memory[PROGRAM_MEMORY_SIZE];

    /// Required for text rendering
    Renderer *renderer;

    /// Next text position
    F32Vector2 text_position;

    /// A tree map that stores the lines of the actual program.
    ProgramTree lines;

    /// A boolean whose values indicates whether the program should wait
    /// for the users input to cancel execution. possible values are:
    /// - true: do not wait for user input and return to the source
    /// - false: wait until the user presses ESC and only then return
    ///          to source
    bool no_wait;

    /// The last key that was pressed by the user.
    /// TODO(plank): Replace with memory
    s32 last_key;

    /// The arena in which all program objects are allocated in.
    MemoryArena objects;
} Program;

/// Creates a program which serves as the handle between emulator and AST
/// @param self The program handle
/// @param renderer The renderer
void program_create(Program *self, Renderer *renderer);

/// Destroys the program and all its data
/// @param self The program handle
void program_destroy(Program *self);

/// Executes the program
/// @param self The program handle
void program_execute(Program *self);

/// Submits formatted text to the renderer
/// @param self The program handle
/// @param fmt The text format string
/// @param ... The variadic arguments
void program_print_format(Program *self, const char *fmt, ...);

#endif// RETRO_PROG_H
