/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <dali/dali.h>
#include <dali-toolkit/dali-toolkit.h>

using namespace Dali;
using namespace Toolkit;

namespace
{

/*
 * Vertex shader
 */
const char* VERTEX_SHADER = DALI_COMPOSE_SHADER(
attribute mediump vec2 aPosition;\n // DALi shader builtin
uniform   mediump mat4 uMvpMatrix;\n // DALi shader builtin
uniform   mediump vec3 uSize;\n // DALi shader builtin
\n
void main()\n
{\n
  mediump vec4 vertexPosition = vec4(aPosition, 0.0, 1.0);\n
  vertexPosition.xyz *= uSize;\n
  gl_Position = uMvpMatrix * vertexPosition;\n
}\n
);

/*
 * Fragment shader
 */
const char* FRAGMENT_SHADER = DALI_COMPOSE_SHADER(
uniform mediump vec4 uColor;\n
\n
void main()\n
{\n
  gl_FragColor = uColor;\n
}\n
);

}

// This example shows how to draw a triangle in actor's color
//
class DrawTriangleController : public ConnectionTracker
{
public:

  DrawTriangleController( Application& application )
  : mApplication( application )
  {
    // Connect to the Application's Init signal
    mApplication.InitSignal().Connect( this, &DrawTriangleController::Create );
  }

  ~DrawTriangleController()
  {
    // Nothing to do here;
  }

  // The Init signal is received once (only) during the Application lifetime
  void Create( Application& application )
  {
    // Get a handle to the stage
    Stage stage = Stage::GetCurrent();
    stage.SetBackgroundColor( Color::WHITE );

    // Step 1. Create shader
    CreateTriangleShader();

    // Step 2. Prepare geometry
    CreateTriangleGeometry();

    // Step 3. Create a renderer
    CreateRenderer();

    // Step 4. Create an Actor
    CreateActor();

    // Respond to a click anywhere on the stage
    stage.GetRootLayer().TouchSignal().Connect( this, &DrawTriangleController::OnTouch );
  }

  bool OnTouch( Actor actor, const TouchData& touch )
  {
    // quit the application
    mApplication.Quit();
    return true;
  }

  /**
   * This function creates a triangle geometry made of three vertices in order
   * to draw a coloured triangle.
   */
  void CreateTriangleGeometry()
  {
    Vector2 vertices[] = {
      Vector2( -1.0f, -1.0f ),
      Vector2(  1.0f,  1.0f ),
      Vector2( -1.0f,  1.0f )
    };

    Property::Map map;
    map[ "aPosition" ] = Property::VECTOR2;
    PropertyBuffer vertexBuffer = PropertyBuffer::New( map );

    vertexBuffer.SetData( vertices, sizeof(vertices) / sizeof(Vector2) );

    mGeometry = Geometry::New();
    mGeometry.AddVertexBuffer( vertexBuffer );
    mGeometry.SetType( Geometry::TRIANGLES );
  }

  /**
   * Creates a shader using inlined variable VERTEX_SHADER and FRAGMENT_SHADER
   *
   * Shaders are very basic and all they do is transforming vertices and applying actor's colour.
   */
  void CreateTriangleShader()
  {
    mShader = Shader::New( VERTEX_SHADER, FRAGMENT_SHADER );
  }

  /**
   * Function creates renderer.
   */
  void CreateRenderer()
  {
    mRenderer = Renderer::New( mGeometry, mShader );
  }

  /**
   * Creates new actor and attaches renderer.
   */
  void CreateActor()
  {
    Stage stage = Stage::GetCurrent();
    Size size = stage.GetSize() * 0.25f;
    mActor = Actor::New();
    mActor.SetAnchorPoint( AnchorPoint::CENTER );
    mActor.SetParentOrigin( ParentOrigin::CENTER );
    mActor.SetPosition( Vector3( 0.0f, 0.0f, 0.0f ) );
    mActor.SetColor( Color::RED );
    mActor.SetSize( Vector3( size.x, size.x, size.x ) );
    mActor.AddRenderer( mRenderer );
    stage.Add( mActor );
  }

private:
  Application&  mApplication;

  Renderer mRenderer;
  Shader mShader;
  Geometry mGeometry;
  Actor mActor;
};

void RunTest( Application& application )
{
  DrawTriangleController test( application );

  application.MainLoop();
}

// Entry point for Linux & Tizen applications
//
int DALI_EXPORT_API main( int argc, char **argv )
{
  Application application = Application::New( &argc, &argv );

  RunTest( application );

  return 0;
}
