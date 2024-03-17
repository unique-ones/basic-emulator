//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef RETRO_PROG_H
#define RETRO_PROG_H

#include "util/arena.h"
#include "util/map.h"
#include "util/text.h"

/// Forward declares
typedef struct statement statement_t;
typedef struct program_tree_node program_tree_node_t;
typedef struct program_tree_iterator program_tree_iterator_t;

typedef struct program_tree_node {
    statement_t* stmt;
    program_tree_node_t* left;
    program_tree_node_t* right;
} program_tree_node_t;

typedef struct program_tree_iterator {
    statement_t* stmt;
    program_tree_iterator_t* next;
} program_tree_iterator_t;

typedef struct program_tree {
    program_tree_node_t* root;
    arena_t arena;
} program_tree_t;

/// Creates a new program tree
/// @param tree The program tree
void program_tree_create(program_tree_t* tree);

/// Destroys the program tree
/// @param tree The program tree
void program_tree_destroy(program_tree_t* tree);

/// Clears all nodes of the program tree
void program_tree_clear(program_tree_t* tree);

/// Inserts the given statement into the program tree
/// @param tree The program tree
/// @param stmt The statement
void program_tree_insert(program_tree_t* tree, statement_t* stmt);

/// Retrieves a program tree node from the given line
/// @param tree The program tree
/// @param line The line which is requested
/// @return The request program tree node or NULL
program_tree_node_t* program_tree_get(program_tree_t* tree, u32 line);

typedef struct program {
    /// The symbols which are stored in the program.
    /// Symbols can be function definitions or user defined variables.
    map_t* symbols;
    
    /// The program memory, which is usually 64 Kb.
    /// TODO(plank): Not really in use yet, we want to write some data
    /// (like keyboard input) to specific memory locations as described
    /// in the Applesoft BASIC spec.
    u8* memory;
    
    /// The text output queue, which is what gets displayed on the screen.
    /// Calls to functions like PRINT and HOME affect the output queue.
    text_queue_t* output;
    
    /// A tree map that stores the lines of the actual program.
    program_tree_t lines;
    
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
    arena_t objects;
} program_t;

/// Creates a program which serves as the handle between emulator and AST
/// @param self The program handle
/// @param memory_size The desired size of the program memory
void program_create(program_t* self, u32 memory_size);

/// Destroys the program and all its data
/// @param self The program handle
void program_destroy(program_t* self);

/// Executes the program
/// @param self The program handle
void program_execute(program_t* self);

#endif// RETRO_PROG_H
