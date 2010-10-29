/**************************************************************************************/
/*                                                                                    */
/*  Visualization Library                                                             */
/*  http://www.visualizationlibrary.com                                               */
/*                                                                                    */
/*  Copyright (c) 2005-2010, Michele Bosi                                             */
/*  All rights reserved.                                                              */
/*                                                                                    */
/*  Redistribution and use in source and binary forms, with or without modification,  */
/*  are permitted provided that the following conditions are met:                     */
/*                                                                                    */
/*  - Redistributions of source code must retain the above copyright notice, this     */
/*  list of conditions and the following disclaimer.                                  */
/*                                                                                    */
/*  - Redistributions in binary form must reproduce the above copyright notice, this  */
/*  list of conditions and the following disclaimer in the documentation and/or       */
/*  other materials provided with the distribution.                                   */
/*                                                                                    */
/*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND   */
/*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED     */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE            */
/*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR  */
/*  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    */
/*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      */
/*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON    */
/*  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           */
/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS     */
/*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                      */
/*                                                                                    */
/**************************************************************************************/

#ifndef App_ShaderOverride_INCLUDE_ONCE
#define App_ShaderOverride_INCLUDE_ONCE

#include "BaseDemo.hpp"
#include "vlut/GeometryPrimitives.hpp"
#include "vl/SceneManagerActorTree.hpp"
#include "vl/Actor.hpp"
#include "vl/Effect.hpp"
#include "vl/Time.hpp"
#include "vl/Light.hpp"
#include "vl/RenderingTree.hpp"

class App_ShaderOverride: public BaseDemo
{
  void initEvent()
  {
    BaseDemo::initEvent();

    mRendering = (vl::VisualizationLibrary::rendering()->as<vl::Rendering>());

    mCubeTransform1 = new vl::Transform;
    mCubeTransform2 = new vl::Transform;
    mCubeTransform3 = new vl::Transform;
    mRendering->transform()->addChild( mCubeTransform1.get() );
    mRendering->transform()->addChild( mCubeTransform2.get() );
    mRendering->transform()->addChild( mCubeTransform3.get() );

    const vl::Real fsize = 9;
    vl::ref<vl::Geometry> ball = vlut::makeUVSphere( vl::vec3(0,0,0), fsize, 10, 10 );
    ball->computeNormals();

    vl::ref<vl::Effect> effect = new vl::Effect;
    effect->shader()->enable(vl::EN_BLEND);
    effect->shader()->enable(vl::EN_DEPTH_TEST);
    effect->shader()->setRenderState( new vl::Light(0) );
    effect->shader()->enable(vl::EN_LIGHTING);
    effect->shader()->enable(vl::EN_CULL_FACE);
    effect->shader()->gocMaterial()->setDiffuse( vlut::gold );
    effect->shader()->gocMaterial()->setTransparency( 0.5f );

    vl::Actor* cube1 = sceneManager()->tree()->addActor( ball.get(), effect.get(), mCubeTransform1.get() );
    vl::Actor* cube2 = sceneManager()->tree()->addActor( ball.get(), effect.get(), mCubeTransform2.get() );
    vl::Actor* cube3 = sceneManager()->tree()->addActor( ball.get(), effect.get(), mCubeTransform3.get() );

    cube1->setEnableMask(0x01);
    cube2->setEnableMask(0x02);
    cube3->setEnableMask(0x04);

    vl::ref<vl::Shader> sh1 = new vl::Shader;
    sh1->enable(vl::EN_LIGHTING);
    sh1->gocMaterial()->setFlatColor(vlut::red);
    sh1->gocPolygonMode()->set(vl::PM_LINE,vl::PM_LINE);

    vl::ref<vl::Shader> sh2 = new vl::Shader;
    sh2->enable(vl::EN_LIGHTING);
    sh2->gocMaterial()->setFlatColor(vlut::green);
    sh2->gocPolygonMode()->set(vl::PM_LINE,vl::PM_LINE);

    vl::ref<vl::Shader> sh3 = new vl::Shader;
    sh3->enable(vl::EN_LIGHTING);
    sh3->gocMaterial()->setFlatColor(vlut::blue);
    sh3->gocPolygonMode()->set(vl::PM_LINE,vl::PM_LINE);

    vl::ref<vl::Renderer> wire_renderer = new vl::Renderer;
    wire_renderer->setClearFlags(vl::CF_DO_NOT_CLEAR);
    mRendering->renderers().push_back(wire_renderer.get());
    wire_renderer->shaderOverrideMask()[0x01] = sh1;
    wire_renderer->shaderOverrideMask()[0x02] = sh2;
    wire_renderer->shaderOverrideMask()[0x04] = sh3;
  }

