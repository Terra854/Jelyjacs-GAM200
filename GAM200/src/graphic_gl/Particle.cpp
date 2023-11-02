#include "Particle.h"

void ParticleSystem::Init()
{
    
    int index = 0;
    
    for (int y = 0;y < 10;y++)
    {
        for (int x = 0; x < 10; x++)
        {
            glm::vec2 translation;
            translation.x = 1.0f - (float) (x * x) / 50.0f;
            translation.y = (float) ((y-5) * x) / 50.0f;
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
        -0.05f,  0.05f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
         0.05f, -0.05f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        -0.05f, -0.05f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

        -0.05f,  0.05f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
         0.05f, -0.05f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
         0.05f,  0.05f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f
    };
    quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    // also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    particle_texture = GLApp::setup_texobj("Asset/Picture/particle.png");
}

void ParticleSystem::Update()
{
    for (long i = 0; i < (long)objectFactory->GetNextId(); i++) {
        //if the object is a player
        if (static_cast<PlayerControllable*>((objectFactory->getObjectWithID(i))->GetComponent(ComponentType::PlayerControllable)) != nullptr) {
            // get the player's position
            Transform* tran_pt = static_cast<Transform*>(objectFactory->getObjectWithID(i)->GetComponent(ComponentType::Transform));
            Physics* phy_pt = static_cast<Physics*>((objectFactory->getObjectWithID(i))->GetComponent(ComponentType::Physics));
            if (phy_pt != nullptr) {
                
                float Vx = phy_pt->Velocity.x;
                float Vy = phy_pt->Velocity.y;
                if (Vx == 0 && Vy == 0) {
                    draw_particle = false;
                    continue;
                }
                draw_particle = true;
                Vec2 pos = tran_pt->Position;
                pos.x -= Vx * 0.1f;
                pos.y -= Vy * 0.1f;
                pos.x = pos.x * 2.0f / window->width;
                pos.y = pos.y * 2.0f / window->height;
                Vec2 scale{0.f,0.f};
                scale.x = tran_pt->Scale.x / window->width;
                scale.y = tran_pt->Scale.y / window->height;
                scale.x *= sqrt(Vx * Vx + Vy * Vy) * 0.002f ;
                scale.y *= sqrt(Vx * Vx + Vy * Vy) * 0.002f;
                //calculate rotation
                float orientation = atan2(Vy, Vx);
               
                world_to_ndc = Mat3Translate(pos) * Mat3Scale(scale) * Mat3RotRad(orientation);
                world_to_ndc = camera2D->world_to_ndc * world_to_ndc;
			}
        }
    }
}

void ParticleSystem::Draw()
{
    if (!draw_particle) return;
    glBindTextureUnit(6, particle_texture);
    glBindTexture(GL_TEXTURE_2D, particle_texture);
    GLApp::shdrpgms["instancing"].Use();
    glBindVertexArray(quadVAO);
    GLApp::shdrpgms["instancing"].SetUniform("uModel_to_NDC", world_to_ndc.ToGlmMat3());
    GLuint tex_loc = glGetUniformLocation(GLApp::shdrpgms["instancing"].GetHandle(), "uTex2d");
    glUniform1i(tex_loc, 6);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
    glBindVertexArray(0);
    GLApp::shdrpgms["instancing"].UnUse();

   
}

void ParticleSystem::Free()
{
    glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
}

