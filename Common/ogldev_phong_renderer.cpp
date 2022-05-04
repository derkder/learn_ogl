/*

        Copyright 2011 Etay Meiri

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

#include "ogldev_engine_common.h"
#include "ogldev_phong_renderer.h"


PhongRenderer::PhongRenderer()
{

}


PhongRenderer::~PhongRenderer()
{

}


void PhongRenderer::InitPhongRenderer()
{
    if (!m_lightingTech.Init()) {
        printf("Error initializing the lighting technique\n");
        exit(1);
    }

    m_lightingTech.Enable();
    m_lightingTech.SetTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    m_lightingTech.SetSpecularExponentTextureUnit(SPECULAR_EXPONENT_UNIT_INDEX);
    glUseProgram(0);
}


void PhongRenderer::Activate()
{
    if (m_isActive) {
        printf("Warning! Trying to activate the Phong Renderer but it is already active\n");
    }

    m_lightingTech.Enable();

    m_isActive = true;
}


void PhongRenderer::Deactivate()
{
    if (!m_isActive) {
        printf("Warning! Trying to de-activate the Phong Renderer but it is not active\n");
        return;
    }

    GLint cur_prog = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur_prog);

    if (cur_prog != m_lightingTech.GetProgram()) {
        printf("Trying to de-activate phong renderer (program %d) but another program is active (%d)\n", m_lightingTech.GetProgram(), cur_prog);
        exit(0);
    }

    glUseProgram(0);

    m_isActive = false;
}


void PhongRenderer::SetDirLight(const DirectionalLight& DirLight)
{
    if (!m_isActive) {
        printf("Trying to set the directional light but the program is not active\n");
        exit(0);
    }

    m_dirLight = DirLight;

    m_lightingTech.SetDirectionalLight(m_dirLight);
}


void PhongRenderer::SetPointLights(uint NumLights, const PointLight* pPointLights)
{
    if (!m_isActive) {
        printf("Trying to set point lights but the program is not active\n");
        exit(0);
    }

    if (!pPointLights || (NumLights == 0)) {
        m_numPointLights = 0;
        return;
    }

    if (NumLights > LightingTechnique::MAX_POINT_LIGHTS) {
        printf("Number of point lights (%d) exceeds max (%d)\n", NumLights, LightingTechnique::MAX_POINT_LIGHTS);
        exit(0);
    }

    for (uint i = 0 ; i < NumLights ; i++) {
        m_pointLights[i] = pPointLights[i];
    }

    m_numPointLights = NumLights;
}


void PhongRenderer::SetSpotLights(uint NumLights, const SpotLight* pSpotLights)
{
    if (!m_isActive) {
        printf("Trying to set spot lights but the program is not active\n");
        exit(0);
    }

    if (!pSpotLights || (NumLights == 0)) {
        m_numSpotLights = 0;
        return;
    }

    if (NumLights > LightingTechnique::MAX_SPOT_LIGHTS) {
        printf("Number of point lights (%d) exceeds max (%d)\n", NumLights, LightingTechnique::MAX_SPOT_LIGHTS);
        exit(0);
    }

    for (uint i = 0 ; i < NumLights ; i++) {
        m_spotLights[i] = pSpotLights[i];
    }

    m_numSpotLights = NumLights;
}


void PhongRenderer::Render(BasicMesh* pMesh)
{
    if (!m_pCamera) {
        printf("PhongRenderer: camera not initialized\n");
        exit(0);
    }

    if (!m_isActive) {
        printf("Must activate the Phong Renderer before calling render\n");
        exit(0);
    }

    if ((m_numPointLights == 0) && (m_numSpotLights == 0) && m_dirLight.IsZero()) {
        printf("Warning! trying to render but all lights are zero\n");
    }

    SetWVP(pMesh);

    WorldTrans& meshWorldTransform = pMesh->GetWorldTransform();

    for (uint i = 0 ; i < m_numPointLights ; i++) {
        m_pointLights[i].CalcLocalPosition(meshWorldTransform);
    }

    m_lightingTech.SetPointLights(m_numPointLights, m_pointLights);

    for (uint i = 0 ; i < m_numSpotLights ; i++) {
        m_spotLights[i].CalcLocalDirectionAndPosition(meshWorldTransform);
    }

    m_lightingTech.SetSpotLights(m_numSpotLights, m_spotLights);

    m_lightingTech.SetMaterial(pMesh->GetMaterial());

    Vector3f CameraLocalPos3f = meshWorldTransform.WorldPosToLocalPos(m_pCamera->GetPos());
    m_lightingTech.SetCameraLocalPos(CameraLocalPos3f);

    pMesh->Render();
}




void PhongRenderer::SetWVP(BasicMesh* pMesh)
{
    WorldTrans& meshWorldTransform = pMesh->GetWorldTransform();

    Matrix4f World = meshWorldTransform.GetMatrix();
    Matrix4f View = m_pCamera->GetMatrix();
    Matrix4f Projection = m_pCamera->GetProjectionMat();

    Matrix4f WVP = Projection * View * World;

    m_lightingTech.SetWVP(WVP);
}
