#ifndef DALI_DEMO_TRANSITION_APPLICATION_H
#define DALI_DEMO_TRANSITION_APPLICATION_H

/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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
 */

// External includes
#include <dali-toolkit/dali-toolkit.h>
//#include <dali-toolkit/devel-api/controls/slider/slider.h>
#include <dali-toolkit/devel-api/controls/popup/popup.h>
#include "beat-control.h"
#include <cstdio>
#include <sstream>

// Internal includes

using namespace Dali;
using namespace Dali::Toolkit;

namespace Demo
{

class TransitionApplication : public ConnectionTracker
{
public:
  // Constructor
  TransitionApplication( Application& application );

  // Destructor
  ~TransitionApplication();

  // Init signal handler
  void Create( Application& application );

  // Create the GUI components
  Toolkit::TextLabel CreateTitle( std::string title );
  Actor CreateContentPane();

  // Key event handler
  void OnKeyEvent( const KeyEvent& event );

  bool OnActionButtonClicked(Button button);

  static const char* DEMO_THEME_ONE_PATH;

private:
  Application& mApplication;
  TextLabel mTitle;
  BeatControl mBeatControl;
  PushButton mActionButtons[3];
  Property::Index mActionIndex;
};

} // Namespace Demo


#endif // DALI_DEMO_TRANSITION_APPLICATION_H