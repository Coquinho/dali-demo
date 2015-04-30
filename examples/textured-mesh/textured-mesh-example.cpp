/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

// EXTERNAL INCLUDES

// INTERNAL INCLUDES
#include "shared/view.h"

#include <dali-toolkit/dali-toolkit.h>

using namespace Dali;

namespace
{
const char* MATERIAL_SAMPLE( DALI_IMAGE_DIR "gallery-small-48.jpg" );
const char* MATERIAL_SAMPLE2( DALI_IMAGE_DIR "gallery-medium-19.jpg" );

#define MAKE_SHADER(A)#A

const char* VERTEX_SHADER = MAKE_SHADER(
attribute mediump vec2    aPosition;
attribute highp   vec2    aTexCoord;
varying   mediump vec2    vTexCoord;
uniform   mediump mat4    uMvpMatrix;
uniform   mediump vec3    uSize;
uniform   lowp    vec4    uFadeColor;

void main()
{
  mediump vec4 vertexPosition = vec4(aPosition, 0.0, 1.0);
  vertexPosition.xyz *= uSize;
  vertexPosition = uMvpMatrix * vertexPosition;
  vTexCoord = aTexCoord;
  gl_Position = vertexPosition;
}
);

const char* FRAGMENT_SHADER = MAKE_SHADER(
varying mediump vec2  vTexCoord;
uniform lowp    vec4  uColor;
uniform sampler2D     sTexture;
uniform lowp    vec4  uFadeColor;

void main()
{
  gl_FragColor = texture2D( sTexture, vTexCoord ) * uColor * uFadeColor;
}
);

Geometry CreateGeometry()
{
  // Create vertices
  const float halfQuadSize = .5f;
  struct TexturedQuadVertex { Vector2 position; Vector2 textureCoordinates; };
  TexturedQuadVertex texturedQuadVertexData[4] = {
    { Vector2(-halfQuadSize, -halfQuadSize), Vector2(0.f, 0.f) },
    { Vector2( halfQuadSize, -halfQuadSize), Vector2(1.f, 0.f) },
    { Vector2(-halfQuadSize,  halfQuadSize), Vector2(0.f, 1.f) },
    { Vector2( halfQuadSize,  halfQuadSize), Vector2(1.f, 1.f) } };

  Property::Map texturedQuadVertexFormat;
  texturedQuadVertexFormat["aPosition"] = Property::VECTOR2;
  texturedQuadVertexFormat["aTexCoord"] = Property::VECTOR2;
  PropertyBuffer texturedQuadVertices = PropertyBuffer::New( PropertyBuffer::STATIC, texturedQuadVertexFormat, 4 );
  texturedQuadVertices.SetData(texturedQuadVertexData);

  // Create indices
  unsigned int indexData[6] = { 0, 3, 1, 0, 2, 3 };
  Property::Map indexFormat;
  indexFormat["indices"] = Property::UNSIGNED_INTEGER;
  PropertyBuffer indices = PropertyBuffer::New( PropertyBuffer::STATIC, indexFormat, sizeof(indexData)/sizeof(indexData[0]) );
  indices.SetData(indexData);

  // Create the geometry object
  Geometry texturedQuadGeometry = Geometry::New();
  texturedQuadGeometry.AddVertexBuffer( texturedQuadVertices );
  texturedQuadGeometry.SetIndexBuffer( indices );

  return texturedQuadGeometry;
}

/**
 * Sinusoidal curve starting at zero with 2 cycles
 */
float AlphaFunctionSineX2(float progress)
{
  return 0.5f - cosf(progress * 4.0f * Math::PI) * 0.5f;
}

} // anonymous namespace

// This example shows how to use a simple mesh
//
class ExampleController : public ConnectionTracker
{
public:

  /**
   * The example controller constructor.
   * @param[in] application The application instance
   */
  ExampleController( Application& application )
  : mApplication( application )
  {
    // Connect to the Application's Init signal
    mApplication.InitSignal().Connect( this, &ExampleController::Create );
  }

  /**
   * The example controller destructor
   */
  ~ExampleController()
  {
    // Nothing to do here;
  }

