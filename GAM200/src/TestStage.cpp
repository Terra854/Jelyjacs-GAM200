#include <Object.h>
#include <Factory.h>
#include <iostream>
#include <Collision.h>
#include <components/Physics.h>
#include <input.h>


Object* scale_and_rotate;
Object* dynamic_collision;

void TestStage_Load() {

}

void TestStage_Initialize() {
	Object* testObj;
	Object* floor1;
	Object* floor2;
	Object* floor3;
	//Transform * trans;
	//Texture* texture;

	std::cout << "Background" << std::endl;
	testObj = objectFactory->createObject("../Asset/Objects/background.json");
	scale_and_rotate = objectFactory->createObject("../Asset/Objects/scale-and-rotate.json");
	//std::cout << "test Object 2" << std::endl;
	//testObj2 = objectFactory->createObject("../drop-forever.json");
	floor1 = objectFactory->createObject("../Asset/Objects/mapbox.json");
	floor2 = objectFactory->createObject("../Asset/Objects/mapbox.json");
	floor3 = objectFactory->createObject("../Asset/Objects/mapbox.json");
	Transform* tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(2))->GetComponent(ComponentType::Transform));

	// offset objects
	tran_pt->Position.x = 190;

	// @Sen Chuan if you move position like that, remember to call these 2 functions cause collision
	// box still using the old position
	Rectangular* rect_pt = static_cast<Rectangular*>((objectFactory->getObjectWithID(2))->GetComponent(ComponentType::Body)); // @Sen Chuan
	rect_pt->Initialize(); // @Sen Chuan

	tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(3))->GetComponent(ComponentType::Transform)); // change to different obj
	tran_pt->Position.x = 380;
	tran_pt->Position.y = -300;

	rect_pt = static_cast<Rectangular*>((objectFactory->getObjectWithID(3))->GetComponent(ComponentType::Body));
	rect_pt->Initialize();

	// Floor
	for (int i = 0; i < 44; i++) {
		Object* floor = objectFactory->createObject("../Asset/Objects/mapbox.json");
		Transform* floor_t = static_cast<Transform*>(floor->GetComponent(ComponentType::Transform));
		floor_t->Position = { -1100.0f + (static_cast<Transform*>(floor->GetComponent(ComponentType::Transform))->Scale_x * (float)i), -500.0f };
		Rectangular* floor_b = static_cast<Rectangular*>(floor->GetComponent(ComponentType::Body)); // @Sen Chuan
		floor_b->Initialize();
	}

	// Dynamic collision
	dynamic_collision = objectFactory->createObject("../Asset/Objects/mapbox.json");
	Transform* dynamic_collision_t = static_cast<Transform*>(dynamic_collision->GetComponent(ComponentType::Transform));
	dynamic_collision_t->Position = { -1000.0f , -446.0f };
	Rectangular* dynamic_collision_b = static_cast<Rectangular*>(dynamic_collision->GetComponent(ComponentType::Body));
	dynamic_collision_b->Initialize();
	Physics* p = new Physics();
	dynamic_collision->AddComponent(p);

	//std::cout << "test bottom_line" << std::endl;
	//bottom_line = objectFactory->createObject("../bottom_line.json");
	//trans = static_cast<Transform*>( testObj->GetComponent(ComponentType::Transform));

	//alternate way to get component without cast
	//trans = testObj->GetComponent_NoCast<Transform>(ComponentTypeId::CT_Transform);
	//trans->Mass = 0.5f;
	//Getting a Object pointer with the game object ID
	//testObj = objectFactory->getObjectWithID(0);
	//texture = static_cast<Texture*>(testObj->GetComponent(ComponentType::Texture));
}
void TestStage_Update() {
	// Rotation of an object
	Transform* t2 = static_cast<Transform*>(scale_and_rotate->GetComponent(ComponentType::Transform));

	if (input::IsPressedRepeatedly(KEY::up)) {
		t2->Scale_x += 1.0f;
		t2->Scale_y += 1.0f;
	}
	if (input::IsPressedRepeatedly(KEY::down)) {
		t2->Scale_x = t2->Scale_x >= 1.0f ? t2->Scale_x - 1.0f : 0.f;
		t2->Scale_y = t2->Scale_y >= 1.0f ? t2->Scale_y - 1.0f : 0.f;
	}
	if (input::IsPressedRepeatedly(KEY::left)) {
		t2->Rotation += 0.01f;
	}
	if (input::IsPressedRepeatedly(KEY::right)) {
		t2->Rotation -= 0.01f;
	}

	// Dynamic collision
	Physics* dynamic_collision_p = static_cast<Physics*>(dynamic_collision->GetComponent(ComponentType::Physics));
	dynamic_collision_p->Velocity.x = 200.0f;
	Transform* dynamic_collision_t = static_cast<Transform*>(dynamic_collision->GetComponent(ComponentType::Transform));
	dynamic_collision_t->Position.x = dynamic_collision_t->Position.x < 1000.0f ? dynamic_collision_t->Position.x : -1000.0f;

	/*
	// DEBUG: Print out collision flags
	int c_flag = static_cast<Rectangular*>(playerObj->GetComponent(ComponentType::Body))->collision_flag;
	std::cout << "FLAG: " << c_flag <<
		" LEFT: " << ((c_flag & COLLISION_LEFT) ? "YES" : "NO") <<
		" RIGHT: " << ((c_flag & COLLISION_RIGHT) ? "YES" : "NO") <<
		" TOP: " << ((c_flag & COLLISION_TOP) ? "YES" : "NO") <<
		" BOTTOM: " << ((c_flag & COLLISION_BOTTOM) ? "YES" : "NO") << std::endl;
	std::cout << "#####################################################################" << std::endl;
	*/
}

void TestStage_Draw() {

}

void TestStage_Free() {

}

void TestStage_Unload() {

}