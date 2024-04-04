
#include "Scripts/TimeTaken.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <../components/Text.h>


bool start = false;
float minutes{0};
float seconds{0};
float totaltime{0};


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
    if (start)
    {
        totaltime += engine->GetDt();
    }
    else
    {
        Text* t = (Text*)obj->GetComponent(ComponentType::Text);
        t->text = std::to_string(static_cast<int>(totaltime));
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


