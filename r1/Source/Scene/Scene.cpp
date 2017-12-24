/**
 *	@file	Scene.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "Scene.h"

Scene::Scene(SceneManager *argSceneManager)
	: sceneManager_(argSceneManager)
{
}

Scene::~Scene() = default;

