#pragma once
#include "raylib.h"
#include "raymath.h"
#include "vectormath.h"
#include <vector>

const float coll_responsivness = 0.75f;
const int sub_step = 16;

struct Particle
{
    Vector2 position;
    Vector2 previousPosition;
    Vector2 acceleration;
    float radius;
    Color color;
};

class ParticleSystem
{
public:
    ParticleSystem(float cons_particleRadius)
    {
        m_leftConstraint = { cons_particleRadius, cons_particleRadius };
        m_rightConstraint = { (float)GetScreenWidth() - cons_particleRadius, (float)GetScreenWidth() - cons_particleRadius };
        m_topConstraint = { cons_particleRadius, cons_particleRadius };
        m_bottomConstraint = { (float)GetScreenHeight() - cons_particleRadius, (float)GetScreenHeight() - cons_particleRadius };
    }

    void addParticle(Vector2 position, float radius, Color color)
    {
        Particle particle;
        particle.position = position;
        particle.previousPosition = position;
        particle.acceleration = { 0, 0 };
        particle.radius = radius;
        particle.color = color;
        m_particles.push_back(particle);
    }

    void deleteParticle()
    {
        auto it = std::find_if(m_particles.begin(), m_particles.end(), [](const auto& particle)
            {
                return CheckCollisionCircles(particle.position, particle.radius, GetMousePosition(), 2.0f);
            });

        if (it != m_particles.end())
        {
            m_particles.erase(it);
        }
    }

    void update(float deltaTime)
    {
        for (auto& particle : m_particles)
        {
            Vector2 temp = particle.position;
            particle.position = Vector2Add(particle.position, Vector2Add(Vector2Subtract(particle.position, particle.previousPosition), Vector2MultiplyValue(particle.acceleration, (deltaTime * deltaTime))));
            particle.previousPosition = temp;

            for (int i(sub_step); i--;) {
                if (particle.position.x < m_leftConstraint.x)
                {
                    particle.position.x = m_leftConstraint.x;
                    particle.previousPosition.x = particle.position.x + (particle.position.x - particle.previousPosition.x);
                }
                else if (particle.position.x > m_rightConstraint.x)
                {
                    particle.position.x = m_rightConstraint.x;
                    particle.previousPosition.x = particle.position.x + (particle.position.x - particle.previousPosition.x);
                }

                if (particle.position.y < m_topConstraint.y)
                {
                    particle.position.y = m_topConstraint.y;
                    particle.previousPosition.y = particle.position.y + (particle.position.y - particle.previousPosition.y);
                }
                else if (particle.position.y > m_bottomConstraint.y)
                {
                    particle.position.y = m_bottomConstraint.y;
                    particle.previousPosition.y = particle.position.y + (particle.position.y - particle.previousPosition.y);
                }

                for (auto& otherParticle : m_particles)
                {
                    if (&particle == &otherParticle)
                    {
                        continue;
                    }

                    Vector2 v = Vector2Subtract(particle.position, otherParticle.position);
                    float dist2 = v.x * v.x + v.y * v.y;
                    float min_distance = particle.radius + otherParticle.radius;

                    if (v.x < min_distance + 5.0f && v.y < min_distance + 5.0f)
                    {
                        if (dist2 < min_distance * min_distance) {
                            float distance = sqrt(dist2);
                            Vector2 n = Vector2DivideValue(v, distance);
                            float mass_calc_1 = particle.radius / (particle.radius + otherParticle.radius);
                            float mass_calc_2 = otherParticle.radius / (particle.radius + otherParticle.radius);
                            float delta = 0.5f * coll_responsivness * (distance - min_distance);

                            particle.position = Vector2Subtract(particle.position, Vector2MultiplyValue(n, (mass_calc_2 * delta)));
                            otherParticle.position = Vector2Add(otherParticle.position, Vector2MultiplyValue(n, (mass_calc_1 * delta)));
                        }
                    }
                }
            }
        }
    }

    void addAcceleration(Vector2 acc)
    {
        for (auto& particle : m_particles)
        {
            particle.acceleration = Vector2Add(particle.acceleration, acc);
        }
    }

    void setVelocity(Vector2 vel, float deltatime)
    {
        for (auto& particle : m_particles)
        {
            particle.previousPosition = Vector2Subtract(particle.position, Vector2MultiplyValue(vel, deltatime));
        }
    }

    void addVelocity(Vector2 vel, float deltatime)
    {
        for (auto& particle : m_particles)
        {
            particle.previousPosition = Vector2Subtract(particle.previousPosition, Vector2MultiplyValue(vel, deltatime));
        }
    }

    void resetAcceleration()
    {
        for (auto& particle : m_particles)
        {
            particle.acceleration = { 0.0f,0.0f };
        }
    }

    void draw()
    {
        for (auto& particle : m_particles)
        {
            DrawCircleV(particle.position, particle.radius + 1, WHITE);
            DrawCircleV(particle.position, particle.radius, particle.color);
        }
    }

private:
    std::vector<Particle> m_particles;
    Vector2 m_leftConstraint;
    Vector2 m_rightConstraint;
    Vector2 m_topConstraint;
    Vector2 m_bottomConstraint;
};