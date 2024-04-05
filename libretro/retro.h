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

#ifndef RETRO_H
#define RETRO_H

#include "arch/thread.h"
#include "arch/time.h"

#include "gpu/buffer.h"
#include "gpu/glyph.h"
#include "gpu/renderer.h"
#include "gpu/shader.h"
#include "gpu/texture.h"

#include "util/list.h"
#include "util/map.h"
#include "util/math.h"
#include "util/random.h"
#include "util/stack.h"
#include "util/text.h"
#include "util/utility.h"

#include "display.h"
#include "emu.h"
#include "expr.h"
#include "input.h"
#include "lexer.h"
#include "prog.h"
#include "stmt.h"
#include "types.h"

#endif// RETRO_H
