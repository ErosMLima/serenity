/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <AK/Function.h>
#include <LibGUI/Frame.h>
#include <LibGfx/BitmapFont.h>

class GlyphEditorWidget final : public GUI::Frame {
    C_OBJECT(GlyphEditorWidget)
public:
    virtual ~GlyphEditorWidget() override;

    void initialize(Gfx::BitmapFont&);

    int glyph() const { return m_glyph; }
    void set_glyph(int);

    void cut_glyph();
    void copy_glyph();
    void paste_glyph();
    void delete_glyph();
    void clear_clipboard_glyph();

    int preferred_width() const;
    int preferred_height() const;

    Gfx::BitmapFont& font() { return *m_font; }
    const Gfx::BitmapFont& font() const { return *m_font; }

    Function<void(u8)> on_glyph_altered;

private:
    GlyphEditorWidget() {};
    virtual void paint_event(GUI::PaintEvent&) override;
    virtual void mousedown_event(GUI::MouseEvent&) override;
    virtual void mousemove_event(GUI::MouseEvent&) override;

    void draw_at_mouse(const GUI::MouseEvent&);

    RefPtr<Gfx::BitmapFont> m_font;
    RefPtr<Gfx::Font> m_clipboard_font;
    Gfx::GlyphBitmap m_clipboard_glyph;
    int m_glyph { 0 };
    int m_scale { 10 };
};
