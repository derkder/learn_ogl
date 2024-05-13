/*

        Copyright 2022 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#pragma once

#include <list>

#include "demolition_scene.h"
#include "Int\core_model.h"


class CoreSceneObject : public SceneObject {
public:
    CoreSceneObject() {}

    void SetModel(CoreModel* pModel)
    {
        if (m_pModel) {
            printf("%s:%d - model already initialized\n", __FILE__, __LINE__);
            exit(0);
        }

        m_pModel = pModel;
    }

    CoreModel* GetModel() const { return m_pModel; }

private:
    CoreModel* m_pModel = NULL;
};


class CoreRenderingSystem;

class CoreScene : public Scene {
public:
    CoreScene(CoreRenderingSystem* pRenderingSystem);

    virtual ~CoreScene() {}

    virtual void LoadScene(const std::string& Filename);

    virtual SceneObject* CreateSceneObject(Model* pModel);

    virtual SceneObject* CreateSceneObject(const std::string& BasicShape);

    const std::vector<PointLight>& GetPointLights();

    const std::vector<SpotLight>& GetSpotLights();

    const std::vector<DirectionalLight>& GetDirLights();

    BasicCamera* GetCurrentCamera() { return &m_defaultCamera; }

    void InitializeDefault();

    const std::list<CoreSceneObject*>& GetRenderList() { return m_renderList; }
    
    void AddToRenderList(SceneObject* pSceneObject);

    bool RemoveFromRenderList(SceneObject* pSceneObject);

    bool IsClearFrame() const { return m_clearFrame; }

    const Vector4f& GetClearColor() { return m_clearColor; }

    void SetCamera(const Vector3f& Pos, const Vector3f& Target);

protected:
    CoreRenderingSystem* m_pCoreRenderingSystem = NULL;
    std::list<CoreSceneObject*> m_renderList;

private:
    void CreateDefaultCamera();
    CoreSceneObject* CreateSceneObjectInternal(CoreModel* pModel);

    BasicCamera m_defaultCamera;
    std::vector<CoreSceneObject> m_sceneObjects;
    int m_numSceneObjects = 0;
};
