
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


// Constructor for the ButtonBase class.
// @param name: The name of the portal.
TimeTaken::TimeTaken(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the ButtonBase script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void TimeTaken::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "TimeTaken Script Ready : " << obj->GetName() << std::endl;
    minutes = 0 ;
    seconds = 0 ;
    totaltime = 0 ;
}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
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
/*********************************************************************/
// Shutdown method called when the ButtonBase script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
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