  virtual void run()
  {
    vl::Real degrees = vl::Time::currentTime() * 45.0f;
    vl::mat4 matrix;
    
    matrix.rotate( degrees, 1,0,0 );
    matrix.translate(-10,0,0);
    mCubeTransform1->setLocalMatrix( matrix );

    matrix.setIdentity();
    matrix.rotate( degrees, 0,1,0 );
    matrix.translate(0,0,0);
    mCubeTransform2->setLocalMatrix( matrix );

    matrix.setIdentity();
    matrix.rotate( degrees, 0,0,1 );
    matrix.translate(+10,0,0);
    mCubeTransform3->setLocalMatrix( matrix );
  }

  void resizeEvent(int /*w*/, int /*h*/)
  {
    mRendering->camera()->viewport()->setWidth(mRendering->renderTarget()->width());
    mRendering->camera()->viewport()->setHeight(mRendering->renderTarget()->height());
    mRendering->camera()->setProjectionAsPerspective();
  }

protected:
  vl::ref<vl::Rendering> mRendering;
  vl::ref<vl::Transform> mCubeTransform1;
  vl::ref<vl::Transform> mCubeTransform2;
  vl::ref<vl::Transform> mCubeTransform3;
};

// Have fun!

#endif
/**************************************************************************************/
/*                                                                                    */
/*  Visualization Library                                                             */
/*  http://www.visualizationlibrary.com                                               */
/*                                                                                    */
/*  Copyright (c) 2005-2010, Michele Bosi                                             */
/*  All rights reserved.                                                              */
/*                                                                                    */
/*  Redistribution and use in source and binary forms, with or without modification,  */
/*  are permitted provided that the following conditions are met:                     */
/*                                                                                    */
/*  - Redistributions of source code must retain the above copyright notice, this     */
/*  list of conditions and the following disclaimer.                                  */
/*                                                                                    */
/*  - Redistributions in binary form must reproduce the above copyright notice, this  */
/*  list of conditions and the following disclaimer in the documentation and/or       */
/*  other materials provided with the distribution.                                   */
/*                                                                                    */
/*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND   */
/*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED     */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE            */
/*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR  */
/*  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    */
/*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      */
/*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON    */
/*  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           */
/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS     */
/*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                      */
/*                                                                                    */
/**************************************************************************************/

#ifndef App_ShaderOverride_INCLUDE_ONCE
#define App_ShaderOverride_INCLUDE_ONCE

#include "BaseDemo.hpp"
#include "vlut/GeometryPrimitives.hpp"
#include "vl/SceneManagerActorTree.hpp"
#include "vl/Actor.hpp"
#include "vl/Effect.hpp"
#include "vl/Time.hpp"
#include "vl/Light.hpp"
#include "vl/RenderingTree.hpp"

