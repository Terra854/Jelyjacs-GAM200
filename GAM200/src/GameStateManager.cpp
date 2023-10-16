#include "GameStateManager.h"
#include "GameStateList.h"
#include "TestStage.h"

int current = 0, previous = 0, next = 0;

FP fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;


/*!***********************************************************************
\brief
	Initialize the Game State Manager with a starting state.
	Sets the previous, current and next state to the initial value.

\param[in] startingState
	Starting state to initialize with

\return None
*************************************************************************/
void GSM_Initialize(int startingState)
{
	// Set previous, current and next state to the initial value
	current = previous = next = startingState;
}


/*!***********************************************************************
\brief Update the state function pointers based on currently selected state.

\param[in] None

\return None
*************************************************************************/
void GSM_Update()
{
	// Check current state and update the function pointers
	switch (current)
	{
	case GS_TEST:
		fpLoad = TestStage_Load;
		fpInitialize = TestStage_Initialize;
		fpUpdate = TestStage_Update;
		fpDraw = TestStage_Draw;
		fpFree = TestStage_Free;
		fpUnload = TestStage_Unload;
		/*
	case GS_MAIN_MENU:
		fpLoad = Main_Menu_Load;
		fpInitialize = Main_Menu_Initialize;
		fpUpdate = Main_Menu_Update;
		fpDraw = Main_Menu_Draw;
		fpFree = Main_Menu_Free;
		fpUnload = Main_Menu_Unload;
		break;
	case GS_LEVEL1:
		fpLoad = Level1_Load;
		fpInitialize = Level1_Initialize;
		fpUpdate = Level1_Update;
		fpDraw = Level1_Draw;
		fpFree = Level1_Free;
		fpUnload = Level1_Unload;
		break;

	case GS_LEVEL2:
		fpLoad = Level2_Load;
		fpInitialize = Level2_Initialize;
		fpUpdate = Level2_Update;
		fpDraw = Level2_Draw;
		fpFree = Level2_Free;
		fpUnload = Level2_Unload;
		break;


	case GS_LEVEL3:
		fpLoad = Level3Load;
		fpInitialize = Level3Initialize;
		fpUpdate = Level3_Update;
		fpDraw = Level3_Draw;
		fpFree = Level3_Free;
		fpUnload = Level3_Unload;
		break;
		*/
	case GS_RESTART:
		break;
	case GS_QUIT:
		break;
	default:
		break;
	}

}