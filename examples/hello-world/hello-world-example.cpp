/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
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

#include <dali-toolkit/dali-toolkit.h>

using namespace Dali;
using namespace Dali::Toolkit;
#include <dali-toolkit/devel-api/controls/gaussian-blur-view/gaussian-blur-view.h>

const char* URL="https://upload.wikimedia.org/wikipedia/commons/2/2c/NZ_Landscape_from_the_van.jpg";

/**
 * This example shows how to use GaussianBlur UI control.
 */
class ImageBlurExample : public ConnectionTracker
{
public:

  // Initialize variables and connect signal
  ImageBlurExample( Application& application )
  : mApplication( application )
  {
    // Connect to the Application's Init signal
    mApplication.InitSignal().Connect( this, &ImageBlurExample::Create );
  }

  ~ImageBlurExample()
  {
    // Nothing to do here;
  }

  // The Init signal is received once (only) during the Application lifetime
  void Create( Application& application )
  {
    // Get a handle to the stage
    mStage = Stage::GetCurrent();
    // Set stage background color
    mStage.SetBackgroundColor( Color::BLACK );
    mStage.KeyEventSignal().Connect(this, &ImageBlurExample::OnKeyEvent);

    mImageView = ImageView::New();
    mImageView.SetProperty(ImageView::Property::IMAGE, Property::Map()
                           .Add(ImageVisual::Property::URL, URL)
                           .Add(ImageVisual::Property::LOAD_POLICY, ImageVisual::LoadPolicy::IMMEDIATE ) );

    mImageView.SetResizePolicy( ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS );
    mImageView.SetAnchorPoint(AnchorPoint::TOP_LEFT);

    auto sceneText = TextLabel::New( "Landscape photo");
    sceneText.SetResizePolicy( ResizePolicy::USE_NATURAL_SIZE, Dimension::ALL_DIMENSIONS );
    sceneText.SetProperty( TextLabel::Property::POINT_SIZE, 40 );
    sceneText.SetProperty( TextLabel::Property::TEXT_COLOR, Color::BLACK );
    sceneText.SetProperty( TextLabel::Property::OUTLINE, Property::Map().Add("color",Color::WHITE).Add("width", 2) );
    sceneText.SetParentOrigin( ParentOrigin::BOTTOM_CENTER );
    sceneText.SetAnchorPoint( AnchorPoint::BOTTOM_CENTER );
    sceneText.SetName("SceneText");
    mImageView.Add(sceneText);

    CreateBlurView( mImageView );
    mStage.KeepRendering( 1000000.0f );
  }

  void CreateBlurView( Actor scene )
  {
    mGaussianBlurView = GaussianBlurView::New(5, 1.5f, 0.5f, 0.5f);
    mGaussianBlurView.SetSize(mStage.GetSize()*0.8f);
    mGaussianBlurView.SetAnchorPoint( AnchorPoint::CENTER );
    mGaussianBlurView.SetParentOrigin( ParentOrigin::CENTER );
    mGaussianBlurView.SetVisible( true );
    mGaussianBlurView.Add( scene );
    mGaussianBlurView.SetBackgroundColor( Color::MAGENTA ); // All render tasks should use this color...

    mStage.Add(mGaussianBlurView);
    mGaussianBlurView.Activate();
  }


  void OnKeyEvent(const KeyEvent& event)
  {
    if(event.state == KeyEvent::Down)
    {
      if( IsKey( event, DALI_KEY_ESCAPE) || IsKey( event, DALI_KEY_BACK ) )
      {
        mApplication.Quit();
      }
      if( event.keyCode == 10 )
      {
        if( mGaussianBlurView )
        {
          mGaussianBlurView.Deactivate();
          printf("Deactivated\n");
        }
      }
      else if( event.keyCode == 11 )
      {
        if( mGaussianBlurView )
        {
          mGaussianBlurView.Activate();
          printf("Activated\n");
        }
      }
      else if( event.keyCode == 12 )
      {
        UnparentAndReset( mGaussianBlurView );
        printf("Destroyed\n");
      }
      else if( event.keyCode == 13 )
      {
        CreateBlurView( mImageView );
        printf("Re-created\n");
      }
    }
  }

private:
  Application&  mApplication;
  Stage mStage;
  GaussianBlurView mGaussianBlurView;
  ImageView mImageView;

};

// Entry point for Linux & Tizen applications
int main(int argc, char **argv)
{
  // Create application instance
  Application app = Application::New(&argc, &argv);
  ImageBlurExample test(app);

  // Start application.
  app.MainLoop();

  return 0;
}
