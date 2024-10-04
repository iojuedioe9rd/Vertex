#pragma once
#include "VXEntities.h"
#include <Vertex.h>
#include <limits>  // For std::numeric_limits

namespace Vertex {

    template <typename T, T min>
    class ENTBaseHealthSystem : public Entity {
    public:
        ENTBaseHealthSystem() : m_health(min), m_maxHealth(min) {}

        ENTBaseHealthSystem(std::string name, Scene* scene) : Entity(name, scene), m_health(min), m_maxHealth(min) {}

        ~ENTBaseHealthSystem() = default;

        

        T GetHealth() const {
            return m_health;
        }

        T GetMaxHealth() const {
            return m_maxHealth;
        }

        T Damage(T Amount) {
            m_health -= Amount;
            if (m_health <= min)
            {
                m_health = min;
                OnDeath();
            }
            if (Amount > std::numeric_limits<T>::max() - m_health) m_health = min;
            return m_health;
        }

        T Heal(T Amount) {
            m_health += Amount;
            if (m_health > m_maxHealth) m_health = m_maxHealth;
            return m_health;
        }

        float GetHealthPercent() const {
            if (m_maxHealth == 0) return 0.0f;
            return static_cast<float>(m_health) / static_cast<float>(m_maxHealth);
        }

    private:
        T m_health;
        T m_maxHealth;

    protected:
        void Init(T health, T maxHealth) {
            m_health = (health < min) ? min : health;
            m_maxHealth = (maxHealth < min) ? min : maxHealth;
        }
    protected:
        // Virtual method for handling death, to be overridden by derived classes
        virtual void OnDeath() {
            VX_INFO("Player is Death");
        }

    };
}
