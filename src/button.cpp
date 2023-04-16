#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Math.hpp"
#include "Entity.hpp"
#include "Button.hpp"
#include "RenderWindow.hpp"

Button::Button(RenderWindow& p_window, const char* p_text, TTF_Font* p_font, SDL_Color p_color, SDL_Texture* p_texture, Vector2f p_position)
    : Entity(p_position, p_texture), m_window(p_window), m_text(p_text), m_font(p_font), m_color(p_color), m_hovered(false), m_onClickTexture(nullptr), m_hoverTexture(nullptr), m_clicked(false)
{
   m_texture = p_texture;
   m_size = {255, 62};
   hoveredSfx = Mix_LoadWAV("res/sounds/button-hover.wav");
   clickSfx = Mix_LoadWAV("res/sounds/button-click.wav");
}

Button::~Button()
{
    Mix_FreeChunk(hoveredSfx);
    Mix_FreeChunk(clickSfx);
    hoveredSfx = nullptr;
    clickSfx = nullptr;
}

void Button::setSize(Vector2f p_size)
{
    m_size = p_size;
}

void Button::setOnClickTexture(SDL_Texture* texture)
{
    m_onClickTexture = texture;
}

void Button::setHoverTexture(SDL_Texture* texture)
{
    m_hoverTexture = texture;
}

void Button::setHoverDestination(Vector2f p_des)
{
    m_des = p_des;
}

void Button::update()
{
     // check if mouse is hovering over the button
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= getPos().getX() && mouseX <= getPos().getX() + m_size.getX()
        && mouseY >= getPos().getY() && mouseY <= getPos().getY() + m_size.getY())
    {
        if (m_hovered == false) Mix_PlayChannel(-1, hoveredSfx, 0);
        m_hovered = true;  

       if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            currentTime = SDL_GetTicks();
            if (currentTime - m_lastClickTime > 200) // 200ms delay
            {
                Mix_PlayChannel(-1, clickSfx, 0);
                m_lastClickTime = currentTime;
                m_clicked = !m_clicked;
            }
        }
    }
    else
    {
        m_hovered = false;
    }
}

void Button::render()
{
    if (m_clicked && m_onClickTexture) // Use the clicked texture if the button has been clicked and a clicked texture has been set
    {
        m_window.render(getPos(), m_onClickTexture);
    }
    else
    {
        m_window.render(getPos(), getTex());
    }

    // Calculate the position of the text
    int textWidth, textHeight;
    TTF_SizeText(m_font, m_text, &textWidth, &textHeight);
    Vector2f center = getPos() + m_size / Vector2f(2.0,2.0);
    Vector2f textPos = center - Vector2f(textWidth, textHeight) / Vector2f(2.0,2.0);

    if (m_hovered)
    {
        if (m_hoverTexture) {
            m_window.render(Vector2f(m_des), m_hoverTexture);
        }
        m_window.render(textPos, m_text, m_font, white);
    }
    else
    {
        m_window.render(textPos, m_text, m_font, m_color);
    }

}


