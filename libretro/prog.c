//
// MIT License
//
// Copyright (c) 2024 Elias Engelbert Plank
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

#include "prog.h"
#include "stmt.h"

#include <stdarg.h>
#include <stdlib.h>


/// Creates a new program tree
void program_tree_create(program_tree_t *tree) {
    tree->root = NULL;
    tree->arena = arena_identity(ALIGNMENT8);
}

/// Destroys the program tree
void program_tree_destroy(program_tree_t *tree) {
    tree->root = NULL;
    arena_destroy(&tree->arena);
}

/// Clears all nodes of the program tree
void program_tree_clear(program_tree_t *tree) {
    program_tree_destroy(tree);
    program_tree_create(tree);
}

/// Creates a program tree node
static program_tree_node_t *program_tree_node_create(arena_t *arena, statement_t *stmt) {
    program_tree_node_t *self = (program_tree_node_t *) arena_alloc(arena, sizeof(program_tree_node_t));
    self->left = NULL;
    self->right = NULL;
    self->stmt = stmt;
    return self;
}

/// Insert the given statement at a feasible position starting at the specified node
static void program_tree_node_insert(arena_t *arena, program_tree_node_t *node, statement_t *stmt) {
    // TODO(elias): program tree should probably be ordered into heap rather than tree
    u32 node_line = node->stmt->line;
    u32 stmt_line = stmt->line;

    program_tree_node_t **walk_position = NULL;
    if (stmt_line > node_line) {
        walk_position = &node->right;
    } else if (stmt_line < node_line) {
        walk_position = &node->left;
    } else {
        // Change stmt at the current node
        node->stmt = stmt;
        return;
    }

    // No node at walk position
    if (*walk_position == NULL) {
        *walk_position = program_tree_node_create(arena, stmt);
        return;
    } else {
        // Node at walk position, traverse down
        program_tree_node_insert(arena, *walk_position, stmt);
    }
}

/// Inserts the given statement into the program tree
void program_tree_insert(program_tree_t *tree, statement_t *stmt) {
    if (tree->root == NULL) {
        tree->root = program_tree_node_create(&tree->arena, stmt);
    } else {
        program_tree_node_insert(&tree->arena, tree->root, stmt);
    }
}

static program_tree_node_t *program_tree_node_get(program_tree_node_t *node, u32 line) {
    if (node == NULL) {
        return NULL;
    }
    u32 node_line = node->stmt->line;
    if (node_line > line) {
        return program_tree_node_get(node->left, line);
    }
    if (node_line < line) {
        return program_tree_node_get(node->right, line);
    }
    return node;
}

/// Retrieves a program tree node from the given line
program_tree_node_t *program_tree_get(program_tree_t *tree, u32 line) {
    return program_tree_node_get(tree->root, line);
}

/// Creates a program which serves as the handle between emulator and AST
void program_create(program_t *self, renderer_t *renderer) {
    self->objects = arena_identity(ALIGNMENT8);
    self->symbols = map_new();
    self->renderer = renderer;
    self->text_position.x = (f32) PROGRAM_MARGIN_SIZE;
    self->text_position.y = (f32) PROGRAM_MARGIN_SIZE;

    program_tree_create(&self->lines);
    self->last_key = -1;
    self->no_wait = false;
}

/// Destroys the program and all its data
void program_destroy(program_t *self) {
    program_tree_destroy(&self->lines);

    map_free(self->symbols);
    self->symbols = NULL;
    self->renderer = NULL;

    arena_destroy(&self->objects);
}

/// Executes a program tree node
static void program_tree_node_execute(program_tree_node_t *node, program_t *program) {
    if (node == NULL) {
        return;
    }
    if (node->left != NULL) {
        program_tree_node_execute(node->left, program);
    }

    statement_execute(node->stmt, program);

    if (node->right != NULL) {
        program_tree_node_execute(node->right, program);
    }
}

/// Executes the program
void program_execute(program_t *self) {
    self->text_position.x = PROGRAM_MARGIN_SIZE;
    self->text_position.y = PROGRAM_MARGIN_SIZE;
    program_tree_node_execute(self->lines.root, self);
}

/// Submits formatted text to the renderer
void program_print_format(program_t *self, const char *fmt, ...) {
    char buffer[1024];
    va_list list;
    va_start(list, fmt);
    u32 length = (u32) vsnprintf(buffer, sizeof buffer, fmt, list);
    va_end(list);

    static f32vec3_t msg = { 1.0f, 0.55f, 0.0f };
    renderer_draw_text(self->renderer, &self->text_position, &msg, 0.5f, "%.*s", length, buffer);
}
