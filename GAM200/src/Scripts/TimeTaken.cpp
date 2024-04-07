/* !
@author Yeo Jia Ming
@date	1/3/2024

This file contains the script for the win screen time taken
*//*__________________________________________________________________________*/
#include "Scripts/TimeTaken.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <../components/Text.h>
#include <../components/Texture.h>


bool start = false;
float minutes{0};
float seconds{0};
float totaltime{0};
enum
{
    tutorial,
    level1,
    level2
} currentlvl;



TimeTaken::TimeTaken(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}

void TimeTaken::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "TimeTaken Script Ready : " << obj->GetName() << std::endl;
    minutes = 0 ;
    seconds = 0 ;
    totaltime = 0 ;
}

void TimeTaken::Update(Object* obj) {
    if (obj == nullptr)
    {
        return;
    }
    if (obj->GetName() == "win_time_taken")
    {

        if (start)
        {
            totaltime += engine->GetDt();
        }
        else
        {
            Text* t = (Text*)obj->GetComponent(ComponentType::Text);
            //t->text = std::to_string(static_cast<int>(totaltime));
            char time_taken_cstr[100];
            sprintf_s(time_taken_cstr, "Time Taken: %02d:%02d", static_cast<int>(totaltime) / 60, static_cast<int>(totaltime) % 60);
            t->text = time_taken_cstr;
        }
    }
    else if (obj->GetName() == "win_level_number")
    {
        Texture* tex = (Texture*)obj->GetComponent(ComponentType::Texture);

        if (engine->loaded_level == "tutorial_level")
        {
            tex->textureName = "0.png";
        }
        else if (engine->loaded_level == "level_1")
        {
            tex->textureName = "1.png";
        }
        else if (engine->loaded_level == "level_2")
        {
            tex->textureName = "2.png";
        }
    }


}

void TimeTaken::Shutdown(Object* obj) {
    std::cout << "TimeTaken Script Shutdown : " << obj->GetName() << std::endl;
}


void WINMENU::StartTime()
{
    start = true;
    totaltime = 0;
}

void WINMENU::StopTime()
{
    start = false;

}


// Creating an instance of ButtonBase.
TimeTaken timetaken("TimeTaken");


