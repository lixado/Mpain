//
// Created by jakimi on 1.11.2019.
//

#include "Animation.h"

Animation::Animation() : m_texture(NULL)
{

}

// Pushes frames from the spritesheet to my_frames vector
void Animation::addFrame(sf::IntRect rect)
{
    m_frames.push_back(rect);
}

// Sets sprite in texture
void Animation::setSpriteSheet(const sf::Texture& texture)
{
    m_texture = &texture;
}

const sf::Texture* Animation::getSpriteSheet() const
{
    return m_texture;
}

std::size_t Animation::getSize() const
{
    return m_frames.size();
}

const sf::IntRect& Animation::getFrame(std::size_t n) const
{
    return m_frames[n];
}