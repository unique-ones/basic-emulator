// Copyright (c) 2025 Elias Engelbert Plank

#include "prog.h"
#include "stmt.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/// Creates a new program tree
void program_tree_create(ProgramTree *tree) {
    tree->root = NULL;
    tree->arena = arena_identity(ALIGNMENT8);
}

/// Destroys the program tree
void program_tree_destroy(ProgramTree *tree) {
    tree->root = NULL;
    arena_destroy(&tree->arena);
}

/// Clears all nodes of the program tree
void program_tree_clear(ProgramTree *tree) {
    program_tree_destroy(tree);
    program_tree_create(tree);
}

/// Creates a program tree node
static ProgramTreeNode *program_tree_node_create(MemoryArena *arena, Statement *stmt) {
    ProgramTreeNode *self = arena_alloc(arena, sizeof(ProgramTreeNode));
    self->left = NULL;
    self->right = NULL;
    self->stmt = stmt;
    return self;
}

/// Insert the given statement at a feasible position starting at the specified node
static void program_tree_node_insert(MemoryArena *arena, ProgramTreeNode *node, Statement *stmt) {
    // TODO(elias): program tree should probably be ordered into heap rather than tree
    usize const node_line = node->stmt->line;
    usize const stmt_line = stmt->line;

    ProgramTreeNode **walk_position = NULL;
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
    } else {
        // Node at walk position, traverse down
        program_tree_node_insert(arena, *walk_position, stmt);
    }
}

/// Inserts the given statement into the program tree
void program_tree_insert(ProgramTree *tree, Statement *stmt) {
    if (tree->root == NULL) {
        tree->root = program_tree_node_create(&tree->arena, stmt);
    } else {
        program_tree_node_insert(&tree->arena, tree->root, stmt);
    }
}

static ProgramTreeNode *program_tree_node_get(ProgramTreeNode *node, usize const line) {
    if (node == NULL) {
        return NULL;
    }
    usize const node_line = node->stmt->line;
    if (node_line > line) {
        return program_tree_node_get(node->left, line);
    }
    if (node_line < line) {
        return program_tree_node_get(node->right, line);
    }
    return node;
}

/// Retrieves a program tree node from the given line
ProgramTreeNode *program_tree_get(ProgramTree const *tree, usize const line) {
    return program_tree_node_get(tree->root, line);
}

/// Creates a program which serves as the handle between emulator and AST
void program_create(Program *self, Renderer *renderer) {
    self->objects = arena_identity(ALIGNMENT8);
    self->symbols = hash_map_new();
    self->renderer = renderer;
    self->text_position.x = (f32) PROGRAM_MARGIN_SIZE;
    self->text_position.y = (f32) PROGRAM_MARGIN_SIZE;

    program_tree_create(&self->lines);
    self->last_key = -1;
    self->no_wait = false;
}

/// Destroys the program and all its data
void program_destroy(Program *self) {
    program_tree_destroy(&self->lines);

    hash_map_free(self->symbols);
    self->symbols = NULL;
    self->renderer = NULL;

    arena_destroy(&self->objects);
}

/// Executes a program tree node
static void program_tree_node_execute(ProgramTreeNode const *node, Program *program) {
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
void program_execute(Program *self) {
    self->text_position.x = PROGRAM_MARGIN_SIZE;
    self->text_position.y = PROGRAM_MARGIN_SIZE;
    program_tree_node_execute(self->lines.root, self);
}

/// Submits formatted text to the renderer
void program_print_format(Program *self, const char *fmt, ...) {
    char buffer[1024];
    va_list list;
    va_start(list, fmt);
    u32 length = (u32) vsnprintf(buffer, sizeof buffer, fmt, list);
    va_end(list);

    static F32Vector3 msg = { 1.0f, 0.55f, 0.0f };
    renderer_draw_text(self->renderer, &self->text_position, &msg, 0.5f, "%.*s", length, buffer);
}
