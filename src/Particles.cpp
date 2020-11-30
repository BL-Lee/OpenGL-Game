#include "Particles.h"
#include "glm/gtx/compatibility.hpp"

Particle ParticleSystem::s_BaseParticle;
std::vector<Particle> ParticleSystem::s_ParticlePool;
uint32_t ParticleSystem::s_TotalParticles;
uint32_t ParticleSystem::s_ParticleIndex;

void ParticleSystem::Init(uint32_t amount)
{
	s_ParticlePool = std::vector<Particle>(amount);
	fill(s_ParticlePool.begin(), s_ParticlePool.end(), s_BaseParticle);
	s_ParticleIndex = 0;
}
void ParticleSystem::SetParticle(const Particle& p)
{
	memcpy(&s_BaseParticle, &p, sizeof(Particle));
}
void ParticleSystem::Add(uint32_t amount)
{
	for (uint32_t i = 0; i < amount; i++)
	{
		s_ParticlePool[s_ParticleIndex] = s_BaseParticle;
		s_ParticlePool[s_ParticleIndex].IsActive = true;
		s_ParticlePool[s_ParticleIndex].Velocity = { (Random::Float() - 0.5f) * 300.0f, (Random::Float() - 0.5f) * 300.0f};
		s_ParticleIndex = (s_ParticleIndex + 1) % s_ParticlePool.size();
	} 
}
void ParticleSystem::Add(const Particle& p)
{
	s_ParticlePool[s_ParticleIndex] = p;
	s_ParticlePool[s_ParticleIndex].IsActive = true;
	s_ParticlePool[s_ParticleIndex].Velocity = { (Random::Float() - 0.5f) * 10.0f, (Random::Float() - 0.5f)};
	s_ParticleIndex = (s_ParticleIndex + 1) % s_ParticlePool.size();
}
void ParticleSystem::Draw(float deltaTime)
{
	for (uint32_t i = 0; i < s_ParticlePool.size(); i++)
	{
		Particle& p = s_ParticlePool[s_ParticleIndex];
		s_ParticleIndex = (s_ParticleIndex + 1) % s_ParticlePool.size();

		if (!p.IsActive)
			continue;

		p.Pos += p.Velocity * deltaTime;
		p.Rotation += p.RotationDelta * deltaTime;

		Renderer::DrawRotatedQuad({ p.Pos,0 },
			glm::lerp(p.Size, p.EndSize, p.CurrentLifetime / p.Lifetime),
			p.Rotation,
			glm::lerp(s_BaseParticle.StartColour, s_BaseParticle.EndColour, p.CurrentLifetime / s_BaseParticle.Lifetime));
		p.CurrentLifetime += deltaTime;
		if (p.CurrentLifetime > p.Lifetime)
			p.IsActive = false;

		
	}
}