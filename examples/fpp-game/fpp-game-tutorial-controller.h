#ifndef FPPGAMETUTORIAL_H
#define FPPGAMETUTORIAL_H

/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
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

#include <dali-toolkit/public-api/controls/text-controls/text-label.h>
#include <dali/public-api/actors/actor.h>
#include <dali/public-api/adaptor-framework/window.h>
#include <dali/public-api/render-tasks/render-task.h>
#include <dali/public-api/signals/connection-tracker.h>

class FppGameTutorialController : public Dali::ConnectionTracker
{
public:
  /**
   * Creates new instance of FppGameTutorialController
   */
  FppGameTutorialController();

  /**
   * Destroys instance of FppGameTutorialController
   */
  ~FppGameTutorialController();

  /**
   * Displays UI with tutorial
   * @param[in] window The window to display the tutorial on
   */
  void DisplayTutorial(Dali::Window window);

  /**
   * Handles tutorial touch input
   * @param[in] touchEvent Incoming touch event in the UI space
   */
  void OnTouch(const Dali::TouchEvent& touchEvent);

  /**
   * Handles tutorial animation finished signal
   * @param[in] animation Animation object
   */
  void OnTutorialAnimationFinished(Dali::Animation& animation);

  /**
   * Handles tutorial completion window
   * @param animation Animation object
   */
  void OnTutorialComplete(Dali::Animation& animation);

private:
  Dali::RenderTask         mTutorialRenderTask;    /// RenderTask associated with rendering tutorial
  Dali::Actor              mUiRoot;                /// The parent actor for tutorial UI
  Dali::Toolkit::TextLabel mLeftLabel;             /// Text label displaying left message
  Dali::Toolkit::TextLabel mRightLabel;            /// Text label displaying right message
  bool                     mLeftTutorialComplete;  /// Flag indicating the walk (left) tutorial action has been performed
  bool                     mRightTutorialComplete; /// Flag indicating the look (right) tutorial action has been performed
  Dali::Window             mWindow;
};

#endif // FPPGAMETUTORIAL_H
