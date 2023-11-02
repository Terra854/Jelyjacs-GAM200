#include "Particle.h"

void ParticleSystem::Init()
{
    
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }


    instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // set up vertex data (and buffer(s)) and configure vertex attributes
   // ------------------------------------------------------------------
    float quadVertices[] = {
        // positions     // colors
        -0.05f,  0.05f,  1.0f, 1.0f, 1.0f,
         0.05f, -0.05f,  1.0f, 1.0f, 1.0f,
        -0.05f, -0.05f,  1.0f, 1.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 1.0f, 1.0f,
         0.05f, -0.05f,  1.0f, 1.0f, 1.0f,
         0.05f,  0.05f,  1.0f, 1.0f, 1.0f
    };
    quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    // also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
}

void ParticleSystem::Update()
{
    for (long i = 0; i < (long)objectFactory->GetNextId(); i++) {
        //if the object is a player
        if (static_cast<PlayerControllable*>((objectFactory->getObjectWithID(i))->GetComponent(ComponentType::PlayerControllable)) != nullptr) {
            // get the player's position
            Transform* tran_pt = static_cast<Transform*>(objectFactory->getObjectWithID(i)->GetComponent(ComponentType::Transform));
            Physics* phy_pt = static_cast<Physics*>((objectFactory->getObjectWithID(i))->GetComponent(ComponentType::Physics));
            if (tran_pt != nullptr) {
                
				Vec2 pos = tran_pt->Position;
                float Vx = phy_pt->Velocity.x;
                float Vy = phy_pt->Velocity.y;

                if (Vx == 0 && Vy == 0) {
                    continue;
                }
                Vec2 scale= tran_pt->Scale;
                //calculate rotation
                float orientation = atan2(Vy, Vx);
                pos.x -= Vx * 0.1f;
                pos.y -= Vy * 0.1f;



                
                world_to_ndc = Mat3Translate(pos) * Mat3Scale(scale) * Mat3RotRad(orientation);
                world_to_ndc = camera2D->world_to_ndc * world_to_ndc;
			}
        }
    }
}

void ParticleSystem::Draw()
{
    GLApp::shdrpgms["instancing"].Use();
    glBindVertexArray(quadVAO);
    GLApp::shdrpgms["instancing"].SetUniform("uModel_to_NDC", world_to_ndc.ToGlmMat3());
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
    glBindVertexArray(0);
    GLApp::shdrpgms["instancing"].UnUse();
}

void ParticleSystem::Free()
{
    glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}