  /**
   * Invoked upon creation of application
   * @param[in] application The application instance
   */
  void Create( Application& application )
  {
    // The Init signal is received once (only) during the Application lifetime

    Stage stage = Stage::GetCurrent();
    stage.KeyEventSignal().Connect(this, &ExampleController::OnKeyEvent);

    mStageSize = stage.GetSize();

    // Hide the indicator bar
    application.GetWindow().ShowIndicator( Dali::Window::INVISIBLE );

    mImage = ResourceImage::New( MATERIAL_SAMPLE, ResourceImage::ON_DEMAND, Image::NEVER );
    mSampler1 = Sampler::New(mImage, "sTexture");

    Image image = ResourceImage::New( MATERIAL_SAMPLE2 );
    mSampler2 = Sampler::New(image, "sTexture");

    mShader = Shader::New( VERTEX_SHADER, FRAGMENT_SHADER );
    mMaterial1 = Material::New( mShader );
    mMaterial1.AddSampler( mSampler1 );

    mMaterial2 = Material::New( mShader );
    mMaterial2.AddSampler( mSampler2 );

    mGeometry = CreateGeometry();

    mRenderer = Renderer::New( mGeometry, mMaterial1 );

    mMeshActor = Actor::New();
    mMeshActor.AddRenderer( mRenderer );
    mMeshActor.SetSize(400, 400);

    Property::Index fadeColorIndex = mMeshActor.RegisterProperty( "fade-color", Color::GREEN );
    mMeshActor.AddUniformMapping( fadeColorIndex, std::string("uFadeColor") );

    fadeColorIndex = mRenderer.RegisterProperty( "fade-color", Color::MAGENTA );
    mRenderer.AddUniformMapping( fadeColorIndex, std::string("uFadeColor" ) );
    mRenderer.SetDepthIndex(0);

    mMeshActor.SetParentOrigin( ParentOrigin::TOP_CENTER );
    mMeshActor.SetAnchorPoint( AnchorPoint::TOP_CENTER );
    stage.Add( mMeshActor );

    mRenderer2 = Renderer::New( mGeometry, mMaterial2 );

    mMeshActor2 = Actor::New();
    mMeshActor2.AddRenderer( mRenderer2 );
    mMeshActor2.SetSize(400, 400);

    mMeshActor2.RegisterProperty( "a-n-other-property", Color::GREEN );
    Property::Index fadeColorIndex2 = mMeshActor2.RegisterProperty( "another-fade-color", Color::GREEN );
    mMeshActor2.AddUniformMapping( fadeColorIndex2, std::string("uFadeColor") );

    mRenderer2.RegisterProperty( "a-n-other-property", Vector3::ZERO );
    mRenderer2.RegisterProperty( "a-coefficient", 0.008f );
    fadeColorIndex2 = mRenderer2.RegisterProperty( "another-fade-color", Color::BLUE );
    mRenderer2.AddUniformMapping( fadeColorIndex2, std::string("uFadeColor" ) );
    mRenderer2.SetDepthIndex(0);

    mMeshActor2.SetParentOrigin( ParentOrigin::BOTTOM_CENTER );
    mMeshActor2.SetAnchorPoint( AnchorPoint::BOTTOM_CENTER );
    stage.Add( mMeshActor2 );

    Animation  animation = Animation::New(5);
    KeyFrames keyFrames = KeyFrames::New();
    keyFrames.Add(0.0f, Vector4::ZERO);
    keyFrames.Add(1.0f, Vector4( Color::GREEN ));

    KeyFrames keyFrames2 = KeyFrames::New();
    keyFrames2.Add(0.0f, Vector4::ZERO);
    keyFrames2.Add(1.0f, Color::MAGENTA);

    animation.AnimateBetween( Property( mRenderer, fadeColorIndex ), keyFrames, AlphaFunction(AlphaFunction::SIN) );
    animation.AnimateBetween( Property( mRenderer2, fadeColorIndex2 ), keyFrames2, AlphaFunction(AlphaFunctionSineX2) );
    animation.SetLooping(true);
    animation.Play();

    stage.SetBackgroundColor(Vector4(0.0f, 0.2f, 0.2f, 1.0f));;
  }

  BufferImage CreateBufferImage()
  {
    BufferImage image = BufferImage::New( 200, 200, Pixel::RGB888 );
    PixelBuffer* pixelBuffer = image.GetBuffer();
    unsigned int stride = image.GetBufferStride();
    for( unsigned int x=0; x<200; x++ )
    {
      for( unsigned int y=0; y<200; y++ )
      {
        PixelBuffer* pixel = pixelBuffer + y*stride + x*3;
        if( ((int)(x/20.0f))%2 + ((int)(y/20.0f)%2) == 1 )
        {
          pixel[0]=255;
          pixel[1]=0;
          pixel[2]=0;
          pixel[3]=255;
        }
        else
        {
          pixel[0]=0;
          pixel[1]=0;
          pixel[2]=255;
          pixel[3]=255;
        }
      }
    }
    image.Update();
    return image;
  }

  /**
   * Invoked whenever the quit button is clicked
   * @param[in] button the quit button
   */
  bool OnQuitButtonClicked( Toolkit::Button button )
  {
    // quit the application
    mApplication.Quit();
    return true;
  }

  void OnKeyEvent(const KeyEvent& event)
  {
    if(event.state == KeyEvent::Down)
    {
      if( IsKey( event, Dali::DALI_KEY_ESCAPE) || IsKey( event, Dali::DALI_KEY_BACK) )
      {
        mApplication.Quit();
      }
    }
  }

private:

  Application&  mApplication;                             ///< Application instance
  Vector3 mStageSize;                                     ///< The size of the stage

  Image    mImage;
  Sampler  mSampler1;
  Sampler  mSampler2;
  Shader   mShader;
  Material mMaterial1;
  Material mMaterial2;
  Geometry mGeometry;
  Renderer mRenderer;
  Actor    mMeshActor;
  Renderer mRenderer2;
  Actor    mMeshActor2;
  Timer    mChangeImageTimer;
};

void RunTest( Application& application )
{
  ExampleController test( application );

  application.MainLoop();
}

// Entry point for Linux & SLP applications
//
int main( int argc, char **argv )
{
  Application application = Application::New( &argc, &argv );

  RunTest( application );

  return 0;
}
