/* !
@file    Particle.cpp
@author  Guo Chen (g.chen@digipen.edu)
@date    02/11/2023

Partical system using instancing rendering.

*//*__________________________________________________________________________*/
#include <Debug.h>
#include "../include/components/Particle.h"

/*  _________________________________________________________________________ */
/*
* init the particle system
* set position for each particle
* set up vertex data (and buffer(s)) and configure vertex attributes
*/
void ParticleSystem::Init()
{
    return;
    int index = 0;
    
    if(particles.size()>= PARTICLE_NUM) return;
   
    for(int i=0; i< PARTICLE_NUM; i++){
        
        translations[index] = random_value(pos_x_min,pos_x_max,pos_y_min,pos_y_max); 
        auto particle = std::make_unique<Particle>(&translations[index]); 
        particle->acceleration = random_value(acc_x_min,acc_x_max,acc_y_min,acc_y_max);
        particle->velocity = random_velocity(vel_x_min,vel_x_max,vel_y_min,vel_y_max);
        particle->life_time = random_life_time(life_min,life_max); 
        particles.push_back(std::move(particle)); 
        index++;
     }
    
    glGenBuffers(1, &instance_pos);
    glBindBuffer(GL_ARRAY_BUFFER, instance_pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * PARTICLE_NUM, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //init the shown buffer
    for (int i = 0; i < PARTICLE_NUM; i++) {
		shown[i] = 2.0f;
	}
    glGenBuffers(2, &instance_shown);
    glBindBuffer(GL_ARRAY_BUFFER, instance_shown);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * PARTICLE_NUM, &shown[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
   // ------------------------------------------------------------------
    float quadVertices[] = {
        // positions     // colors          // texture coords
        -0.1f,  0.1f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
         0.1f, -0.1f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        -0.1f, -0.1f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,

        -0.1f,  0.1f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
         0.1f, -0.1f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
         0.1f,  0.1f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f
    };
    
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
    glBindBuffer(GL_ARRAY_BUFFER, instance_pos); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, instance_shown); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(4, 1); // tell OpenGL this is an instanced vertex attribute.

    //particle_texture = GLApp::setup_texobj("Asset/Picture/p2.png");
    std::cout << "PARTICLE BEING USED == " << particle_texture << std::endl;
}

/*  _________________________________________________________________________ */
/*
* update the particle system
* get the player's position
* set the particle's position
*/
void ParticleSystem::Update(Object* player)
{
    return;
    static float accum_time = 0.0f;
    static int frame_dt_count = 0;

   if (engine->isPaused())
        return;
   
    frame_dt_count = engine->Get_NumOfSteps();
    while (frame_dt_count) {
        frame_dt_count--;
        switch (prticle_type) {
        case::PrticleType::Prticle_Finn:
            if (player != nullptr) {
                for (auto& ptc : this->particles)
                {

                    ptc->Update();
                    if (ptc->life_count > ptc->life_time) {
                        ptc->life_count = 0.0f;
                        ptc->position->x = random_value(pos_x_min, pos_x_max, pos_y_min, pos_y_max).x;
                        ptc->position->y = random_value(pos_x_min, pos_x_max, pos_y_min, pos_y_max).y;
                        ptc->velocity = random_velocity(vel_x_min, vel_x_max, vel_y_min, vel_y_max);
                        ptc->life_time = random_life_time(life_min, life_max);
                        //shown[&ptc - &particles[0]] = 2.0f;
                    }

                }
                // get the player's position
                Transform* tran_pt = static_cast<Transform*>(player->GetComponent(ComponentType::Transform));
                Physics* phy_pt = static_cast<Physics*>(player->GetComponent(ComponentType::Physics));
                if (phy_pt != nullptr) {

                    float Vx = phy_pt->Velocity.x;
                    float Vy = phy_pt->Velocity.y;
                    if (Vx == 0 && Vy == 0) {
                        draw_particle = false;
                        return;
                    }

                    draw_particle = true;
                    float orientation = atan2(Vy, Vx);


                    Vec2 pos = tran_pt->Position;
                    pos.x -= tran_pt->Scale.x * cos(orientation);
                    pos.y -= tran_pt->Scale.y * sin(orientation);
                    pos.x = pos.x * 2.0f / window->width_init;
                    pos.y = pos.y * 2.0f / window->height_init;
                    Vec2 scale{ 0.f,0.f };
                    scale.x = tran_pt->Scale.x / window->width_init;
                    scale.y = tran_pt->Scale.y / window->height_init;


                    world_to_ndc = Mat3Translate(pos) * Mat3Scale(scale) * Mat3RotRad(orientation);
                    /*Vec2 window_scaling{ (float)window->width / (float)window->width_init, (float)window->height / (float)window->height_init };
                    world_to_ndc = Mat3Scale(window_scaling.x, window_scaling.y) * world_to_ndc;*/
                    world_to_ndc = camera2D->world_to_ndc * world_to_ndc;
                }
            }
            break;
        case::PrticleType::Prticle_Explosion:
            if (prticle_state == ParticleState::Prticle_Start) {
                for (auto& ptc : this->particles)
                {

                    ptc->life_count = 0.0f;
                    ptc->position->x = random_value(pos_x_min, pos_x_max, pos_y_min, pos_y_max).x;
                    ptc->position->y = random_value(pos_x_min, pos_x_max, pos_y_min, pos_y_max).y;
                    ptc->acceleration = random_value(acc_x_min, acc_x_max, acc_y_min, acc_y_max);
                    ptc->velocity = random_velocity(vel_x_min, vel_x_max, vel_y_min, vel_y_max);
                    ptc->life_time = random_life_time(life_min, life_max);
                    shown[&ptc - &particles[0]] = 1.0f;

                }
                prticle_state = ParticleState::Prticle_Running;
            }

            if (prticle_state == ParticleState::Prticle_End) return;

            // if shown is all 2.0f, then change the state to end
            int count = 0;

            for (auto& ptc : this->particles)
            {

                ptc->Update();
                if (ptc->life_count > ptc->life_time) {
                    shown[&ptc - &particles[0]] = 2.0f;
                    count++;
                }
            }
            if (count == PARTICLE_NUM) {
                prticle_state = ParticleState::Prticle_End;
            }

            Transform* tran_pt = static_cast<Transform*>(player->GetComponent(ComponentType::Transform));
            //Physics* phy_pt = static_cast<Physics*>(player->GetComponent(ComponentType::Physics));

           
            Vec2 pos = tran_pt->Position;

            pos.x = pos.x * 2.0f / window->width_init;
            pos.y = pos.y * 2.0f / window->height_init;
            Vec2 scale{ 0.f,0.f };
            scale.x = tran_pt->Scale.x / window->width_init;
            scale.y = tran_pt->Scale.y / window->height_init;

            world_to_ndc = Mat3Translate(pos) * Mat3Scale(scale) * Mat3RotRad(0.0f);
           /* Vec2 window_scaling{ (float)window->width / (float)window->width_init, (float)window->height / (float)window->height_init };
            world_to_ndc = Mat3Scale(window_scaling.x, window_scaling.y) * world_to_ndc;*/
            world_to_ndc = camera2D->world_to_ndc * world_to_ndc;

            break;
        }

        // update the vbo

        glBindBuffer(GL_ARRAY_BUFFER, instance_pos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * PARTICLE_NUM, &translations[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, instance_shown);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * PARTICLE_NUM, &shown[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        
    }
    Draw();
}

/*  _________________________________________________________________________ */
/*
* draw the particle system
* use instancing rendering
*/
void ParticleSystem::Draw()
{
    return;
    //if (!draw_particle) return;
    glBindTextureUnit(6, particle_texture);
    glBindTexture(GL_TEXTURE_2D, particle_texture);
    
    
    AssetManager::shaderval("instancing").Use();
    
    glBindVertexArray(quadVAO);
    AssetManager::shaderval("instancing").SetUniform("uModel_to_NDC", world_to_ndc.ToGlmMat3());
    GLuint tex_loc = glGetUniformLocation(AssetManager::shaderval("instancing").GetHandle(), "uTex2d");
    glUniform1i(tex_loc, 6);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, PARTICLE_NUM); // 100 triangles of 6 vertices each
    glBindVertexArray(0);
    AssetManager::shaderval("instancing").UnUse();

   
}

/*  _________________________________________________________________________ */
/*
* free the particle system
*/
void ParticleSystem::Free()
{
    glDeleteTextures(1, &particle_texture);
    glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &instance_pos);
	particles.clear();

}

/*  _________________________________________________________________________ */
/*
* set the particle's position
* @param x_min, x_max, y_min, y_max: the range of the position
* 
*/
Vec2 random_value(float x_min, float x_max, float y_min, float y_max)
{
    Vec2 pos_return;
   if(x_min == x_max) pos_return.x = x_min;
   else pos_return.x = (rand() % 1000) / 1000.0f * (x_max - x_min) + x_min;
   if(y_min == y_max) pos_return.y = y_min;
   else pos_return.y = (rand() % 1000) / 1000.0f * (y_max - y_min) + y_min;

    return pos_return;
}

/*  _________________________________________________________________________ */
/*
* set the particle's velocity
* @param x_min, x_max, y_min, y_max: the range of the velocity
*/
Vec2 random_velocity(float x_min, float x_max, float y_min, float y_max)
{
    Vec2 vel_return;
    // x is from -1.0 to -0.5
    if (x_min == x_max) vel_return.x = x_min;
    else vel_return.x = (rand() % 1000) / 1000.0f * (x_max - x_min) + x_min;
    if (y_min == y_max) vel_return.y = y_min;
    else vel_return.y = (rand() % 1000) / 1000.0f * (y_max - y_min) + y_min;
    return vel_return;
}

/*  _________________________________________________________________________ */
/*
* set the particle's life time
* @param min, max: the range of the life time
*/
float random_life_time(float min, float max)
{
    if(min== max) return min;
    else return (rand() % 1000) / 1000.0f * (max - min) + min;
}

/*  _________________________________________________________________________ */
/*
* update the particle
*/

void ParticleSystem::Particle::Update()
{
        velocity = velocity + acceleration * engine->Get_Fixed_DT();
        Vec2 replacement = velocity * engine->Get_Fixed_DT();
        *position += replacement;
        life_count += engine->Get_Fixed_DT();
  
}
