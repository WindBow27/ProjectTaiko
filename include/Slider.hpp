#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "RenderWindow.hpp"
#include "Math.hpp"

class Slider {
public:
    Slider(RenderWindow& renderWindow, Vector2f position, Vector2f size, SDL_Texture* bgTexture, SDL_Texture* handleTexture) :
        m_renderWindow(renderWindow), m_position(position), m_size(size), m_bgTexture(bgTexture), m_handleTexture(handleTexture), m_hovered(false), m_dragging(false), m_handleTextureRect {m_position.x + m_size.x / 2 - 5, m_position.y + m_size.y / 2 - 5, 10, 10} {}

    void update(SDL_Event& event) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Check if mouse is hovering over the slider
        if (mouseX >= m_position.x && mouseX <= m_position.x + m_size.x &&
            mouseY >= m_position.y && mouseY <= m_position.y + m_size.y) {
            m_hovered = true;

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                m_dragging = true;
            }
            else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                m_dragging = false;
            }
        }
        else {
            m_hovered = false;
        }

        // Move handle if slider is being dragged
        if (m_dragging) {
            int handleX = mouseX - m_handleTextureRect.w / 2;
            if (handleX < m_position.x) handleX = m_position.x;
            if (handleX > m_position.x + m_size.x - m_handleTextureRect.w) handleX = m_position.x + m_size.x - m_handleTextureRect.w;
            m_handleTextureRect.x = handleX;
        }
    }

    void render() {
        // Render slider background
        m_renderWindow.render(m_position, m_bgTexture);

        // Render handle
        if (m_hovered || m_dragging) {
            SDL_SetTextureColorMod(m_handleTexture, 200, 200, 200); // Set handle color to gray when hovered or dragged
        }
        else {
            SDL_SetTextureColorMod(m_handleTexture, 255, 255, 255); // Set handle color to white when not hovered or dragged
        }
        m_renderWindow.render(m_handleTextureRect, m_handleTexture);
    }

private:
    RenderWindow& m_renderWindow;
    Vector2f m_position;
    Vector2f m_size;
    SDL_Texture* m_bgTexture;
    SDL_Texture* m_handleTexture;
    SDL_Rect m_handleTextureRect;
    bool m_hovered;
    bool m_dragging;
};
