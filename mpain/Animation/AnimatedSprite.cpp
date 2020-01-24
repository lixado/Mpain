//
// Created by jakimi on 1.11.2019.
//

#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(sf::Time frameTime, bool paused, bool looped) :
        m_animation(NULL), m_frameTime(frameTime), m_currentFrame(0), m_isPaused(paused), m_isLooped(looped), m_texture(NULL)
{

}

void AnimatedSprite::setAnimation(const Animation& animation)
{
    m_animation = &animation;
    m_texture = m_animation->getSpriteSheet();
    m_currentFrame = 0;
    setFrame(m_currentFrame);
}

void AnimatedSprite::setFrameTime(sf::Time time)
{
    m_frameTime = time;
}

void AnimatedSprite::play()
{
    m_isPaused = false;
}

void AnimatedSprite::play(const Animation& animation)
{
    if (getAnimation() != &animation)
        setAnimation(animation);
    play();
}

// Pause the animation
void AnimatedSprite::pause()
{
    m_isPaused = true;
}

// Stop the animation and reset currentFrame
void AnimatedSprite::stop()
{
    m_isPaused = true;
    m_currentFrame = 0;
    setFrame(m_currentFrame);
}

// Set the animations on loop
void AnimatedSprite::setLooped(bool looped)
{
    m_isLooped = looped;
}

const Animation* AnimatedSprite::getAnimation() const
{
    return m_animation;
}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
    sf::IntRect rect = m_animation->getFrame(m_currentFrame);

    float width = static_cast<float>(std::abs(rect.width));
    float height = static_cast<float>(std::abs(rect.height));

    return sf::FloatRect(0.f, 0.f, width, height);
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

bool AnimatedSprite::isLooped() const
{
    return m_isLooped;
}

bool AnimatedSprite::isPlaying() const
{
    return !m_isPaused;
}

sf::Time AnimatedSprite::getFrameTime() const
{
    return m_frameTime;
}

void AnimatedSprite::setFrame(std::size_t newFrame, bool resetTime)
{
    if (m_animation)
    {
        // Calc new vertex pos and texture coords
        sf::IntRect rect = m_animation->getFrame(newFrame);

        m_vertices[0].position = sf::Vector2f(0.f, 0.f);
        m_vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
        m_vertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
        m_vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

        float left = static_cast<float>(rect.left) + 0.0001f;
        float right = left + static_cast<float>(rect.width);
        float top = static_cast<float>(rect.top);
        float bottom = top + static_cast<float>(rect.height);

        m_vertices[0].texCoords = sf::Vector2f(left, top);
        m_vertices[1].texCoords = sf::Vector2f(left, bottom);
        m_vertices[2].texCoords = sf::Vector2f(right, bottom);
        m_vertices[3].texCoords = sf::Vector2f(right, top);
    }

    if (resetTime)
        m_currentTime = sf::Time::Zero;
}

void AnimatedSprite::update(sf::Time deltaTime)
{
    // If not paused and we have a valid animation
    if (!m_isPaused && m_animation)
    {
        // Add delta time
        m_currentTime += deltaTime;

        // If current time is bigger then the frame time advance one frame
        if (m_currentTime >= m_frameTime)
        {
            // Reset time, but keep the remainder
            m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

            // Get next Frame index
            if (m_currentFrame + 1 < m_animation->getSize())
                m_currentFrame++;
            else
            {
                // Animation has ended
                if (!m_isLooped)
                {
                    m_isPaused = true;
                }
                else
                {
                    // Reset to start
                    m_currentFrame = 0;
                }
            }

            // Set the current frame, not resetting the time
            setFrame(m_currentFrame, false);
        }
    }
}

// Draw the frames
void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_animation && m_texture)
    {
        states.transform *= getTransform();
        states.texture = m_texture;
        target.draw(m_vertices, 4, sf::Quads, states);
    }
}