class App_ShaderOverride: public BaseDemo
{
  void initEvent()
  {
    BaseDemo::initEvent();

    mRendering = vl::VisualizationLibrary::rendering()->as<vl::Rendering>();

    mCubeTransform1 = new vl::Transform;
    mCubeTransform2 = new vl::Transform;
    mCubeTransform3 = new vl::Transform;
    mRendering->transform()->addChild( mCubeTransform1.get() );
    mRendering->transform()->addChild( mCubeTransform2.get() );
    mRendering->transform()->addChild( mCubeTransform3.get() );

    const vl::Real fsize = 9;
    vl::ref<vl::Geometry> ball = vlut::makeUVSphere( vl::vec3(0,0,0), fsize, 10, 10 );
    ball->computeNormals();

    vl::ref<vl::Effect> effect = new vl::Effect;
    effect->shader()->enable(vl::EN_BLEND);
    effect->shader()->enable(vl::EN_DEPTH_TEST);
    effect->shader()->setRenderState( new vl::Light(0) );
    effect->shader()->enable(vl::EN_LIGHTING);
    effect->shader()->enable(vl::EN_CULL_FACE);
    effect->shader()->gocMaterial()->setDiffuse( vlut::gold );
    effect->shader()->gocMaterial()->setTransparency( 0.5f );

    // creates 3 actors and adds them to the scene
    vl::Actor* cube1 = sceneManager()->tree()->addActor( ball.get(), effect.get(), mCubeTransform1.get() );
    vl::Actor* cube2 = sceneManager()->tree()->addActor( ball.get(), effect.get(), mCubeTransform2.get() );
    vl::Actor* cube3 = sceneManager()->tree()->addActor( ball.get(), effect.get(), mCubeTransform3.get() );

    vl::ref<vl::Shader> sh1 = new vl::Shader;
    vl::ref<vl::Shader> sh2 = new vl::Shader;
    vl::ref<vl::Shader> sh3 = new vl::Shader;

    sh1->enable(vl::EN_LIGHTING);
    sh1->enable(vl::EN_CULL_FACE);
    sh1->gocPolygonMode()->set(vl::PM_LINE,vl::PM_LINE);
    sh1->gocCullFace()->set(vl::PF_BACK);

    // share all the states except for the glMaterial ones
    *sh3 = *sh2 = *sh1;
    sh1->gocMaterial()->setFlatColor(vlut::red);
    sh2->gocMaterial()->setFlatColor(vlut::green);
    sh3->gocMaterial()->setFlatColor(vlut::blue);

    vl::ref<vl::Renderer> wire_renderer = new vl::Renderer;
    wire_renderer->setClearFlags(vl::CF_DO_NOT_CLEAR);
    mRendering->renderers().push_back(wire_renderer.get());

    wire_renderer->shaderOverrideMask()[0x01] = sh1;
    wire_renderer->shaderOverrideMask()[0x02] = sh2;
    wire_renderer->shaderOverrideMask()[0x04] = sh3;

    cube1->setEnableMask(0x01);
    cube2->setEnableMask(0x02);
    cube3->setEnableMask(0x04);

  }

  virtual void run()
  {
    vl::Real degrees = vl::Time::currentTime() * 45.0f;
    vl::mat4 matrix;
    
    matrix.rotate( degrees, 1,0,0 );
    matrix.translate(-10,0,0);
    mCubeTransform1->setLocalMatrix( matrix );

    matrix.setIdentity();
    matrix.rotate( degrees, 0,1,0 );
    matrix.translate(0,0,0);
    mCubeTransform2->setLocalMatrix( matrix );

    matrix.setIdentity();
    matrix.rotate( degrees, 0,0,1 );
    matrix.translate(+10,0,0);
    mCubeTransform3->setLocalMatrix( matrix );
  }

  void resizeEvent(int /*w*/, int /*h*/)
  {
    mRendering->camera()->viewport()->setWidth ( mRendering->renderTarget()->width () );
    mRendering->camera()->viewport()->setHeight( mRendering->renderTarget()->height() );
    mRendering->camera()->setProjectionAsPerspective();
  }

protected:
  vl::ref<vl::Rendering> mRendering;
  vl::ref<vl::Transform> mCubeTransform1;
  vl::ref<vl::Transform> mCubeTransform2;
  vl::ref<vl::Transform> mCubeTransform3;
};

// Have fun!

#endif
