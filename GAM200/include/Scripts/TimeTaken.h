#pragma once
/* !
@author Yeo Jia Ming
@date	1/3/2024

This file contains the script for the win screen time taken
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>


class TimeTaken : public LogicScript
{
public:
 
    TimeTaken(std::string name);

  
    ~TimeTaken() = default;


    void Start(Object* obj);

 
    void Update(Object* obj);

  
    void Shutdown(Object* obj);
};

namespace WINMENU
{
    void StartTime();
    void StopTime();

}
