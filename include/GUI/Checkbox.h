/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Dune Legacy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dune Legacy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "Button.h"
#include "GUIStyle.h"

#include <string>

/// A class for a checkbox implemented as a toggle button
class Checkbox : public Button {
public:
    /// Default constructor
    Checkbox() : Button() {
        textcolor = COLOR_DEFAULT;
        textshadowcolor = COLOR_DEFAULT;

        enableResizing(true,false);
        setToggleButton(true);
        pCheckedActiveTexture = nullptr;
        bFreeCheckedActiveTexture = false;
    }

    /// destructor
    virtual ~Checkbox() {
        invalidateTextures();
    }

    /**
        This method sets a new text for this checkbox and resizes it
        to fit this text.
        \param  Text The new text for this checkbox
    */
    virtual inline void setText(const std::string& text) {
        this->text = text;
        resizeAll();
    }

    /**
        Get the text of this checkbox.
        \return the text of this checkbox
    */
    inline const std::string& getText() { return text; };

    /**
        Sets the text color for this checkbox.
        \param  textcolor       the color of the text (COLOR_DEFAULT = default color)
        \param  textshadowcolor the color of the shadow of the text (COLOR_DEFAULT = default color)
    */
    virtual inline void setTextColor(Uint32 textcolor, Uint32 textshadowcolor = COLOR_DEFAULT) {
        this->textcolor = textcolor;
        this->textshadowcolor = textshadowcolor;
        invalidateTextures();
    }

    /**
        This method sets this checkbox to checked or unchecked. It does the same as setToggleState().
        \param bChecked true = checked, false = unchecked
    */
    inline void setChecked(bool bChecked) {
        setToggleState(bChecked);
    }

    /**
        This method returns whether this checkbox is checked. It is the same as getToggleState().
        \return true = checked, false = unchecked
    */
    inline bool isChecked() {
        return getToggleState();
    }

    /**
        Draws this button to screen. This method is called before drawOverlay().
        \param  position    Position to draw the button to
    */
    virtual void draw(Point position) {
        if(isVisible() == false) {
            return;
        }

        updateTextures();

        SDL_Texture* tex;
        if(isChecked()) {
            if((isActive() || bHover) && pCheckedActiveTexture != nullptr) {
                tex = pCheckedActiveTexture;
            } else {
                tex = pPressedTexture;
            }
        } else {
            if((isActive() || bHover) && pActiveTexture != nullptr) {
                tex = pActiveTexture;
            } else {
                tex = pUnpressedTexture;
            }
        }

        if(tex == nullptr) {
            return;
        }

        SDL_Rect dest = calcDrawingRect(tex, position.x, position.y);
        SDL_RenderCopy(renderer, tex, nullptr, &dest);
    }

    /**
        This method resizes the checkbox. This method should only
        called if the new size is a valid size for this progress bar (See getMinumumSize).
        \param  newSize the new size of this progress bar
    */
    virtual void resize(Point newSize) {
        resize(newSize.x,newSize.y);
    }

    /**
        This method resizes the checkbox to width and height. This method should only
        called if the new size is a valid size for this checkbox (See getMinimumSize).
        \param  width   the new width of this checkbox
        \param  height  the new height of this checkbox
    */
    virtual void resize(Uint32 width, Uint32 height) {
        invalidateTextures();
        Widget::resize(width,height);
    }

    /**
        Returns the minimum size of this button. The button should not
        resized to a size smaller than this.
        \return the minimum size of this button
    */
    virtual Point getMinimumSize() const {
        return GUIStyle::getInstance().getMinimumCheckboxSize(text);
    }

protected:
    /**
        This method is called whenever the textures of this widget are needed, e.g. before drawing. This method
        should be overwritten by subclasses if they like to defer texture creation as long as possible.
        This method should first check whether a renewal of the textures is necessary.
    */
    virtual void updateTextures() {
        Button::updateTextures();

        if(pUnpressedTexture == nullptr) {
            invalidateTextures();

            setSurfaces(GUIStyle::getInstance().createCheckboxSurface(getSize().x, getSize().y, text, false, false, textcolor, textshadowcolor),true,
                        GUIStyle::getInstance().createCheckboxSurface(getSize().x, getSize().y, text, true, false, textcolor, textshadowcolor),true,
                        GUIStyle::getInstance().createCheckboxSurface(getSize().x, getSize().y, text, false, true, textcolor, textshadowcolor),true);

            pCheckedActiveTexture = convertSurfaceToTexture(GUIStyle::getInstance().createCheckboxSurface(getSize().x, getSize().y, text, true, true, textcolor, textshadowcolor), true);
            bFreeCheckedActiveTexture = true;
        }
    }

    /**
        This method frees all textures that are used by this checkbox
    */
    virtual void invalidateTextures() {
        Button::invalidateTextures();

        if((bFreeCheckedActiveTexture == true) && (pCheckedActiveTexture != nullptr)) {
            SDL_DestroyTexture(pCheckedActiveTexture);
            bFreeCheckedActiveTexture = false;
        }
        pCheckedActiveTexture = nullptr;
    }

private:
    Uint32 textcolor;                       ///< Text color
    Uint32 textshadowcolor;                 ///< Text shadow color
    std::string text;                       ///< Text of this checkbox
    SDL_Texture* pCheckedActiveTexture;     ///< Texture that is shown when the checkbox is activated by keyboard or by mouse hover
    bool bFreeCheckedActiveTexture;         ///< Should pActiveTexture be freed if this button is destroyed?
};

#endif // CHECKBOX_H
